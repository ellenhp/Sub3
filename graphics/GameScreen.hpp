#pragma once

#include "graphics/SubScreen.hpp"
#include "simulation/Vessel.hpp"

#include <SFGUI/Widgets.hpp>

class SubWindow;

class GameScreen : public SubScreen
{
public:
    GameScreen(SubWindow* subWindow);

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);
    void updateScreen(float dt);

private:
    void fillWindow();

    SubWindow* mSubWindow;

    sfg::Window::Ptr mGameWindow;
    sfg::Widget::Ptr mVesselWidget;

    std::shared_ptr<Vessel::VesselUI> mVesselUI;

};
