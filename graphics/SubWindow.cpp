#include "SubWindow.hpp"

#include <SFML/Window/Event.hpp>

#include "MainMenu.hpp"

SubWindow::SubWindow(sf::VideoMode videoMode) :
    mRenderWindow(videoMode, "Sub^3"), mCurrentScreen(NULL)
{
    //Don't let the window show itself until it can respond to events.
    mRenderWindow.setVisible(false);
}

void SubWindow::run()
{
    //We're ready to show the window.
    mRenderWindow.setVisible(true);

    mRenderWindow.resetGLStates();

    //We always want to show the main menu first.
    switchToScreen<MainMenu>();

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
        mDesktop.Update(1.0f);

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
