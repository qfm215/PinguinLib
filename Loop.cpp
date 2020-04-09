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
void Loop::keyLoop(int *ret, bool *end)
{
    bool *keys = new bool[sf::Keyboard::Return + 1];
    int key_ret = *ret;
    sf::Time wait = sf::milliseconds(100);

    while (!(*end))
    {
        this->fillKeys(keys);

        mtx.lock();
        key_ret = this->callbackKey(data, keys);
        mtx.unlock();
        if (key_ret != CONTINUE)
        {
            mtx.lock();
            *ret = key_ret;
            mtx.unlock();
            break;
        }
        sf::sleep(wait);
    }
    delete keys;
    mtx.lock();
    *end = true;
    mtx.unlock();
}

// main game loop
void Loop::mainLoop(int *ret, bool *end)
{
    sf::Clock clock;
    sf::Event event;
    int main_ret = *ret;
    sf::Time timeBetweenFrame = sf::microseconds(1000000 / this->fps);
    sf::Time wait;

    while (!(*end) && this->win->window->isOpen())
    {
        while (this->win->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->win->window->close();
        }
        mtx.lock();
        main_ret = this->callbackMain(data);
        mtx.unlock();
        if (main_ret != CONTINUE)
        {
            mtx.lock();
            *ret = main_ret;
            mtx.unlock();
            break;
        }
        wait = timeBetweenFrame - clock.getElapsedTime();
        if (wait > sf::Time::Zero)
            sf::sleep(wait);
        clock.restart();
    }
    mtx.lock();
    *end = true;
    mtx.unlock();
}

// display loop calling at each frame a function created by the user
int Loop::run()
{
    int ret = CONTINUE;
    bool end = false;

    std::thread keyThread(&Loop::keyLoop, this, &ret, &end);
    std::thread mainThread(&Loop::mainLoop, this, &ret, &end);

    mainThread.join();
    keyThread.join();

    return (ret);
}
