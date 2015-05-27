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

#pragma once

#include <SFGUI/Image.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Config.hpp>

#include <memory>
#include <deque>
#include <vector>

#include "Sub3.hpp"
#include "simulation/BroadbandState.hpp"
#include "graphics/widgets/ResizableImage.hpp"

//This class is a custom SFGUI Widget to display broadband sonar data.
class WaterfallDisplay : public ResizableImage
{
public:
    //Trying to make it behave like a sfg::Widget as much as possible, even where it's unnecessary.
    typedef std::shared_ptr<WaterfallDisplay> Ptr;
    typedef std::shared_ptr<const WaterfallDisplay> PtrConst;

    virtual ~WaterfallDisplay();

    void SetState(BroadbandState state);

    //angularResolution is in degrees, noise is from 0 to 1.
    static Ptr Create(float timeResolution = 0.1f, float secondsOfHistory = 30.f, sf::Color color = sf::Color::Green);

protected:
    WaterfallDisplay(float timeResolution, float secondsOfHistory, sf::Color color);

    //Move the history back.
    virtual void HandleUpdate(float seconds);

    //Generate the waterfall display image.
    void GenerateImage();

    //This holds n rows of history.
    std::deque<std::vector<float>> mHistory;

    BroadbandState mState;
    bool mStateSet;

    //How much time has passed since we purged old history data?
    float mSecondsSinceHistoryUpdate;
    float mTimeResolution;

    int mRows;
    int mBuckets;

    sf::Color mColor;

    sf::Uint8* mImageBuffer;

    static const std::string name;
};
