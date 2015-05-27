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

#include <SFML/Graphics/Image.hpp>

#include <memory>

class ResizableImage : public sfg::Image
{
public:
    typedef std::shared_ptr<ResizableImage> Ptr;
    typedef std::shared_ptr<const ResizableImage> PtrConst;

    static Ptr Create(const sf::Image& image = sf::Image());

    //Need to override this.
    virtual const std::string& GetName() const;

    virtual ~ResizableImage();

    //Override set function.
    void SetImage(const sf::Image& image);

    void Resize(const unsigned int sizeX, const unsigned int sizeY);
    void Resize(const sf::Vector2u& customSize = sf::Vector2u(0, 0));

protected:
    ResizableImage(const sf::Image& image = sf::Image());

private:
    sf::Image mOriginalImage;

};
