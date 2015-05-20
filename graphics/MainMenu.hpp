#pragma once

#include "SubScreen.hpp"

class SubWindow;

//This class is the main menu for the game.
class MainMenu : public SubScreen
{
public:
    MainMenu(SubWindow* subWindow);
    void setupScreen(sfg::Desktop& desktop);

private:
    SubWindow* mSubWindow;

    void quitHandler();
};
