#pragma once

#include <SFGUI/Desktop.hpp>

//This abstract class represents a screen that a SubWindow can set up for a user.
class SubScreen
{
public:
    virtual void setupScreen(sfg::Desktop& desktop) = 0;

};
