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
    void SetImage( const sf::Image& image );

    //Getters.
    const sf::Image& GetImage() const;
    const sf::Image& GetResizedImage() const;

    void Resize(const unsigned int sizeX, const unsigned int sizeY);

    void Resize(const sf::Vector2u& customSize = sf::Vector2u(0, 0));

    void SetKeepAspect(bool value);

    bool GetKeepAspect() const;

protected:
    ResizableImage(const sf::Image& image = sf::Image());

private:
    sf::Image mOriginalImage;
    bool mKeepAspect = true;

};
