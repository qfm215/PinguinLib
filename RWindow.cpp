#include "Pinguin.h"

// constructor
Window::Window(int width, int height, const char *title)
{
//    XInitThreads();
    this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->width = width;
    this->height = height;
}

// destructor
Window::~Window()
{
    this->window->close();
    delete this->window;
}

// return true if the window is open or false if it is not
bool Window::isOpen() const
{
    return this->window->isOpen();
}

// close the render window
void Window::close()
{
    this->window->close();
}

// set the pixels of the window from the color values of a pixelarray (reference)
void Window::display(Pixelarray &pix)
{
    pix.update();
    this->window->clear();
    this->window->draw(*pix.sprite);
    this->window->display();
}

// set the pixels of the window from the color values of a pixelarray (pointer)
void Window::display(Pixelarray *pix)
{
    pix->update();
    this->window->clear();
    this->window->draw(*pix->sprite);
    this->window->display();
}