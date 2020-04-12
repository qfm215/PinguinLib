#include "Pinguin.h"

// constructor
RWindow::RWindow(int width, int height, const char *title)
{
    XInitThreads();
    this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->width = width;
    this->height = height;
}

// destructor
RWindow::~RWindow()
{
    this->window->close();
    delete this->window;
}

// return true if the window is open or false if it is not
bool RWindow::isOpen() const
{
    return this->window->isOpen();
}

// close the render window
void RWindow::close()
{
    this->window->close();
}

// set the pixels of the window from the color values of a pixelarray (reference)
void RWindow::display(Pixelarray &pix)
{
    std::thread t(&Pixelarray::update, &pix);
    this->window->clear();
    t.join();
    this->window->draw(*pix.sprite);
    this->window->display();
}

// set the pixels of the window from the color values of a pixelarray (pointer)
void RWindow::display(Pixelarray *pix)
{
    std::thread t(&Pixelarray::update, pix);
    this->window->clear();
    t.join();
    this->window->draw(*pix->sprite);
    this->window->display();
}