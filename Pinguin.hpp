#ifndef PINGUIN_HPP_
# define PINGUIN_HPP_

# include <iostream>
# include <thread>
# include <mutex>
# include <SFML/Graphics.hpp>

# define CONTINUE 0
# define EXIT_ON_SUCCESS 1
# define EXIT_ON_ERROR 2

// 2d position structure
typedef struct s_position
{
    unsigned int x;
    unsigned int y;
} t_position;

// color structure with red, green, blue and alpha properties
typedef struct s_color
{
    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    sf::Uint8 a;
} t_color;

// class creating an array of uint32 with each element of the array representing the color of one pixel on the final window
class Pixelarray
{
public:
    sf::Uint8 *pixels;
    unsigned int width;
    unsigned int height;
    sf::Texture *texture;
    sf::Sprite *sprite;

    // constructor
    Pixelarray(int width, int height)
    {
        this->width = width;
        this->height = height;
        this->pixels = new sf::Uint8[width * height * 4];

        this->texture = new sf::Texture();
        this->texture->create(width, height);
        this->sprite = new sf::Sprite(*this->texture);
    }

    // destructor
    ~Pixelarray()
    {
        delete this->pixels;
        delete this->texture;
        delete this->sprite;
    }

    // set color of a pixel at a 2d position in the array from t_color structure
    void setPixel(t_position pos, t_color value)
    {
        ((t_color *)this->pixels)[pos.y * this->width + pos.x] = value;
    }

    // set color of a pixel at a 2d position in the array from unsigned int
    void setPixel(t_position pos, unsigned int value)
    {
        ((unsigned int *)this->pixels)[pos.y * this->width + pos.x] = value;
    }

    // get the color of one pixel in the array
    t_color getPixel(t_position pos)
    {
        return (((t_color *)this->pixels)[pos.y * this->width + pos.x]);
    }

    // call this function before pushing the pixelarray to the window
    void update()
    {
        this->texture->update(this->pixels);
    }
};

// class creating a sfml window
class Window
{
public:
    sf::RenderWindow *window;
    unsigned int width;
    unsigned int height;

    // constructor
    Window(int width, int height, const char *title)
    {
        this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
        this->width = width;
        this->height = height;
    }

    // destructor
    ~Window()
    {
        this->window->close();
        delete this->window;
    }

    // return true if the window is open or false if it is not
    bool isOpen() const
    {
        return this->window->isOpen();
    }

    // set the pixels of the window from the color values of a pixelarray (reference)
    void display(Pixelarray &pix)
    {
        std::thread t(&Pixelarray::update, &pix);
        this->window->clear();
        t.join();
        this->window->draw(*pix.sprite);
        this->window->display();
    }

    // set the pixels of the window from the color values of a pixelarray (pointer)
    void display(Pixelarray *pix)
    {
        std::thread t(&Pixelarray::update, pix);
        this->window->clear();
        t.join();
        this->window->draw(*pix->sprite);
        this->window->display();
    }
};

// class running a display loop updating a window at a specific framerate
class Loop
{
private:
    Window *win;
    int fps;
    void *data;
    int (*callbackMain)(void *);
    int (*callbackKey)(void *, const bool *keys);
    
    // get the state of keyboard inputs and store it into array
    void fillKeys(bool *keys)
    {
        for (int k = 0; k <= sf::Keyboard::Return; ++k)
        {
            keys[k] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)k);
        }
    }

public:
    // constructor
    Loop(Window *win, int fps, void *data, int (*callbackMain)(void *), int (*callbackKey)(void *, const bool *)) : win(win), fps(fps), data(data), callbackMain(callbackMain), callbackKey(callbackKey)
    {
    }

    // destructor
    ~Loop() = default;

    // display loop calling at each frame a function created by the user
    int run()
    {
        sf::Event event;
        bool *keys = new bool[sf::Keyboard::Return + 1];
        int ret = CONTINUE;

        sf::Time timeBetweenFrame = sf::microseconds(1000000 / this->fps);
        sf::Time wait;
        sf::Clock clock;

        while (ret == CONTINUE && this->win->window->isOpen())
        {
            while (this->win->window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    this->win->window->close();
            }
            std::thread t1(&Loop::fillKeys, this, keys);
            ret = this->callbackMain(data);
            t1.join();
            ret += this->callbackKey(data, keys);

            wait = timeBetweenFrame - clock.getElapsedTime();
            if (wait > sf::Time::Zero)
                sf::sleep(wait);
            clock.restart();
        }
        delete keys;

        return (ret);
    }
};

#endif
