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

const sf::Image& ResizableImage::GetImage() const
{
    return mOriginalImage;
}

const sf::Image& ResizableImage::GetResizedImage() const
{
    return Image::GetImage();
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

    if(mKeepAspect)
    {
        // Use same scale for both sides of the sprite
        float lowerScale = std::min(scaleX, scaleY);

        tempSprite.scale(lowerScale, lowerScale);

        //Set offsets.
        float offsetX = (desiredX - (originalX * (lowerScale))) / 2;
        float offsetY = (desiredY - (originalY * (lowerScale))) / 2;

        tempSprite.move(offsetX, offsetY);
    }
    else
    {
        tempSprite.scale(scaleX, scaleY);
    }

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

void ResizableImage::SetKeepAspect(bool value)
{
    mKeepAspect = value;
}

bool ResizableImage::GetKeepAspect() const
{
    return mKeepAspect;
}
