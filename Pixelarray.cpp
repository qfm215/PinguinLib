#include "Pinguin.h"

// constructors
Pixelarray::Pixelarray(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pixels = new sf::Uint8[width * height * 4];

    this->texture = new sf::Texture();
    this->texture->create(width, height);
    this->sprite = new sf::Sprite(*this->texture);
}
Pixelarray::Pixelarray(const std::string filename)
{
    sf::Image image;
    image.loadFromFile(filename);
    this->width = image.getSize().x;
    this->height = image.getSize().y;
    
    unsigned int total_size = this->width * this->height * 4;
    this->pixels = new sf::Uint8[total_size];
    std::memcpy(this->pixels, image.getPixelsPtr(), total_size);

    this->texture = new sf::Texture();
    this->texture->create(this->width, this->height);
    this->sprite = new sf::Sprite(*this->texture);
}

// destructor
Pixelarray::~Pixelarray()
{
    delete this->pixels;
    delete this->texture;
    delete this->sprite;
}

// fill all pixelarray with a t_color
void Pixelarray::fill(t_color value)
{
    for (unsigned int i = 0; i < this->width * this->height; ++i)
    {
        ((t_color *)this->pixels)[i] = value;
    }
}

// fill all pixelarray with an unsigned int
void Pixelarray::fill(unsigned int value)
{
    for (unsigned int i = 0; i < this->width * this->height; ++i)
    {
        ((unsigned int *)this->pixels)[i] = value;
    }
}

// fill pixelarray with another pixelarray starting from the upper left corner
void Pixelarray::fill(Pixelarray *pix)
{
    for (unsigned int x = 0; x < this->width; ++x)
    {
        for (unsigned int y = 0; y < this->height; ++y)
        {
            this->setPixel(x, y, pix->getPixel(x, y));
        }
    }
}

// set color of a pixel at a 2d position in the array from t_color structure
void Pixelarray::setPixel(t_position pos, t_color value)
{
    if (pos.x < this->width && pos.y < this->height)
        ((t_color *)this->pixels)[pos.y * this->width + pos.x] = value;
}
void Pixelarray::setPixel(unsigned int x, unsigned int y, t_color value)
{
    if (x < this->width && y < this->height)
        ((t_color *)this->pixels)[y * this->width + x] = value;
}

// set color of a pixel at a 2d position in the array from unsigned int
void Pixelarray::setPixel(t_position pos, unsigned int value)
{
    if (pos.x < this->width && pos.y < this->height)
        ((unsigned int *)this->pixels)[pos.y * this->width + pos.x] = value;
}
void Pixelarray::setPixel(unsigned int x, unsigned int y, unsigned int value)
{
    if (x < this->width && y < this->height)
        ((unsigned int *)this->pixels)[y * this->width + x] = value;
}

// get the color of one pixel in the array
unsigned int Pixelarray::getPixel(t_position pos)
{
    if (pos.x < this->width && pos.y < this->height)
        return (((unsigned int *)this->pixels)[pos.y * this->width + pos.x]);
    else
        return BLACK;
}
unsigned int Pixelarray::getPixel(unsigned int x, unsigned int y)
{
    if (x < this->width && y < this->height)
        return (((unsigned int *)this->pixels)[y * this->width + x]);
    else
        return BLACK;
}

// call this function before pushing the pixelarray to the window
void Pixelarray::update()
{
    this->texture->update(this->pixels);
}