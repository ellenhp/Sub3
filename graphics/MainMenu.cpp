#include "MainMenu.hpp"

#include "SubWindow.hpp"

#include <SFGUI/Widgets.hpp>

//Keep around the SubWindow pointer for event handling purposes.
MainMenu::MainMenu(SubWindow* subWindow) :
    mSubWindow(subWindow)
{
}

void MainMenu::setupScreen(sfg::Desktop& desktop)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

    //Main menu buttons.
    auto connectButton = sfg::Button::Create("Connect to Game");
    auto optionsButton = sfg::Button::Create("Change Preferences");
    auto licenseButton = sfg::Button::Create("View License");
    auto quitButton = sfg::Button::Create("Quit Game");

    //Hook up event handlers.
    quitButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::quitHandler, this)
    );

    //Layout the buttons.
    box->Pack(connectButton);
    box->Pack(optionsButton);
    box->Pack(licenseButton);
    box->Pack(quitButton);
    box->SetSpacing(10);

    //Put the box in a window.
    auto window = sfg::Window::Create(0);
    window->Add(box);

    //Center the window then add it to the desktop.
    float width = window->GetAllocation().width;
    float height = window->GetAllocation().height;
    float winX = (mSubWindow->getWidth() - width) / 2;
    float winY = (mSubWindow->getHeight() - height) / 2;
    window->SetAllocation({winX, winY, width, height});

    desktop.Add(window);
}

void MainMenu::quitHandler()
{
    mSubWindow->quit();
}
