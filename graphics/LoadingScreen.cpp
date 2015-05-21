#include "LoadingScreen.hpp"

#include "SubWindow.hpp"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <chrono>

using std::chrono::steady_clock;

LoadingScreen::LoadingScreen(SubWindow* subWindow) :
    mSubWindow(subWindow), mLoadingWindow(NULL), mLoadingDone(false), mLaunchGame(false)
{
}

LoadingScreen::~LoadingScreen()
{
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
        mSubWindow->quit();
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
    mLoadingText = "Loading USML...";
    mLoadingMutex.unlock();

    //TODO: Load the temperature and salinity data and create an ocean_shared object

    mLoadingMutex.lock();
    mLoadingText = "Connecting to Game...";
    auto copyIP = mIpAddress;
    auto copyPort = mPortNumber;
    mLoadingMutex.unlock();

    auto tcpSocket = std::make_shared<sf::TcpSocket>();
    tcpSocket->connect(sf::IpAddress(copyIP), copyPort);

    mGameSocket = std::make_shared<SubSocket>(tcpSocket);

    //Wait for the set player id message (or any others) to verify we connected successfully.
    bool hasPackets = false;
    auto startTime = steady_clock::now();
    while (steady_clock::now() - startTime < std::chrono::seconds(5))
    {
        auto end_time = steady_clock::now() + std::chrono::milliseconds(100);

        if (mGameSocket->hasPackets())
        {
            hasPackets = true;
            break;
        }

        std::this_thread::sleep_until(end_time);
    }

    //Let the user know if we connected successfully
    if (hasPackets)
    {
        mLoadingMutex.lock();
        mLoadingText = "Done!";
        mLoadingDone = true;
        mLoadingMutex.unlock();
    }
    else
    {
        mLoadingMutex.lock();
        mLoadingText = "Connection Timed Out.";
        mLoadingDone = true;
        mLoadingMutex.unlock();
    }

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
}
