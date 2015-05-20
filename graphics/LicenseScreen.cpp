#include "LicenseScreen.hpp"

#include "SubWindow.hpp"
#include "MainMenu.hpp"

#include <SFGUI/Widgets.hpp>

#include <fstream>
#include <sstream>

LicenseScreen::LicenseScreen(SubWindow* subWindow) :
    mSubWindow(subWindow)
{
}

void LicenseScreen::setupScreen(sfg::Desktop& desktop)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

    //Back button.
    auto backButton = sfg::Button::Create("Back");

    //Hook up event handler.
    backButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&LicenseScreen::backHandler, this)
    );

    //Load the license file.
    std::ifstream creditsStream("CREDITS");
    std::stringstream creditsBuffer;
    creditsBuffer << creditsStream.rdbuf();

    auto licenseLabel = sfg::Label::Create(creditsBuffer.str());

    //Layout the widgets.
    box->Pack(backButton, false, false);
    box->Pack(licenseLabel);
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

void LicenseScreen::backHandler()
{
    mSubWindow->switchToScreen<MainMenu>();
}
