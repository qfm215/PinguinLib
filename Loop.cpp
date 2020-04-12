#include "Pinguin.h"

// constructor
Loop::Loop(Window *win, int fps, void *data, int (*callbackMain)(void *)) : win(win), fps(fps), data(data), callbackMain(callbackMain)
{
}

// main game loop
int Loop::mainLoop()
{
    sf::Event event;

    sf::Time timeBetweenFrame = sf::microseconds(1000000 / this->fps);
    sf::Time wait;

    int state = CONTINUE;
    sf::Clock clock;

    while (state == CONTINUE)
    {
        state = this->callbackMain(this->data);
        wait = timeBetweenFrame - clock.getElapsedTime();
        if (wait > sf::Time::Zero)
            sf::sleep(wait);
        clock.restart();
        while (this->win->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                state = EXIT_ON_SUCCESS;
            }
        }
    }
    this->win->close();
    return state;
}

// display loop calling at each frame a function created by the user
int Loop::run()
{
    int state = this->mainLoop();
    return (state);
}
