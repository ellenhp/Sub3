#include "LoadingScreen.hpp"

#include "SubWindow.hpp"
#include "GameScreen.hpp"
#include "../simulation/GameManager.hpp"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <chrono>

#include <usml/ocean/ocean_shared.h>
#include <usml/waveq3d/waveq3d.h>
#include <usml/netcdf/netcdf_files.h>

#include <boost/assert.hpp>

using std::chrono::steady_clock;

using usml::netcdf::netcdf_woa;
using usml::netcdf::netcdf_bathy;
using namespace usml::ocean;

LoadingScreen::LoadingScreen(SubWindow* subWindow) :
    mSubWindow(subWindow), mLoadingWindow(NULL), mLoadingDone(false), mLaunchGame(false)
{
}

LoadingScreen::~LoadingScreen()
{
    mLoadingThread = NULL;
    mLoadingWindow = NULL;
}

void LoadingScreen::setupScreen(sfg::Desktop& desktop, std::vector<std::string> args)
{
    //Load the IP and port number.
    BOOST_ASSERT_MSG(args.size() == 2, "Fatal: LoadingScreen initialized incorrectly.");
    mIpAddress = args[0];
    mPortNumber = stoi(args[1]);

    subDebug << "Connecting to " << mIpAddress << ":" << mPortNumber << std::endl;

    //Kick off the loading thread.
    mLoadingThread = std::unique_ptr<std::thread>(new std::thread(&LoadingScreen::doLoading, this));

    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Back button.
    mSpinner = sfg::Spinner::Create();
    mSpinner->Start();

    //Loading label.
    mLabel = sfg::Label::Create("");

    //Layout the widgets.
    box->Pack(mSpinner);
    box->Pack(mLabel);
    box->SetSpacing(10);

    //Put the box in a window.
    mLoadingWindow = sfg::Window::Create(0);
    mLoadingWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mLoadingWindow);
    centerWindow();
}

void LoadingScreen::updateScreen()
{
    mLoadingMutex.lock();
    mLabel->SetText(mLoadingText);
    mLoadingMutex.unlock();

    mLoadingMutex.lock();
    bool loadingDone = mLoadingDone;
    bool launchGame = mLaunchGame;
    mLoadingMutex.unlock();

    if (loadingDone)
    {
        mSpinner->Stop();
    }

    if (launchGame)
    {
        BOOST_ASSERT_MSG(mLoadingThread, "Fatal: LoadingScreen tried to join thread but it's null");
        mLoadingThread->join();
        subDebug << "Starting game screen" << std::endl;
        mSubWindow->switchToScreen<GameScreen>();
        return;
    }

    centerWindow();
}

void LoadingScreen::centerWindow()
{
    //Center the window.
    float width = mLoadingWindow->GetAllocation().width;
    float height = mLoadingWindow->GetAllocation().height;
    float winX = (mSubWindow->getWidth() - width) / 2;
    float winY = (mSubWindow->getHeight() - height) / 2;
    mLoadingWindow->SetAllocation({winX, winY, width, height});
}

void LoadingScreen::doLoading()
{
    mLoadingMutex.lock();
    mLoadingText = "Connecting to Game...";
    auto copyIP = mIpAddress;
    auto copyPort = mPortNumber;
    mLoadingMutex.unlock();

    auto tcpSocket = std::make_shared<sf::TcpSocket>();
    tcpSocket->connect(sf::IpAddress(copyIP), copyPort);

    mGameSocket = std::make_shared<SubSocket>(tcpSocket);

    auto gameManager = std::make_shared<GameManager>();
    GameManager::setCurrent(gameManager);

    //Wait for the set player id message (or any others) to verify we connected successfully.
    bool isInitialized = false;
    auto startTime = steady_clock::now();
    while (steady_clock::now() - startTime < std::chrono::seconds(5))
    {
        auto end_time = steady_clock::now() + std::chrono::milliseconds(100);

        std::shared_ptr<Message> message = NULL;
        while (mGameSocket->hasPackets())
        {
            BOOST_ASSERT_MSG(*mGameSocket >> message, "Fatal: Failed to load init message");
            message->execute();
        }

        if (gameManager->isInitialized())
        {
            isInitialized = true;
            break;
        }

        std::this_thread::sleep_until(end_time);
    }

    //Let the user know if the connection timed out.
    if (!isInitialized)
    {
        mLoadingMutex.lock();
        mLoadingText = "Connection Timed Out.";
        mLoadingDone = true;
        mLoadingMutex.unlock();

        //Give the user some time to read the message we just showed them.
        //But make sure we keep pulling messages off the networking buffer.
        auto endTime = steady_clock::now() + std::chrono::milliseconds(1500);
        while (steady_clock::now() < endTime)
        {
            mGameSocket->hasPackets();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        //Now we'll let the main thread know we're ready.
        mLoadingMutex.lock();
        mLaunchGame = true;
        mLoadingMutex.unlock();
        return;
    }
    else
    {
        //TODO: wait until we get a message that tells us what month it is and where we are.
    }

    mLoadingMutex.lock();
    mLoadingText = "Loading Ocean Temperature Data...";
    mLoadingMutex.unlock();

    //TODO: use correct location.
    const double lat1 = 30.0;
    const double lat2 = 46.0;
    const double lng1 = -8.0;
    const double lng2 = 37.0;

    //Load temperature and salinity.
    //TODO: Use the correct month.
    netcdf_woa* temperature = new netcdf_woa(
        "usml/woa09/temperature_seasonal_1deg.nc",
        "usml/woa09/temperature_monthly_1deg.nc",
        10, lat1, lat2, lng1, lng2);

    mLoadingMutex.lock();
    mLoadingText = "Loading Ocean Salinity Data...";
    mLoadingMutex.unlock();

    netcdf_woa* salinity = new netcdf_woa(
        "usml/woa09/salinity_seasonal_1deg.nc",
        "usml/woa09/salinity_monthly_1deg.nc",
        10, lat1, lat2, lng1, lng2);

    //Compute sound speed.
    profile_model* profile = new profile_lock(new profile_grid<double,3>(
        data_grid_mackenzie::construct(temperature, salinity)));

    mLoadingMutex.lock();
    mLoadingText = "Loading Bathymetry...";
    mLoadingMutex.unlock();

    //Load bathymetry.
    boundary_model* bottom = new boundary_lock(new boundary_grid<double,2>(new netcdf_bathy(
        "usml/bathymetry/ETOPO1_Ice_g_gmt4.grd",
        lat1, lat2, lng1, lng2)));

    //TODO: Get weather data somehow?
    boundary_model* surface = new boundary_lock(new boundary_flat());

    //Updated the ocean_shared reference.
    ocean_shared::reference ocean(new ocean_model(surface, bottom, profile));
    ocean_shared::update(ocean);

    mLoadingMutex.lock();
    mLoadingText = "Done!";
    mLoadingDone = true;
    mLoadingMutex.unlock();

    //Give the user some time to read the message we just showed them.
    //But make sure we keep pulling messages off the networking buffer.
    auto endTime = steady_clock::now() + std::chrono::milliseconds(1500);
    while (steady_clock::now() < endTime)
    {
        mGameSocket->hasPackets();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    //Now we'll let the main thread know we're ready.
    mLoadingMutex.lock();
    mLaunchGame = true;
    mLoadingMutex.unlock();

    subDebug << "Loading thread done" << std::endl;
}
