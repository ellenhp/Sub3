#pragma once

#include "graphics/SubScreen.hpp"
#include "graphics/SubWindow.hpp"
#include "simulation/Vessel.hpp"

#include <SFGUI/Widgets.hpp>

class GameScreen : public SubScreen
{
public:
    GameScreen(SubWindow* subWindow);

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);
    void updateScreen();

private:
    void fillWindow();

    SubWindow* mSubWindow;

    sfg::Window::Ptr mGameWindow;

    std::shared_ptr<Vessel::VesselUI> mVesselUI;

};
