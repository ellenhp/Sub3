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

#include "MainMenu.hpp"

#include "graphics/SubWindow.hpp"
#include "graphics/LicenseScreen.hpp"
#include "graphics/LoadingScreen.hpp"

//Keep around the SubWindow pointer for event handling purposes.
MainMenu::MainMenu(SubWindow& subWindow) :
    mSubWindow(subWindow), mMainWindow(NULL), mConnectWindow(NULL),
    mIpEntry(NULL), mPortEntry(NULL)
{
}

MainMenu::~MainMenu()
{
    mConnectWindow = NULL;
    mMainWindow = NULL;
}

void MainMenu::setupScreen(sfg::Desktop& desktop, std::vector<std::string> args)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Main menu buttons.
    auto playButton = sfg::Button::Create("Play Game");
    auto licenseButton = sfg::Button::Create("View License");
    auto quitButton = sfg::Button::Create("Quit Game");

    //Hook up event handlers.
    playButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::playHandler, this)
    );
    licenseButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::licenseHandler, this)
    );
    quitButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::quitHandler, this)
    );

    //Layout the buttons.
    box->Pack(playButton);
    box->Pack(licenseButton);
    box->Pack(quitButton);
    box->SetSpacing(10);

    //Put the box in a window.
    mMainWindow = sfg::Window::Create(0);
    mMainWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mMainWindow);
    centerWindow();

    mDesktop = &desktop;
}

void MainMenu::updateScreen(float dt)
{
    static int lastWidth = 0;
    static int lastHeight = 0;
    if (mSubWindow.getWidth() != lastWidth || mSubWindow.getHeight() != lastHeight)
    {
        centerWindow();
        lastWidth = mSubWindow.getWidth();
        lastHeight = mSubWindow.getHeight();
    }
}

void MainMenu::playHandler()
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Play window widgets.
    mIpEntry = sfg::Entry::Create("127.0.0.1");
    mPortEntry = sfg::Entry::Create("22969");

    auto buttonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    auto cancelButton = sfg::Button::Create("Cancel");
    auto connectButton = sfg::Button::Create("Connect");

    //Hook up event handlers.
    cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::playCancelHandler, this)
    );
    connectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&MainMenu::playConnectHandler, this)
    );

    //Layout the buttons.
    box->Pack(sfg::Label::Create("IP Address:"));
    box->Pack(mIpEntry);
    box->Pack(sfg::Label::Create("Port Number:"));
    box->Pack(mPortEntry);
    buttonBox->Pack(cancelButton);
    buttonBox->Pack(connectButton);
    buttonBox->SetSpacing(10);
    box->Pack(buttonBox);
    box->SetSpacing(10);

    //Put the box in a window.
    mConnectWindow = sfg::Window::Create();
    mConnectWindow->SetTitle("Connect To Game");
    mConnectWindow->Add(box);

    //Center the window then add it to the desktop.
    float width = mConnectWindow->GetAllocation().width;
    float height = mConnectWindow->GetAllocation().height;
    float winX = (mSubWindow.getWidth() - width) / 2;
    float winY = (mSubWindow.getHeight() - height) / 2;
    mConnectWindow->SetAllocation({winX, winY, width, height});

    mDesktop->Add(mConnectWindow);
}

void MainMenu::licenseHandler()
{
    mSubWindow.switchToScreen<LicenseScreen>();
}

void MainMenu::quitHandler()
{
    mSubWindow.quit();
}

void MainMenu::playConnectHandler()
{
    int port = std::stoi((std::string)mPortEntry->GetText());
    if (port > 0 && port < 0xFFFF)
    {
        mDesktop->Remove(mConnectWindow);
        mSubWindow.switchToScreen<LoadingScreen>({mIpEntry->GetText(), mPortEntry->GetText()});
    }
    else
    {
        //TODO: Let the user know why nothing happened
    }
}

void MainMenu::playCancelHandler()
{
    mDesktop->Remove(mConnectWindow);
    mConnectWindow = NULL;
    mIpEntry = NULL;
    mPortEntry = NULL;
}

void MainMenu::centerWindow()
{
    //Center the window.
    float width = mMainWindow->GetAllocation().width;
    float height = mMainWindow->GetAllocation().height;
    float winX = (mSubWindow.getWidth() - width) / 2;
    float winY = (mSubWindow.getHeight() - height) / 2;
    mMainWindow->SetAllocation({winX, winY, width, height});
}
