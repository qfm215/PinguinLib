#ifndef PINGUIN_HPP_
# define PINGUIN_HPP_

# include <iostream>
# include <cstring>
# include <SFML/Graphics.hpp>

# define CONTINUE 0
# define EXIT_ON_SUCCESS 1
# define EXIT_ON_ERROR 2

# define BLACK 0xFF000000
# define WHITE 0xFFFFFFFF

// 2d position structure
typedef struct s_position
{
    unsigned int x;
    unsigned int y;
} t_position;

// color structure with red, green, blue and alpha properties
typedef struct s_color
{
    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    sf::Uint8 a;
} t_color;

// class creating an array of uint32 with each element of the array representing the color of one pixel on the final window
class Pixelarray
{
public:
    sf::Uint8 *pixels;
    unsigned int width;
    unsigned int height;
    sf::Texture *texture;
    sf::Sprite *sprite;

    // constructors
    Pixelarray(int width, int height);
    Pixelarray(const std::string filename);

    // destructor
    ~Pixelarray();

    // fill all pixelarray with a t_color
    void fill(t_color value);

    // fill all pixelarray with a unsigned int
    void fill(unsigned int value);

    // fill pixelarray with another pixelarray starting from the upper left corner
    void fill(Pixelarray *pix);

    // set color of a pixel at a 2d position in the array from t_color structure
    void setPixel(t_position pos, t_color value);
    void setPixel(unsigned int x, unsigned int y, t_color value);

    // set color of a pixel at a 2d position in the array from unsigned int
    void setPixel(t_position pos, unsigned int value);
    void setPixel(unsigned int x, unsigned int y, unsigned int value);

    // get the color of one pixel in the array
    unsigned int getPixel(t_position pos);
    unsigned int getPixel(unsigned int x, unsigned int y);

    // call this function before pushing the pixelarray to the window
    void update();
};

// class creating a sfml window
class Window
{
public:
    sf::RenderWindow *window;
    unsigned int width;
    unsigned int height;

    // constructor
    Window(int width, int height, const char *title);

    // destructor
    ~Window();

    // return true if the window is open or false if it is not
    bool isOpen() const;

    // close the render window
    void close();

    // set the pixels of the window from the color values of a pixelarray (reference)
    void display(Pixelarray &pix);

    // set the pixels of the window from the color values of a pixelarray (pointer)
    void display(Pixelarray *pix);
};

// class running a display loop updating a window at a specific framerate
class Loop
{
private:
    Window *win;
    int fps;
    void *data;
    int (*callbackMain)(void *);
    int (*callbackKey)(void *, const bool *keys);

    // main loop function
    int mainLoop();

public:
    // constructor
    Loop(Window *win, int fps, void *data, int (*callbackMain)(void *));

    // destructor
    ~Loop() = default;

    // display loop calling at each frame a function created by the user
    int run();
};

#endif
