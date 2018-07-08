#include "math.h"
#include <stdlib.h>
#include "stdint.h"

// Explanation for this math can be found here: https://www.rapidtables.com/convert/color/hsv-to-rgb.html

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct {
    float hue; // Expecting 0-360 degrees
    float sat; // Expecting 0-1
    float val; // Expecting 0-1
} HSV;

RGB hsv2rgb(HSV input) {
    RGB output;
    float c = input.val * input.sat;
    float x = c * (1 - fabs(fmod(input.hue / 60.0f, 2.0f) - 1));
    float m = input.val - c;

    float red = 0;
    float green = 0;
    float blue = 0;

    int color = floor(input.hue / 60);

    switch(color) {
        case 0:
            red = c;
            green = x;
            blue = 0;
            break;
        case 1:
            red = x;
            green = c;
            blue = 0;
            break;
        case 2:
            red = 0;
            green = c;
            blue = x;
            break;
        case 3:
            red = 0;
            green = x;
            blue = c;
            break;
        case 4:
            red = x;
            green = 0;
            blue = c;
            break;
        case 5:
            red = c;
            green = 0;
            blue = x;
            break;
    }
    output.r = (red + m) * 255;
    output.g = (green + m) * 255;
    output.b = (blue + m) * 255;
    return output;
}