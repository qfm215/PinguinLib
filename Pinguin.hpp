#ifndef PINGUIN_HPP_
# define PINGUIN_HPP_

# include <iostream>
# include <ctime>
# include <unistd.h>
# include <thread>
# include <mutex>
# include <SFML/Graphics.hpp>

# define CONTINUE 0
# define EXIT_ON_SUCCESS 1
# define EXIT_ON_ERROR 2

typedef struct s_position
{
    int x;
    int y;
} t_position;

typedef struct s_color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    unsigned int *value = (unsigned int *)&r;
} t_color;

class Pixelarray
{
public:
    sf::Uint8 *pixels;
    unsigned int width;
    unsigned int height;
    sf::Texture *texture;
    sf::Sprite *sprite;

    Pixelarray(int width, int height)
    {
        this->width = width;
        this->height = width;
        this->pixels = new sf::Uint8[width * height * 4];

        this->texture = new sf::Texture();
        this->texture->create(width, height);
        this->sprite = new sf::Sprite(*this->texture);
    }

    ~Pixelarray()
    {
        delete this->pixels;
        delete this->sprite;
        delete this->texture;
    }

    void setPixel(t_position pos, unsigned int value)
    {
        ((unsigned int *)this->pixels)[pos.y * this->width + pos.x] = value;
    }

    unsigned int getPixel(t_position pos)
    {
        return (((unsigned int *)this->pixels)[pos.y * this->width + pos.x]);
    }

    void update()
    {
        this->texture->update(this->pixels);
    }
};

class Window
{
public:
    sf::RenderWindow *window;
    unsigned int width;
    unsigned int height;

    Window(int width, int height, const char *title)
    {
        this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
        this->width = width;
        this->height = height;
    }

    ~Window()
    {
        this->window->close();
        delete this->window;
    }

    bool isOpen() const
    {
        return (this->window->isOpen());
    }

    void getEvent()
    {

    }
    
    void display(Pixelarray &pix)
    {
        this->window->clear();
        this->window->draw(*pix.sprite);
        this->window->display();
    }
};

class Loop
{
private:
    Window *win;
    int fps;
    void *data;
    int (*callbackMain)(void *);
    int (*callbackKey)(void *, const bool *keys);

    void fillKeys(bool *keys)
    {
        for (int k = 0; k <= sf::Keyboard::Return; ++k)
        {
            keys[k] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)k);
        }
    }

public:
    Loop(Window *win, int fps, void *data, int (*callbackMain)(void *), int (*callbackKey)(void *, const bool *)) : win(win), fps(fps), data(data), callbackMain(callbackMain), callbackKey(callbackKey)
    {
    }

    ~Loop() = default;

    int run()
    {
        int wait;
        long double t = time(0) * 1000000;
        sf::Event event;
        bool keys[sf::Keyboard::Return + 1];
        int ret = CONTINUE;

        while (ret == CONTINUE && this->win->isOpen())
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
            wait = 1000000 / fps + t - (t = time(0) * 1000000);
            if (wait > 0)
                usleep(wait);
        }

        return (ret);
    }
};

#endif