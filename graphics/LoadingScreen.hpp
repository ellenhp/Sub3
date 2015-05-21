#pragma once

#include "SubScreen.hpp"

#include <SFGUI/Widgets.hpp>

class SubWindow;

//This class opens a connection and initializes USML
class LoadingScreen : public SubScreen
{
public:
    LoadingScreen(SubWindow* subWindow);
    virtual ~LoadingScreen();

    void setupScreen(sfg::Desktop& desktop);

    void updateScreen();

private:
    SubWindow* mSubWindow;

    sfg::Label::Ptr mLabel;
    sfg::Window::Ptr mLoadingWindow;

};
