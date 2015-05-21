#pragma once

#include "SubScreen.hpp"

#include <SFGUI/Widgets.hpp>

class SubWindow;

//This class is the main menu for the game.
class MainMenu : public SubScreen
{
public:
    MainMenu(SubWindow* subWindow);
    void setupScreen(sfg::Desktop& desktop);

private:
    SubWindow* mSubWindow;

    sfg::Window::Ptr mConnectWindow;

    sfg::Entry::Ptr mIpEntry;
    sfg::Entry::Ptr mPortEntry;

    sfg::Desktop* mDesktop;

    void playHandler();
    void licenseHandler();
    void quitHandler();

    void playConnectHandler();
    void playCancelHandler();
};
