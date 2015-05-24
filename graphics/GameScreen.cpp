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

#include "GameScreen.hpp"

#include "Sub3.hpp"
#include "graphics/SubWindow.hpp"
#include "simulation/GameManager.hpp"

#include <SFGUI/Widgets.hpp>

#include <sstream>

GameScreen::GameScreen(SubWindow* subWindow) :
    mSubWindow(subWindow), mGameWindow(NULL), mVesselWidget(NULL), mVesselUI(NULL)
{
}

void GameScreen::setupScreen(sfg::Desktop& desktop, std::vector<std::string> args)
{
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Get the current GameManager and make sure it isn't NULL.
    auto gameMgr = GameManager::getCurrent().lock();
    BOOST_ASSERT_MSG(gameMgr, "Fatal: GameManager doesn't exist in GameScreen.setupScreen()");

    //Get the current vessel's UI.
    auto currentVessel = gameMgr->getCurrentVessel();
    mVesselUI = currentVessel->constructUI();
    mVesselWidget = mVesselUI->setupUI();
    box->Pack(mVesselWidget);

    //Make a box for the toolbar thing at the bottom.
    auto bottomBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    //Version string for the toolbar.
    std::stringstream versionStream;
    versionStream << "Sub^3 version: " << subVersionMajor << "." << subVersionMinor;

    //Toolbar widgets.
    auto versionLabel = sfg::Label::Create(versionStream.str());
    auto quitButton = sfg::Button::Create("Quit to Main Menu");

    //Pack the toolbar widgets.
    bottomBox->Pack(versionLabel);
    bottomBox->Pack(quitButton, false, false);

    //Pack the toolbar.
    box->Pack(bottomBox, false, false);

    mGameWindow = sfg::Window::Create(0);
    mGameWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mGameWindow);
    fillWindow();
}

void GameScreen::updateScreen(float dt)
{
    if (mVesselWidget)
    {
        mVesselUI->updateUI(dt);
    }
    static int lastWidth = 0;
    static int lastHeight = 0;
    if (mSubWindow->getWidth() != lastWidth || mSubWindow->getHeight() != lastHeight)
    {
        fillWindow();
        lastWidth = mSubWindow->getWidth();
        lastHeight = mSubWindow->getHeight();
    }

    auto gameManager = GameManager::getCurrent().lock();
    BOOST_ASSERT_MSG(gameManager, "Fatal: GameManager doesn't exist");
    gameManager->tick(dt);
}

void GameScreen::fillWindow()
{
    //Center the window.
    float width = 0.8f * mSubWindow->getWidth();
    float height = 0.8f * mSubWindow->getHeight();
    float winX = 0.1f * mSubWindow->getWidth();
    float winY = 0.1f * mSubWindow->getHeight();
    mGameWindow->SetAllocation({winX, winY, width, height});

}
