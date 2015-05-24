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

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include "graphics/SubScreen.hpp"

//This class controls the window that the user will see and interact with.
class SubWindow
{
public:
    SubWindow(sf::VideoMode videoMode);

    //Start the event loop. This won't return until the window is closed.
    void run();

    //Tempate function to switch to a new SubScreen.
    template<typename NewScreen>
    void switchToScreen(std::vector<std::string> args = {})
    {
        mDesktop.RemoveAll();
        if (mCurrentScreen)
        {
            delete mCurrentScreen;
        }
        mCurrentScreen = new NewScreen(this);
        mCurrentScreen->setupScreen(mDesktop, args);
    }

    //Close the window.
    void quit();

    //Gets the dimensions of the window.
    int getWidth();
    int getHeight();

private:
    sfg::SFGUI mGui;
    sfg::Desktop mDesktop;
    sf::RenderWindow mRenderWindow;

    SubScreen* mCurrentScreen;
};
