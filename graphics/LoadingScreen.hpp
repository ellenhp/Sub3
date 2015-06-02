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

#pragma once

#include "graphics/SubScreen.hpp"
#include "Sub3.hpp"

#include <SFGUI/Widgets.hpp>

#include <thread>
#include <mutex>

#include <usml/ocean/ocean_shared.h>

class SubWindow;
class SubSocket;

//This class opens a connection and initializes USML
class LoadingScreen : public SubScreen
{
public:
    LoadingScreen(SubWindow& subWindow);
    virtual ~LoadingScreen();

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);

    void updateScreen(float dt);

private:
    SubWindow& mSubWindow;

    //Stuff required to load and connect asynchronously.
    std::unique_ptr<std::thread> mLoadingThread;
    std::mutex mLoadingMutex;
    bool mLoadingDone;
    bool mLaunchGame;

    std::shared_ptr<SubSocket> mGameSocket;
    std::shared_ptr<usml::ocean::ocean_shared> mGameOcean;

    std::string mLoadingText;

    sfg::Spinner::Ptr mSpinner;
    sfg::Label::Ptr mLabel;
    sfg::Window::Ptr mLoadingWindow;

    std::string mIpAddress;
    uint16_t mPortNumber;

    void centerWindow();

    void doLoading();


};
