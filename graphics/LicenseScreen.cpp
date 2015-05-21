#include "LicenseScreen.hpp"

#include "SubWindow.hpp"
#include "MainMenu.hpp"

#include <fstream>
#include <sstream>

LicenseScreen::LicenseScreen(SubWindow* subWindow) :
    mSubWindow(subWindow)
{
}

LicenseScreen::~LicenseScreen()
{
    mLicenseWindow = NULL;
}

void LicenseScreen::setupScreen(sfg::Desktop& desktop, std::vector<std::string> args)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

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
    mLicenseWindow = sfg::Window::Create(0);
    mLicenseWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mLicenseWindow);
    centerWindow();
}

void LicenseScreen::updateScreen()
{
    static int lastWidth = 0;
    static int lastHeight = 0;
    if (mSubWindow->getWidth() != lastWidth || mSubWindow->getHeight() != lastHeight)
    {
        centerWindow();
        lastWidth = mSubWindow->getWidth();
        lastHeight = mSubWindow->getHeight();
    }
}

void LicenseScreen::centerWindow()
{
    //Center the window.
    float width = mLicenseWindow->GetAllocation().width;
    float height = mLicenseWindow->GetAllocation().height;
    float winX = (mSubWindow->getWidth() - width) / 2;
    float winY = (mSubWindow->getHeight() - height) / 2;
    mLicenseWindow->SetAllocation({winX, winY, width, height});
}

void LicenseScreen::backHandler()
{
    mSubWindow->switchToScreen<MainMenu>();
}
