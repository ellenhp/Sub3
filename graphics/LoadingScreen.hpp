#pragma once

#include "graphics/SubScreen.hpp"
#include "Sub3.hpp"

#include <SFGUI/Widgets.hpp>

#include <thread>
#include <mutex>

#include <usml/ocean/ocean_shared.h>

class SubWindow;
class SubSocket;

//This class opens a connection and initializes USML
class LoadingScreen : public SubScreen
{
public:
    LoadingScreen(SubWindow* subWindow);
    virtual ~LoadingScreen();

    void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args);

    void updateScreen(float dt);

private:
    SubWindow* mSubWindow;

    //Stuff required to load and connect asynchronously.
    std::unique_ptr<std::thread> mLoadingThread;
    std::mutex mLoadingMutex;
    bool mLoadingDone;
    bool mLaunchGame;

    std::shared_ptr<SubSocket> mGameSocket;
    std::shared_ptr<usml::ocean::ocean_shared> mGameOcean;

    std::string mLoadingText;

    sfg::Spinner::Ptr mSpinner;
    sfg::Label::Ptr mLabel;
    sfg::Window::Ptr mLoadingWindow;

    std::string mIpAddress;
    uint16_t mPortNumber;

    void centerWindow();

    void doLoading();


};
