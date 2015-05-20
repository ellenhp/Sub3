#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include "SubScreen.hpp"

//This class controls the window that the user will see and interact with.
class SubWindow
{
public:
    SubWindow(sf::VideoMode videoMode);

    //Start the event loop. This won't return until the window is closed.
    void run();

    //Tempate function to switch to a new SubScreen.
    template<typename NewScreen>
    void switchToScreen()
    {
        mDesktop.RemoveAll();
        if (mCurrentScreen)
        {
            delete mCurrentScreen;
        }
        mCurrentScreen = new NewScreen(this);
        mCurrentScreen->setupScreen(mDesktop);
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
