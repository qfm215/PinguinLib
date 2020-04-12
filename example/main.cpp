#include "Pinguin/Pinguin.h"

typedef struct s_data
{
    RWindow *win;
    Pixelarray *pix;
    Pixelarray *bg;
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

    d->pix->fill(d->bg);

    for (int x = 0; x < d->pix->width; ++x)
    {
        for (int y = 0; y < d->pix->height; ++y)
        {
            if (x > d->f && x < d->f + 50 && d->pix->getPixel(x, y) == BLACK)
                d->pix->setPixel(x, y, color);
        }
    }

    d->f = (d->f + 5) % (d->pix->width - 100);
    d->win->display(d->pix);

    return CONTINUE;
}

int main()
{
    t_data d;
    d.win = new RWindow(1920, 1080, "Sfml Works !");

    d.bg = new Pixelarray("test.jpg");

    d.pix = new Pixelarray(1920, 1080);
    d.f = 0;

    Loop l(d.win, 30, &d, &loop, &key);
    int ret = l.run();

    return 0;
}
