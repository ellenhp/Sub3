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

#include "SubWindow.hpp"

#include <thread>
#include <chrono>

#include <SFML/Window/Event.hpp>

#include "graphics/MainMenu.hpp"
#include "Sub3.hpp"

typedef std::chrono::duration<int, std::ratio<1, 60>> frameTime;

SubWindow::SubWindow(sf::VideoMode videoMode) :
    mRenderWindow(videoMode, "Sub^3"), mCurrentScreen(NULL)
{
    //Don't let the window show itself until it can respond to events.
    mRenderWindow.setVisible(false);
}

void SubWindow::run()
{
    using seconds = std::chrono::duration<float, std::ratio<1, 1>>;

    //We're ready to show the window.
    mRenderWindow.setVisible(true);

    mRenderWindow.resetGLStates();

    //We always want to show the main menu first.
    switchToScreen<MainMenu>();

    //This clock is used for the SFGUI Update method.
    auto startTime = std::chrono::steady_clock::now();

    //Handle events indefinitely.
    sf::Event event;
    while (mRenderWindow.isOpen()) {
        auto startTime = std::chrono::steady_clock::now();

        while (mRenderWindow.pollEvent(event)) {
            mDesktop.HandleEvent(event);
            if (event.type == sf::Event::Closed) {
                mRenderWindow.close();
            }
        }
        //Update the desktop.
        auto endTime = std::chrono::steady_clock::now();
        auto duration = endTime - startTime;
        mDesktop.Update(seconds(duration).count());
        startTime = std::chrono::steady_clock::now();

        //Update the screen.
        if (mCurrentScreen)
        {
            mCurrentScreen->updateScreen(seconds(duration).count());
        }

        //Draw the GUI.
        mRenderWindow.clear();
        mGui.Display(mRenderWindow);
        mRenderWindow.display();

        std::this_thread::sleep_until(startTime + frameTime(1));
    }
}

void SubWindow::quit()
{
    mRenderWindow.close();
}

int SubWindow::getWidth()
{
    return mRenderWindow.getSize().x;
}

int SubWindow::getHeight()
{
    return mRenderWindow.getSize().y;
}
