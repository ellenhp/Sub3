#include "SubWindow.hpp"

#include <chrono>

#include <SFML/Window/Event.hpp>

#include "MainMenu.hpp"
#include "../Sub3.hpp"

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
            mCurrentScreen->updateScreen();
        }

        //Draw the GUI.
        mRenderWindow.clear();
        mGui.Display(mRenderWindow);
        mRenderWindow.display();
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
