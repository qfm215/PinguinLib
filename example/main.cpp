#include "Pinguin/Pinguin.h"

typedef struct s_data
{
    Window *win;
    Pixelarray *pix;
    Pixelarray *bg;
    int f;
} t_data;

int key(t_data *data)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        printf("key A pressed\n");
    return (CONTINUE);
}

int loop(void *data)
{
    t_data *d = (t_data*)data;

    key(d);

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
    d.win = new Window(1920, 1080, "Sfml Works !");

    d.bg = new Pixelarray("test.jpg");

    d.pix = new Pixelarray(1920, 1080);
    d.f = 0;

    Loop l(d.win, 30, &d, &loop);
    int ret = l.run();

    return 0;
}
