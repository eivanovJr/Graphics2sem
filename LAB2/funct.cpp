#include "Line.h"

int intPart(double x)
{
    return (int)x;
}

double distance(Point a, Point b)
{
    return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y));
}

void draw(int x, int y, double thickness, double brightness, int height, int width, uchar *buff)
{
    thickness = max(min(thickness, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    double color = brightness / 255.0;
    double sRGBcolor = buff[y * width + x] / 255.0;
    double Linear = sRGBcolor <= 0.04045 ? sRGBcolor / 12.92 : pow((sRGBcolor + 0.055) / 1.055, 2.4);
    double temp = (1 - thickness) * color + thickness * Linear;
    sRGBcolor = temp <= 0.0031308 ? 12.92 * temp : 1.055 * pow(temp, 1 / 2.4) - 0.055;
    buff[width * y + x] = 255 * sRGBcolor;
}

void draw(int x, int y, double thickness, double brightness, double gamma, int height, int width, uchar *buff)
{
    thickness = max(min(thickness, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    buff[width * y + x] = 255 * pow((thickness * buff[width * y + x] + brightness * (1 - thickness)) / 255.0, (1.0 / gamma - 1.0) * (1.0 - thickness) + 1.0);
}

void plot(int x, int y, double thickness, double brightness, int height, int width, double gamma, bool reverse, uchar *buff)
{
    if (reverse)
    {
        if (gamma == 0) {
            draw(y, x, 1.0 - thickness, brightness, height, width, buff);
        }
        else
            draw(y, x, 1.0 - thickness, brightness, gamma, height, width, buff);
    }
    else
    {
        if (gamma == 0) {
            draw(x, y, 1.0 - thickness, brightness, height, width, buff);
        }
        else
            draw(x, y, 1.0 - thickness, brightness, gamma, height, width, buff);
    }
}

void drawLine(Line myLine, uchar *buff, int width, int height)
{
    double dx, dy, gradient, y;
    bool rotate = abs(myLine.begin.Y - myLine.end.Y) > abs(myLine.begin.X - myLine.end.X);
    if (rotate)
    {
        swap(myLine.begin.X, myLine.begin.Y);
        swap(myLine.end.X, myLine.end.Y);
    }
    if (myLine.begin.X > myLine.end.X)
    {
        swap(myLine.begin.X, myLine.end.X);
        swap(myLine.begin.Y, myLine.end.Y);
    }
    dx = myLine.end.X - myLine.begin.X;
    dy = myLine.end.Y - myLine.begin.Y;
    gradient = dy / dx;
    y = myLine.begin.Y + gradient * (round(myLine.begin.X) - myLine.begin.X);
    double radius = (myLine.thickness) / 2.0;
    for (int x = round(myLine.begin.X); x <= round(myLine.end.X); x++)
    {
        for (int y1 = intPart(y - (myLine.thickness - 1) / 2); y1 <= intPart(y - (myLine.thickness - 1) / 2 + myLine.thickness); y1++)
        {
           plot(x, y1, min(1.0, (myLine.thickness + 1.0) / 2.0 - fabs(y - y1)), myLine.brightness, height, width, myLine.gamma, rotate, buff);
        }
        y += gradient;
    }
    Point plotStart = { round(myLine.begin.X), round(myLine.begin.Y) };
    Point plotEnd = { round(myLine.end.X), round(myLine.end.Y) };

    for (int plotX = round(myLine.begin.X) - radius; plotX < round(myLine.begin.X); plotX++) {
        y = myLine.begin.Y + gradient * (plotX - myLine.begin.X);
        for (int plotY = (int)(y - radius); plotY <= (int)(y + radius); plotY++) {
            plot(plotX, plotY, min(1.0, radius - distance({ static_cast<double>(plotX), static_cast<double>(plotY) }, { plotStart.X, plotStart.Y })),  myLine.brightness, height, width, myLine.gamma, rotate, buff);

        }

    }
    for (int plotX = round(myLine.end.X + 1); plotX <= round(myLine.end.X) + radius; plotX++) {
        y = myLine.begin.Y + gradient * (plotX - myLine.begin.X);
        for (int plotY = int(y - radius); plotY <= int(y + radius); plotY++) {
            plot(plotX, plotY, min(1.0, radius - distance({ static_cast<double>(plotX), static_cast<double>(plotY) }, { plotEnd.X, plotEnd.Y })),  myLine.brightness, height, width, myLine.gamma, rotate, buff);
        }
    }
}


