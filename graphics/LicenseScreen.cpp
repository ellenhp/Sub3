/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LicenseScreen.hpp"

#include "graphics/SubWindow.hpp"
#include "graphics/MainMenu.hpp"

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

void LicenseScreen::updateScreen(float dt)
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
