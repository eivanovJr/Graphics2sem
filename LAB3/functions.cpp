#include "header.h"

void drawGradient(uchar *buff, int width, int height)
{
    double step;
    int brightness;
    step = (double)height / 255.0;
    for (int i = 0; i < height; i++)
    {
        brightness = (int)((double)i / step);
        for (int j = 0; j < width; j++)
        {
            buff[i * width + j] = brightness;
        }
    }
}

