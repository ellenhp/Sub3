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

#include "ResizableImage.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

ResizableImage::ResizableImage(const sf::Image& image)
{
    mOriginalImage = image;
    SetImage(image);
}

ResizableImage::~ResizableImage()
{
}

ResizableImage::Ptr ResizableImage::Create(const sf::Image& image)
{
    return std::shared_ptr<ResizableImage>(new ResizableImage(image));
}

const std::string& ResizableImage::GetName() const
{
    static const std::string name("ResizableImage");
    return name;
}

void ResizableImage::SetImage(const sf::Image& image)
{
    mOriginalImage = image;
    Image::SetImage(mOriginalImage);
}

void ResizableImage::Resize(const unsigned int sizeX, const unsigned int sizeY)
{
    Resize(sf::Vector2u(sizeX, sizeY));
}

void ResizableImage::Resize(const sf::Vector2u& customSize)
{
    // If no size is given (0,0) , use original image...
    if(!customSize.x || !customSize.y)
    {
        Image::SetImage(mOriginalImage);
        return;
    }

    float originalX = static_cast<float>(mOriginalImage.getSize().x);
    float originalY = static_cast<float>(mOriginalImage.getSize().y);
    float desiredX = static_cast<float>(customSize.x);
    float desiredY = static_cast<float>(customSize.y);

    // Compute the scale we will apply to image
    float scaleX = desiredX / originalX;
    float scaleY = desiredY / originalY;

    // Get texture of the original image
    sf::Texture tempTexture;
    tempTexture.loadFromImage(mOriginalImage);

    // Sprite allows transformations to be made to it, so let's assign our texture to it
    //      (warn: sprite doesn't own the texture, keep it alive till we're done)
    sf::Sprite tempSprite;
    tempSprite.setTexture(tempTexture, true);

    tempSprite.scale(scaleX, scaleY);

    // Transformations on Sprite are set, so we can pre-render the sprite on
    //  a new texture with a transparent background
    sf::RenderTexture tempRenderTexture;
    tempRenderTexture.create(customSize.x, customSize.y);
    tempRenderTexture.setSmooth(true);
    tempRenderTexture.clear(sf::Color(255,255,255,0));
    tempRenderTexture.draw(tempSprite);
    tempRenderTexture.display();

    Image::SetImage(tempRenderTexture.getTexture().copyToImage());
}
