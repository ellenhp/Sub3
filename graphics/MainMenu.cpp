#include "MainMenu.hpp"

#include "SubWindow.hpp"
#include "LicenseScreen.hpp"

//Keep around the SubWindow pointer for event handling purposes.
MainMenu::MainMenu(SubWindow* subWindow) :
    mSubWindow(subWindow), mConnectWindow(NULL), mIpEntry(NULL), mPortEntry(NULL)
{
}

void MainMenu::setupScreen(sfg::Desktop& desktop)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Main menu buttons.
    auto playButton = sfg::Button::Create("Play Game");
    auto optionsButton = sfg::Button::Create("Change Preferences");
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

    mDesktop = &desktop;
}

void MainMenu::playHandler()
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Play window widgets.
    auto ipEntry = sfg::Entry::Create("127.0.0.1");
    auto portEntry = sfg::Entry::Create("22969");

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
    box->Pack(ipEntry);
    box->Pack(sfg::Label::Create("Port Number:"));
    box->Pack(portEntry);
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
    float winX = (mSubWindow->getWidth() - width) / 2;
    float winY = (mSubWindow->getHeight() - height) / 2;
    mConnectWindow->SetAllocation({winX, winY, width, height});

    mDesktop->Add(mConnectWindow);
}

void MainMenu::licenseHandler()
{
    mSubWindow->switchToScreen<LicenseScreen>();
}

void MainMenu::quitHandler()
{
    mSubWindow->quit();
}

void MainMenu::playConnectHandler()
{
    mDesktop->Remove(mConnectWindow);
    mConnectWindow = NULL;
}

void MainMenu::playCancelHandler()
{
    mDesktop->Remove(mConnectWindow);
    mConnectWindow = NULL;
}
