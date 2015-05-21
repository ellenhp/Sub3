#include "LoadingScreen.hpp"

#include "SubWindow.hpp"

LoadingScreen::LoadingScreen(SubWindow* subWindow) :
    mSubWindow(subWindow), mLoadingWindow(NULL)
{
}

LoadingScreen::~LoadingScreen()
{
    mLoadingWindow = NULL;
}

void LoadingScreen::setupScreen(sfg::Desktop& desktop)
{
    //sfg::Box is for layout purposes.
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    //Back button.
    auto spinner = sfg::Spinner::Create();
    spinner->Start();

    //Loading label.
    mLabel = sfg::Label::Create("Loading...");

    //Layout the widgets.
    box->Pack(spinner);
    box->Pack(mLabel);
    box->SetSpacing(10);

    //Put the box in a window.
    mLoadingWindow = sfg::Window::Create(0);
    mLoadingWindow->Add(box);

    //Add it to the desktop.
    desktop.Add(mLoadingWindow);
}

void LoadingScreen::updateScreen()
{
    static int lastWidth = 0;
    static int lastHeight = 0;
    if (mSubWindow->getWidth() != lastWidth || mSubWindow->getHeight() != lastHeight)
    {
        //Center the window.
        float width = mLoadingWindow->GetAllocation().width;
        float height = mLoadingWindow->GetAllocation().height;
        float winX = (mSubWindow->getWidth() - width) / 2;
        float winY = (mSubWindow->getHeight() - height) / 2;
        mLoadingWindow->SetAllocation({winX, winY, width, height});

        lastWidth = mSubWindow->getWidth();
        lastHeight = mSubWindow->getHeight();
    }
}
