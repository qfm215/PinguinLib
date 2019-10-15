#include "Pinguin.h"

// constructor
Loop::Loop(Window *win, int fps, void *data, int (*callbackMain)(void *), int (*callbackKey)(void *, const bool *)) : win(win), fps(fps), data(data), callbackMain(callbackMain), callbackKey(callbackKey)
{
}

// get the state of keyboard inputs and store it into array
void Loop::fillKeys(bool *keys)
{
    for (int k = 0; k <= sf::Keyboard::Return; ++k)
    {
        keys[k] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)k);
    }
}


// display loop calling at each frame a function created by the user
int Loop::run()
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
