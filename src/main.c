#include <stdio.h>
#include <ncurses.h>
#include <termios.h>
#include "stb_image.h"

#include "ansi_colours.h"

#define ARG_COUNT 2
#define DEFAULT_CHANNELS 3
#define GRAYSCALE_CHANNELS 1

int input;

int scale = 5;

uint32_t rgbToHex(unsigned char r, unsigned char g, unsigned char b) {
    return (r << 16) | (g << 8) | b;
}

void asciify(int width, int height, unsigned char* data) {
    for (int x = 0; x < width / scale; x++) {
        for (int y = 0; y < height / scale; y++) {
            int index = ((y*scale) * width + (x*scale)) * DEFAULT_CHANNELS;

            int ir = data[index + 0];
            int ig = data[index + 1];
            int ib = data[index + 2];

            uint32_t hex = rgbToHex(ir, ig, ib);
            uint8_t ansiIndex = ansi256_from_rgb(hex);

            attron(COLOR_PAIR(ansiIndex));
            mvprintw(y, x, " ");
            attroff(COLOR_PAIR(ansiIndex));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != ARG_COUNT) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    if (initscr() == NULL || start_color() == ERR || raw() == ERR || noecho() == ERR) {
        printf("Error initializing ncurses.\n");
        return 1;
    }

    if (has_colors() == FALSE || can_change_color() == FALSE) {
        printf("Error cannot change color.\n");
        return 1;
    }

    // Load image
    int width, height, channels;
    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, DEFAULT_CHANNELS);
    // {r1, g1, b1, r2, g2, b2, ...}
    if (data == NULL) {
        printf("Failed to load image\n");
        return 1;
    }

    for (int i = 0; i < 256; i++) {
        init_pair(i, COLOR_BLACK, i);
    }

    // Draw image to prevent empty screen before input
    asciify(width, height, data);

    while ((input = getch() ) != 27)
    {
        /* Check if the window was resized */
        if (input == KEY_RESIZE)
            asciify(width, height, data);
    }

    refresh();

    // Free image
    stbi_image_free(data);

    echo();
    noraw();
    endwin();
    
    return 0;
}