#pragma once

#include <SFGUI/Desktop.hpp>

#include <string>
#include <vector>

//This abstract class represents a screen that a SubWindow can set up for a user.
class SubScreen
{
public:
    virtual ~SubScreen();

    virtual void setupScreen(sfg::Desktop& desktop, std::vector<std::string> args) = 0;

    virtual void updateScreen();
};
