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
    sf::Time wait = sf::milliseconds(100);

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
            mtx.unlock();
            break;
        }
        sf::sleep(wait);
    }
    delete keys;
    mtx.lock();
    this->win->window->close();
    mtx.unlock();
}

// event game loop
void Loop::eventLoop()
{
    sf::Event event;
    sf::Time wait = sf::milliseconds(100);

    while (this->win->window->isOpen())
    {
        while (this->win->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mtx.lock();
                this->win->window->close();
                mtx.unlock();
            }
        }
        sf::sleep(wait);
    }
}

// main game loop
void Loop::mainLoop(int *state)
{
    sf::Clock clock;
    int main_ret = *state;
    sf::Time timeBetweenFrame = sf::microseconds(1000000 / this->fps);
    sf::Time wait;

    while (this->win->window->isOpen())
    {
        mtx.lock();
        main_ret = this->callbackMain(this->data);
        mtx.unlock();
        if (main_ret != CONTINUE)
        {
            mtx.lock();
            *state = main_ret;
            mtx.unlock();
            break;
        }
        wait = timeBetweenFrame - clock.getElapsedTime();
        if (wait > sf::Time::Zero)
            sf::sleep(wait);
        clock.restart();
    }
    mtx.lock();
    this->win->window->close();
    mtx.unlock();
}

// display loop calling at each frame a function created by the user
int Loop::run()
{
    int state = CONTINUE;

    std::thread keyThread(&Loop::keyLoop, this, &state);
    std::thread eventThread(&Loop::eventLoop, this);
    std::thread mainThread(&Loop::mainLoop, this, &state);

    keyThread.join();
    eventThread.join();
    mainThread.join();

    return (state);
}
