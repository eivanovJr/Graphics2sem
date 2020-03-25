#ifndef LINEDRAWING_LINE_H
#define LINEDRAWING_LINE_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

typedef unsigned char uchar;

class Point
{
public:
    Point() = default;
    double X;
    double Y;
};

class Line
{
public:
    Point begin;
    Point end;
    double thickness;
    int brightness;
    double gamma;
};

class myException : public exception
{
public:
    string value;
    [[nodiscard]] string getValue() const {return value;}
    explicit myException(string str) {value = std::move(str);}
};
#endif //LINEDRAWING_LINE_H
