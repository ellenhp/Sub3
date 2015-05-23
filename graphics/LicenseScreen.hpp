#pragma once

#include "graphics/SubScreen.hpp"

#include <SFGUI/Widgets.hpp>

class SubWindow;

//This class is the license/credits screen for the game.
class LicenseScreen : public SubScreen
{
public:
    LicenseScreen(SubWindow* subWindow);
    virtual ~LicenseScreen();

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);

    void updateScreen(float dt);

private:
    SubWindow* mSubWindow;
    sfg::Window::Ptr mLicenseWindow;

    void backHandler();

    void centerWindow();
};
