#include "Pinguin/Pinguin.h"

typedef struct s_data
{
    RWindow *win;
    Pixelarray *pix;
    int f;
} t_data;

int key(void *data, const bool *keys)
{
    if (keys[sf::Keyboard::A])
        printf("key A pressed\n");
    return (CONTINUE);
}

int loop(void *data)
{
    t_data *d = (t_data*)data;

    t_color color;
    color.r = 0;
    color.g = 255;
    color.b = 0;
    color.a = 255;
    t_position pos;

    d->pix->fill(BLACK);

    pos.x = 0;
    while (pos.x < 800)
    {
        pos.y = 0;
        while (pos.y < 600)
        {
            if (pos.x > d->f && pos.x < d->f + 50)
                d->pix->setPixel(pos, color);
            pos.y += 1;
        }
        pos.x += 1;
    }

    d->f = (d->f + 1) % (d->pix->width - 100);
    d->win->display(*d->pix);

    return CONTINUE;
}

int main()
{
    t_data d;
    d.win = new RWindow(800, 600, "Sfml Works !");

    d.pix = new Pixelarray(800, 600);

    d.f = 0;

    Loop l(d.win, 40, &d, &loop, &key);
    int ret = l.run();

    return 0;
}
