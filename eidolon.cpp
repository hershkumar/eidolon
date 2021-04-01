#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/Window.hpp>

// screen size
#define screen_height 256
#define screen_width 256
// map size
#define map_width 10
#define map_height 10

// defines just a box map
int map[map_width][map_height] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int main(){
    // make the window object
    sf::Window window;
    // set up the window with the screen size and the title
    window.create(sf::VideoMode(screen_width, screen_height), "Eidolon");
    

    return 0;
}