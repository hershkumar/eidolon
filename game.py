import curses
from curses import wrapper
import noise
import numpy as np
import random

#NOTE: LINES -> HEIGHT, COLS -> WIDTH

# world map dimensions
MAP_HEIGHT = 1024
MAP_WIDTH = 1024


# terrain gen constants
SEED = 5
SCALE = 30.0
OCTAVES = 8
LACUNARITY = 2.0
PERSISTENCE = 0.5

#COLORS
WATER = 1
SAND = 2
GRASS = 3
MOUNTAIN = 4
PEAK = 5


COLOR_GREY = 8

def map_colors(r, g, b):
    return (int(r/256 * 1000), int(g/256 * 1000), int(b/256 * 1000))

def change_colors():
    # define the RGB values of the terrain
    if (curses.can_change_color()):
        grey_tuple = map_colors(112,128,144)
        curses.init_color(COLOR_GREY, grey_tuple[0], grey_tuple[1], grey_tuple[2])

# generates a heightmap via perlin noise 
def gen_heightmap(scale, octaves, lacunarity, persistence):
    random.seed(SEED)
    shape = (MAP_WIDTH , MAP_HEIGHT)
    map = np.zeros(shape)
    # every pixel in the heightmap is based on the output of the noise function
    for i in range(shape[0]):
        for j in range(shape[1]):
            map[i][j] = 100 * noise.pnoise2(
            i/scale,
            j/scale,
            octaves=octaves,
            persistence=persistence,
            lacunarity=lacunarity,
            repeatx=MAP_WIDTH,
            repeaty=MAP_HEIGHT,
            base=0)
    # normalize it to be between 0 and 1
    map = (map - map.min())/(map.max() - map.min())
    # convert it to an array of integers
    #map = map.astype(int)
    
    return map

# gets the color a tile is based on its height
def get_color(height):
    # water
    if (height < .4):
        color_pair_num = WATER
    # sand
    elif (height < .45):
        color_pair_num = SAND
    # grass
    elif (height < .6):
        color_pair_num = GRASS
    # mountains
    elif (height < .7):
        color_pair_num = MOUNTAIN
    else:
        color_pair_num = PEAK
    
    return color_pair_num

def main(stdscr):
    # clear the screen
    stdscr.clear()
    # hide the cursor
    curses.curs_set(0)
    # set the colors
    change_colors()
    #panel dimensions
    MAP_PAD_WIDTH = curses.COLS - 15
    MAP_PAD_HEIGHT = curses.LINES - 5


    # Set up the color system
    curses.init_pair(WATER, curses.COLOR_WHITE, curses.COLOR_CYAN)
    curses.init_pair(SAND, curses.COLOR_WHITE, curses.COLOR_YELLOW)
    curses.init_pair(GRASS, curses.COLOR_WHITE, curses.COLOR_GREEN)
    curses.init_pair(MOUNTAIN, curses.COLOR_WHITE, COLOR_GREY)
    curses.init_pair(PEAK, curses.COLOR_WHITE, curses.COLOR_WHITE)
    # generate the heightmap
    map = gen_heightmap(SCALE, OCTAVES, LACUNARITY, PERSISTENCE)

    stdscr.refresh()
    # make a pad for the game map
    map_pad = curses.newpad(MAP_WIDTH, MAP_HEIGHT)
    # draw the entire map onto the pad
    for i in range(MAP_WIDTH - 1):
        for j in range(MAP_HEIGHT - 1):
            
            # we want to get the correct character to represent the pixel there
            height = map[i][j]
            ch = ' '
            color_pair_num = get_color(height)
            # add to the game map
            map_pad.addch(j,i, ch, curses.color_pair(color_pair_num))
    center_x = int(MAP_WIDTH/2)
    center_y = int(MAP_HEIGHT/2)
    map_pad.refresh(center_y, center_x , 0,0 , MAP_PAD_HEIGHT, MAP_PAD_WIDTH)
    
    stdscr.getkey()

wrapper(main)