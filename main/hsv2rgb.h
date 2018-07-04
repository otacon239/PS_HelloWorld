#ifndef _hsv2rgb_h
#define _hsv2rgb_h

typedef struct
{
    char r;
    char g;
    char b;
} RGB;

typedef struct
{
    float hue; // Expecting 0-360 degrees
    float sat; // Expecting 0-1
    float val; // Expecting 0-1
} HSV;

RGB hsv2rgb(HSV);

#endif