#include "header.h"

int index(int x, int y, int width)
{
    return x * width + y;
}

void draw(int x, int y, int thickness, int brightness, int bit, int height, int width, uchar *buff)
{
    int factor = pow(2, bit);
    factor--;
    double sRGBcolor = brightness / 255.0;
    double Linear = sRGBcolor <= 0.04045 ? sRGBcolor / 12.92 : pow((sRGBcolor + 0.055) / 1.055, 2.4);
    sRGBcolor = Linear <= 0.0031308 ? 12.92 * Linear : 1.055 * pow(Linear, 1 / 2.4) - 0.055;
    sRGBcolor *= 255.0;
    buff[x * width + y] = round(factor * (double)sRGBcolor / 255.0) * (255 / factor);
}

void draw(int x, int y, double thickness, int bit, int brightness, double gamma, int height, int width, uchar *buff)
{
    int factor = pow(2, bit);
    factor--;
    double gammaV;
    gammaV = 255 * pow(brightness / 255.0, (1.0 / gamma));
    buff[x * width + y] = round(factor * (double)gammaV / 255) * (255 / factor);

}

void plot (int x, int y, int brightness, int bit, double gamma, int height, int width, uchar *buff)
{
    if (gamma == 0)
    {
        draw(x, y, 1, brightness, bit, height, width, buff);
    }
    else
    {
        draw(x, y, 1, bit, brightness, gamma, height, width, buff);
    }
}

void noDithering(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    int newpixel;
    int factor = pow(2, bit);
    factor--;
    int treshold = 255 / factor;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            newpixel = buff[i * width + j] / treshold * treshold;
            plot(i, j, newpixel, bit, gamma, height, width, buff);
        }
    }
}

void Ordered(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma) {
    int treshold[8][8] = {0, 48, 12, 60, 3, 51, 15, 63,
                          32, 16, 44, 28, 35, 19, 47, 31,
                          8, 56, 4, 52, 11, 59, 7, 55,
                          40, 24, 36, 20, 43, 27, 39, 23,
                          2, 50, 14, 62, 1, 49, 13, 61,
                          34, 18, 46, 30, 33, 17, 45, 29,
                          10, 58, 6, 54, 9, 57, 5, 53,
                          42, 26, 38, 22, 41, 25, 37, 21};
    double error;
    int newpixel, oldpixel;
    double pallete;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pallete = buff[i * width + j] + 255 / factor * (double(treshold[i % 8][j % 8]) / 64.0 - 1 / 2.0);
            newpixel = round(factor * (double)(pallete) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
        }
    }
}

void Random(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    int newpixel;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            newpixel = round(factor * double(buff[i * width + j]) / 255.0) * (255 / factor);
            if (rand() % 256 > buff[i * width + j])
                plot(i, j, newpixel, bit, gamma, height, width, buff);
            else
                plot(i, j, newpixel - 255 / factor, bit, gamma, height, width, buff);
        }
    }
}

void FloydSteinberg(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    double error;
    int newpixel, oldpixel;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            oldpixel = buff[index(i, j, width)];
            newpixel = round(factor * (double)(buff[index(i, j, width)]) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
            error = oldpixel - newpixel;
            buff[index(i, j + 1, width)] += error * 7 / 16.0;
            buff[index(i + 1, j - 1, width)] += error * 3 / 16.0;
            buff[index(i + 1, j, width)] += error * 5 / 16.0;
            buff[index(i + 1, j + 1, width)] += error * 1 / 16.0;
        }
    }
}

void Jarvis(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    double error;
    int multiply[5] = {3, 5, 7, 5 ,3};
    int oldpixel, newpixel, multiplier;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            oldpixel = buff[i * width + j];
            newpixel = round(factor * (double)(buff[index(i, j, width)]) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
            error = oldpixel - newpixel;
            if (i > 0 && i < height && j + 1 > 0 && j + 1 < width)
                buff[i * width + j + 1] += error * 7 / 48.0;
            if (i > 0 && i < height && j + 2 > 0 && j + 2 < width)
                buff[i * width + j + 2] += error * 5 / 48.0;
            multiplier = -2;
            for (int m = 1; m <= 2; m ++)
            {
                multiplier += 2;
                for (int n = -2; n <= 2; n++)
                {
                    if (i + m > 0 && i + m < height && j + n > 0 && j + n < width)
                        buff[(i + m) * width + j + n] += error * multiply[n + 2] * multiplier / 48.0;
                }
            }
        }
    }
}

void Sierra(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    double error;
    int multiply2[5] = {2, 4, 5, 4, 2};
    int multiply3[3] = {2, 3, 2};
    int newpixel, oldpixel;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldpixel = buff[i * width + j];
            newpixel = round(factor * (double)(buff[index(i, j, width)]) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
            error = oldpixel - newpixel;
            if (j + 1 < width)
                buff[i * width + j + 1] += error * 5 / 32.0;
            if (j + 2 < width)
                buff[i * width + j + 2] += error * 3 / 32.0;
            if (i + 1 < height)
            {
                for (int m = -2; m <= 2; m++)
                {
                    if (j + m > 0 && j + m < width)
                    {
                        buff[(i + 1) * width + j + m] += error * multiply2[m + 2] / 32.0;
                    }
                }
                if (i + 2 < height)
                {
                    for (int m = -1; m <= 1; m++)
                    {
                        if (j + m > 0 && j + m < width)
                        {
                            buff[(i + 2) * width + j + m] += error * multiply3[m + 1] / 32.0;
                        }
                    }
                }
            }
        }
    }
}

void Atkinson(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    double error;
    int newpixel, oldpixel;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldpixel = buff[i * width + j];
            newpixel = round(factor * (double)(buff[index(i, j, width)]) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
            error = oldpixel - newpixel;
            for (int m = 1; m <= 2; m++)
            {
                if (j + m < width)
                    buff[i * width + j + m] += error * 1 / 8.0;
            }
            if (i + 1 < height)
            {
                for (int m = -1; m < 2; m++)
                {
                    if (j + m > 0 && j + m < width)
                        buff[(i + 1) * width + j + m] += error * 1 / 8.0;
                }
                if (i + 2 < height)
                    buff[(i + 2) * width + j] += error * 1 / 8.0;
            }
        }
    }
}

void Halftone(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    int treshold[4][4] = {7 ,13, 11, 4, 12, 16 ,14, 8, 10, 15, 6, 2, 5, 9, 3, 1};
    double error;
    int newpixel, oldpixel;
    double pallete;
    int factor = pow(2, bit);
    factor--;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pallete = buff[i * width + j] + 255 / factor * (double(treshold[i % 4][j % 4]) / 16.0 - 1 / 2.0);
            newpixel = round(factor * (double)(pallete) / 255) * (255 / factor);
            plot(i, j, newpixel, bit, gamma, height, width, buff);
        }
    }
}

void dither(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma)
{
    switch (dithering)
    {
        case 0 :
            noDithering(buff, width, height, bit, dithering, gamma);
            break;
        case 1 :
            Ordered(buff, width, height, bit, dithering, gamma);
            break;
        case 2 :
            Random(buff, width, height, bit, dithering, gamma);
            break;
        case 3 :
            FloydSteinberg(buff, width, height, bit, dithering, gamma);
            break;
        case 4 :
            Jarvis(buff, width, height, bit, dithering, gamma);
            break;
        case 5 :
            Sierra(buff, width, height, bit, dithering, gamma);
            break;
        case 6 :
            Atkinson(buff, width, height, bit, dithering, gamma);
            break;
        case 7 :
            Halftone(buff, width, height, bit, dithering, gamma);
            break;
        default : throw myException("Wrong dithering number\n");
    }

}