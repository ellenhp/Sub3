/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LoadingScreen.hpp"

#include "graphics/SubWindow.hpp"
#include "graphics/GameScreen.hpp"
#include "network/SubSocket.hpp"
#include "simulation/GameManager.hpp"
#include "simulation/USMLManager.hpp"

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
    mSubWindow(subWindow), mLoadingDone(false), mLaunchGame(false), mLoadingWindow(NULL)
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

void LoadingScreen::updateScreen(float dt)
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

        auto gameManager = GameManager::getCurrent().lock();
        BOOST_ASSERT_MSG(gameManager, "Fatal: GameManager doesn't exist");
        gameManager->setSocket(mGameSocket);

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

    mLoadingMutex.lock();
    mLoadingText = "Loading Oceanographic Data...";
    mLoadingMutex.unlock();

    subDebug << "Loading Oceanographic Data..." << std::endl;

    //Let the USMLManager do the loading.
    BOOST_ASSERT_MSG(gameManager->isAlive(), "Fatal: not alive to load oceanographic data");
    auto pos = gameManager->getCurrentVessel()->getState().getLocation();
    USMLManager::getInstance()->ensureDataAround(pos, true);

    subDebug << "Done loading Oceanographic Data..." << std::endl;

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
