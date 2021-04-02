#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// screen size
#define screen_height 200
#define screen_width 200
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
    sf::RenderWindow window;
    // set up the window with the screen size and the title
    window.create(sf::VideoMode(screen_width, screen_height), "Eidolon");
    
    window.setVerticalSyncEnabled(true);
    
    //character position
    double x_pos = 5, y_pos = 5;
    // initial direction vector
    double x_dir = -1, y_dir = 0;
    // camera plane
    double x_plane = 0, y_plane = .66;

    // current time
    double time = 0;
    // last frame time
    double last_frame_time = 0;

    //event loop
    while (window.isOpen()){
        // get the events that happened
        sf::Event event;
        while (window.pollEvent(event)){
            sf::Clock clock;
            // if they tried to close the window
            if (event.type == sf::Event::Closed){
                window.close();
            }
            //clear the window
            window.clear();
            // the actual raycasting algorithm
            for (int x = 0; x < screen_width; x++){
                // calc the ray position and direction
                double camera_x = 2* x/ (double) screen_width - 1;
                double ray_dir_x = x_dir + x_plane * camera_x;
                double ray_dir_y = y_dir + y_plane * camera_x;

                // get the current square that the ray is in
                int map_x = int(x_pos);
                int map_y = int(y_pos);
                // dist of the ray from the next x or y box side
                double side_dist_x, side_dist_y;

                // dist from ray from one x/y side to next x/y side
                double del_dist_x = std::abs(1 / ray_dir_x);
                double del_dist_y = std::abs(1 / ray_dir_y);
                double perp_wall_dist;

                // which direction to step forward
                int step_x, step_y;

                bool hit = false;
                bool side;

                if (ray_dir_x < 0){
                    step_x = -1;
                    side_dist_x = (x_pos - map_x) * del_dist_x;
                }
                else {
                    step_x = 1;
                    side_dist_x = (map_x + 1.0 - x_pos) * del_dist_x;
                }

                if (ray_dir_y < 0){
                    step_y = -1;
                    side_dist_y = (y_pos - map_y) * del_dist_y;
                }
                else{
                    step_y = 1;
                    side_dist_y = (map_y + 1.0 - y_pos) * del_dist_y;
                }
                // while the ray hasn't hit a wall
                while (!hit){
                    if (side_dist_x < side_dist_y){
                        side_dist_x += del_dist_x;
                        map_x += step_x;
                        side = 0;
                    }
                    else{
                        side_dist_y += del_dist_y;
                        map_y += step_y;
                        side = 1;
                    }
                    // check if we hit a wall
                    if (map[map_x][map_y] > 0){
                        hit = true;
                    }
                }

                if (!side){
                    perp_wall_dist = (map_x - x_pos + (1 - step_x) / 2) / ray_dir_x;
                }
                else{
                    perp_wall_dist = (map_y - y_pos  + (1 - step_y) / 2) / ray_dir_y;
                }
                
                // get the height of the line that we need to draw
                int line_height = (int) (screen_height / perp_wall_dist);

                // get the lowest and highest pixel to fill
                int draw_start = - line_height / 2 + screen_height / 2;
                if (draw_start < 0){
                    draw_start = 0;
                }
                int draw_stop = line_height / 2 + screen_height / 2;
                if (draw_stop >= screen_height){
                    draw_stop = screen_height - 1;
                }
                // get the color that we need to draw
                int r,g,b;
                r = 100;
                g = 0;
                b = 0;
                
                if (side){
                    r /= 2;
                    g /= 2;
                    b /= 2;
                }
                sf::Color color = sf::Color(r, g, b);
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(x, draw_start)),
                    sf::Vertex(sf::Vector2f(x, draw_stop))
                };
                line[0].color = color;
                line[1].color = color;
                window.draw(line, 2, sf::Lines);
            }
            
            
            time = clock.getElapsedTime().asMilliseconds();
            clock.restart();
            double frame_time = (time) / 1000.0;
            
            double fps = 1.0 / frame_time;
            printf("fps: %f\n", fps);

            double mov_speed = frame_time * 5.0;
            double rot_speed = frame_time * 3.0;

            // key input
            // if they press w
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                if (map[int(x_pos + x_dir * mov_speed)][int(y_pos)] == false){
                    x_pos += x_dir * mov_speed;
                }
                if (map[int(x_pos)][int(y_pos + y_dir * mov_speed)] == false){
                    y_pos += y_dir * mov_speed;
                }
                
            }
            // if they press a
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
               double old_x_dir = x_dir;
                x_dir = x_dir * cos(rot_speed) - y_dir * sin(rot_speed);
                y_dir = old_x_dir * sin(rot_speed) + y_dir * cos(rot_speed);
                double old_x_plane = x_plane;
                x_plane = x_plane * cos(rot_speed) - y_plane * sin(rot_speed);
                y_plane = old_x_plane * sin(rot_speed) + y_plane * cos(rot_speed);
            }
            // if they press S
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                if (map[int(x_pos - x_dir * mov_speed)][int(y_pos)] == false){
                    x_pos -= x_dir * mov_speed;
                }
                if (map[int(x_pos)][int(y_pos - y_dir * mov_speed)] == false){
                    y_pos -= y_dir * mov_speed;
                }
                
            }
            // if they press d
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                double old_x_dir = x_dir;
                x_dir = x_dir * cos(-rot_speed) - y_dir * sin(-rot_speed);
                y_dir = old_x_dir * sin(-rot_speed) + y_dir * cos(-rot_speed);
                double old_x_plane = x_plane;
                x_plane = x_plane * cos(-rot_speed) - y_plane * sin(-rot_speed);
                y_plane = old_x_plane * sin(-rot_speed) + y_plane * cos(-rot_speed);
            }
            // display the window
            window.display();
        }

    }

    return 0;
}