#pragma once

#include "SubScreen.hpp"

#include <SFGUI/Widgets.hpp>

class SubWindow;

//This class is the main menu for the game.
class MainMenu : public SubScreen
{
public:
    MainMenu(SubWindow* subWindow);
    virtual ~MainMenu();

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);

    void updateScreen();

private:
    SubWindow* mSubWindow;

    sfg::Window::Ptr mMainWindow;
    sfg::Window::Ptr mConnectWindow;

    sfg::Entry::Ptr mIpEntry;
    sfg::Entry::Ptr mPortEntry;

    sfg::Desktop* mDesktop;

    void centerWindow();

    void playHandler();
    void licenseHandler();
    void quitHandler();

    void playConnectHandler();
    void playCancelHandler();
};
