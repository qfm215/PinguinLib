#include "Pinguin.h"

// constructor
Pixelarray::Pixelarray(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pixels = new sf::Uint8[width * height * 4];

    this->texture = new sf::Texture();
    this->texture->create(width, height);
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
    for (int i = 0; i < this->width * this->height; ++i)
    {
        ((t_color *)this->pixels)[i] = value;
    }
}

// fill all pixelarray with an unsigned int
void Pixelarray::fill(unsigned int value)
{
    for (int i = 0; i < this->width * this->height; ++i)
    {
        ((unsigned int *)this->pixels)[i] = value;
    }
}

// set color of a pixel at a 2d position in the array from t_color structure
void Pixelarray::setPixel(t_position pos, t_color value)
{
    ((t_color *)this->pixels)[pos.y * this->width + pos.x] = value;
}
void Pixelarray::setPixel(unsigned int x, unsigned int y, t_color value)
{
    ((t_color *)this->pixels)[y * this->width + x] = value;
}

// set color of a pixel at a 2d position in the array from unsigned int
void Pixelarray::setPixel(t_position pos, unsigned int value)
{
    ((unsigned int *)this->pixels)[pos.y * this->width + pos.x] = value;
}
void Pixelarray::setPixel(unsigned int x, unsigned int y, unsigned int value)
{
    ((unsigned int *)this->pixels)[y * this->width + x] = value;
}

// get the color of one pixel in the array
t_color Pixelarray::getPixel(t_position pos)
{
    return (((t_color *)this->pixels)[pos.y * this->width + pos.x]);
}
t_color Pixelarray::getPixel(unsigned int x, unsigned int y)
{
    return (((t_color *)this->pixels)[y * this->width + x]);
}

// call this function before pushing the pixelarray to the window
void Pixelarray::update()
{
    this->texture->update(this->pixels);
}