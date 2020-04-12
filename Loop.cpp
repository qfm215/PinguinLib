#include "Pinguin.h"

std::mutex mtx;

// constructor
Loop::Loop(RWindow *win, int fps, void *data, int (*callbackMain)(void *), int (*callbackKey)(void *, const bool *)) : win(win), fps(fps), data(data), callbackMain(callbackMain), callbackKey(callbackKey)
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

// game loop handling the keyboard entries
void Loop::keyLoop(int *state)
{
    bool *keys = new bool[sf::Keyboard::Return + 1];
    int key_ret = *state;
    sf::Time wait = sf::milliseconds(10);

    while (this->win->window->isOpen())
    {
        this->fillKeys(keys);
        mtx.lock();
        key_ret = this->callbackKey(this->data, keys);
        mtx.unlock();
        if (key_ret != CONTINUE)
        {
            mtx.lock();
            *state = key_ret;
            this->win->window->close();
            mtx.unlock();
            break;
        }
        sf::sleep(wait);
    }
    delete keys;
}

// main game loop
void Loop::mainLoop(int *state)
{
    sf::Event event;
    int main_ret = *state;
    sf::Time timeBetweenFrame = sf::microseconds(1000000 / this->fps);
    sf::Time wait;
    sf::Clock clock;

    while (this->win->window->isOpen())
    {
        while (this->win->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                this->win->window->close();
            }
        }
        mtx.lock();
        main_ret = this->callbackMain(this->data);
        mtx.unlock();
        if (main_ret != CONTINUE)
        {
            mtx.lock();
            *state = main_ret;
            this->win->window->close();
            mtx.unlock();
            break;
        }
        wait = timeBetweenFrame - clock.getElapsedTime();
        if (wait > sf::Time::Zero)
            sf::sleep(wait);
        clock.restart();
    }
}

// display loop calling at each frame a function created by the user
int Loop::run()
{
    int state = CONTINUE;

    std::thread keyThread(&Loop::keyLoop, this, &state);
    std::thread mainThread(&Loop::mainLoop, this, &state);

    keyThread.join();
    mainThread.join();

    return (state);
}
