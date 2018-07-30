#include "Pinguin.hpp"

typedef struct s_data
{
    Window *win;
    Pixelarray *pix;
    int f;
} t_data;

int loop(void *data)
{
    t_data *d = (t_data*)data;

    t_color color;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    color.a = 255;
    for( int x = 0; x < 800; ++x)
    {
        for (int y = 0; y < 600; ++y)
        {
            if (x > d->f && x < d->f + 50)
                d->pix->setPixel(x, y, *color.value);
            else
                d->pix->setPixel(x, y, 0);
            if (d->f > 100 && x < 20 && y < 20)
                d->pix->setPixel(x, y, 0);
        }
    }

    d->pix->update();
    d->f = (d->f + 1) % (d->pix->width - 100);
    d->win->display(*d->pix);

    return CONTINUE;
}

int main()
{
    t_data d;
    d.win = new Window(800, 600, "Sfml Works !");

    d.pix = new Pixelarray(800, 600);

    d.f = 0;

    Loop l(d.win, 40, &d, &loop);
    int ret = l.run();

    return 0;
}