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

#include "WaterfallDisplay.hpp"

#include <SFML/Graphics/Image.hpp>

#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveTexture.hpp>

#include <boost/assert.hpp>

const std::string WaterfallDisplay::name = "Waterfall Display";

WaterfallDisplay::WaterfallDisplay(float timeResolution, float secondsOfHistory, sf::Color color) :
    mStateSet(false), mSecondsSinceHistoryUpdate(0.f), mTimeResolution(timeResolution), mBuckets(0), mColor(color), mImageBuffer(NULL)
{
    mRows = secondsOfHistory / timeResolution;
    sf::Image img;
    img.create(1, 1);
    SetImage(img);
}

WaterfallDisplay::~WaterfallDisplay()
{
    if (mImageBuffer)
    {
        delete mImageBuffer;
    }
}

void WaterfallDisplay::SetState(BroadbandState state)
{
    mStateSet = true;
    mState = state;
}

WaterfallDisplay::Ptr WaterfallDisplay::Create(float timeResolution, float secondsOfHistory, sf::Color color)
{
    return std::shared_ptr<WaterfallDisplay>(new WaterfallDisplay(timeResolution, secondsOfHistory, color));
}

void WaterfallDisplay::HandleUpdate(float seconds)
{
    mSecondsSinceHistoryUpdate += seconds;

    if (!mStateSet)
    {
        return;
    }

    //Get the row from the BroadbandState.
    auto row = mState.getRow();
    if (mBuckets == 0)
    {
        mBuckets = row.size();
    }

    //Make sure it's the correct size.
    BOOST_ASSERT_MSG(mBuckets == row.size(), "Fatal: WaterfallDisplay found row size mismatch");

    //Keep track of whether we've updated history so we can know whether to invalidate or not.
    bool updatedHistory = false;

    //While we can do a full history update.
    while (mSecondsSinceHistoryUpdate - mTimeResolution > 0)
    {
        //Subtract off the time since the last update.
        mSecondsSinceHistoryUpdate -= mTimeResolution;

        //push_front the row into the history.
        mHistory.push_front(row);

        while (mHistory.size() > mRows)
        {
            mHistory.pop_back();
        }

        //Make sure to call Invalidate() at the end.
        updatedHistory = true;
    }

    //This is here so we don't call Invalidate multiple times.
    if (updatedHistory)
    {
        GenerateImage();

        sf::Image img;
        img.create(mBuckets, mRows, mImageBuffer);
        SetImage(img);
        Resize(GetAllocation().width, GetAllocation().height);
    }
}

void WaterfallDisplay::GenerateImage()
{
    //SFML uses 32-bit RGBA internally.
    const int pixelSize = 4;

    //This is the number of pixels in the image.
    const int numPixels = mRows * mBuckets;

    //Make sure we know how big the image is supposed to be.
    if (!mStateSet)
    {
        //If this is true, we don't know what to do in this function at all.
        return;
    }

    //Make sure we have an image buffer to work with.
    if (mImageBuffer == NULL)
    {
        mImageBuffer = new sf::Uint8[numPixels * pixelSize];
    }

    //Boilerplate double for loop.
    for (int row = 0; row < mRows; row++)
    {
        for (int bucket = 0; bucket < mBuckets; bucket++)
        {
            //Row-major order.
            int pixelIndex = row * mBuckets + bucket;
            int pixelOffset = pixelIndex * pixelSize;

            //If we have data on this row.
            if (row < mHistory.size())
            {
                auto& rowDeque = mHistory.at(row);
                float intensity = rowDeque.at(bucket);
                //Fill in the data.
                mImageBuffer[pixelOffset + 0] = (sf::Uint8)(mColor.r * intensity);
                mImageBuffer[pixelOffset + 1] = (sf::Uint8)(mColor.g * intensity);
                mImageBuffer[pixelOffset + 2] = (sf::Uint8)(mColor.b * intensity);
                mImageBuffer[pixelOffset + 3] = (sf::Uint8)(mColor.a * intensity);
            }
            else
            {
                mImageBuffer[pixelOffset + 0] = 100;
                mImageBuffer[pixelOffset + 1] = 0;
                mImageBuffer[pixelOffset + 2] = 0;
                mImageBuffer[pixelOffset + 3] = 255;
            }
            BOOST_ASSERT_MSG(pixelIndex < numPixels, "Fatal: GenerateImage mess-up");
        }
    }
}
