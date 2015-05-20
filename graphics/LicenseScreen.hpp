#pragma once

#include "SubScreen.hpp"

class SubWindow;

//This class is the license/credits screen for the game.
class LicenseScreen : public SubScreen
{
public:
    LicenseScreen(SubWindow* subWindow);
    void setupScreen(sfg::Desktop& desktop);

private:
    SubWindow* mSubWindow;

    void backHandler();
};
