import curses
from curses import wrapper
import noise
import numpy as np
import random

#NOTE: LINES -> HEIGHT, COLS -> WIDTH

# world map dimensions
MAP_HEIGHT = 200
MAP_WIDTH = 200


# terrain gen constants
SEED = 5
SCALE = 100.0
OCTAVES = 8
LACUNARITY = 2.0
PERSISTENCE = 0.5

#COLORS
WATER_COLOR = 1
SAND_COLOR = 2
GRASS_COLOR = 3
MOUNTAIN_COLOR = 4
PEAK_COLOR = 5

# define the RGB values of the terrain


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
    
    # convert it to an array of integers
    map = map.astype(int)
    
    return map

# gets the color a tile is based on its height
def get_color(height):
    # water
    if (height < 1):
        color_pair_num = WATER_COLOR
    # sand
    elif (height < 4):
        color_pair_num = SAND_COLOR
    # grass
    elif (height < 8):
        color_pair_num = GRASS_COLOR
    elif (height < 12):
        color_pair_num = MOUNTAIN_COLOR
    else:
        color_pair_num = PEAK_COLOR
    
    return color_pair_num

def main(stdscr):
    # clear the screen
    stdscr.clear()
    # hide the cursor
    curses.curs_set(0)
    #panel dimensions
    MAP_PAD_WIDTH = curses.COLS - 15
    MAP_PAD_HEIGHT = curses.LINES - 5


    # Set up the color system
    curses.init_pair(WATER_COLOR, curses.COLOR_WHITE, curses.COLOR_CYAN)
    curses.init_pair(SAND_COLOR, curses.COLOR_WHITE, curses.COLOR_YELLOW)
    curses.init_pair(GRASS_COLOR, curses.COLOR_WHITE, curses.COLOR_GREEN)
    curses.init_pair(MOUNTAIN_COLOR, curses.COLOR_WHITE, curses.COLOR_RED)
    curses.init_pair(PEAK_COLOR, curses.COLOR_WHITE, curses.COLOR_WHITE)

    map = gen_heightmap(SCALE, OCTAVES, LACUNARITY, PERSISTENCE)
    
    stdscr.refresh()
    # make a pad for the game map
    map_pad = curses.newpad(MAP_WIDTH, MAP_HEIGHT)

    for i in range(MAP_WIDTH - 1):
        for j in range(MAP_HEIGHT - 1):
            
            # we want to get the correct character to represent the pixel there
            height = map[i][j]
            ch = ' '
            color_pair_num = get_color(height)
            # add to the game map
            map_pad.addch(j,i, ch, curses.color_pair(color_pair_num))
    map_pad.refresh(0,0 , 0,0 , MAP_PAD_HEIGHT, MAP_PAD_WIDTH)
    stdscr.getkey()

wrapper(main)