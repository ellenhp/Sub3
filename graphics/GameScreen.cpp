#include "GameScreen.hpp"

#include "../simulation/GameManager.hpp"
#include "../Sub3.hpp"

#include <SFGUI/Widgets.hpp>

#include <sstream>

GameScreen::GameScreen(SubWindow* subWindow) :
    mSubWindow(subWindow), mGameWindow(NULL), mVesselUI(NULL)
{
}

void GameScreen::setupScreen(sfg::Desktop& desktop, std::vector<std::string> args)
{
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Get the current GameManager and make sure it isn't NULL.
    auto gameMgr = GameManager::getCurrent().lock();
    BOOST_ASSERT_MSG(gameMgr, "Fatal: GameManager doesn't exist in GameScreen.setupScreen()");

    //Get the current vessel's UI.
    auto currentVessel = gameMgr->getCurrentVessel();
    mVesselUI = currentVessel->constructUI();
    box->Pack(mVesselUI->setupUI());

    //Make a box for the toolbar thing at the bottom.
    auto bottomBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    //Version string for the toolbar.
    std::stringstream versionStream;
    versionStream << "Sub^3 version: " << subVersionMajor << "." << subVersionMinor;

    //Toolbar widgets.
    auto versionLabel = sfg::Label::Create(versionStream.str());
    auto quitButton = sfg::Button::Create("Quit to Main Menu");

    //Pack the toolbar widgets.
    bottomBox->Pack(versionLabel);
    bottomBox->Pack(quitButton, false, false);

    //Pack the toolbar.
    box->Pack(bottomBox, false, false);

    mGameWindow = sfg::Window::Create(0);
    mGameWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mGameWindow);
    fillWindow();
}

void GameScreen::updateScreen()
{
    static int lastWidth = 0;
    static int lastHeight = 0;
    if (mSubWindow->getWidth() != lastWidth || mSubWindow->getHeight() != lastHeight)
    {
        fillWindow();
        lastWidth = mSubWindow->getWidth();
        lastHeight = mSubWindow->getHeight();
    }
}

void GameScreen::fillWindow()
{
    //Center the window.
    float width = 0.8f * mSubWindow->getWidth();
    float height = 0.8f * mSubWindow->getHeight();
    float winX = 0.1f * mSubWindow->getWidth();
    float winY = 0.1f * mSubWindow->getHeight();
    mGameWindow->SetAllocation({winX, winY, width, height});

}
