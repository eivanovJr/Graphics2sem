#ifndef COLOCCONVERTION_HEADER_H
#define COLOCCONVERTION_HEADER_H

#include <exception>
#include <iostream>
#include <string>
#include <cmath>

typedef unsigned char uchar;

using namespace std;

class myException : public exception
{
public:
    string value;
    [[nodiscard]] string getValue() const {return value;}
    explicit myException(string str) {value = std::move(str);}
};

enum Colors{RGB = 0, HSL, HSV, YCbCr_601, YCbCr_709, YCoCg, CMY};

class Point
{
public:
    uchar X;
    uchar Y;
    uchar Z;
};

class Image
{
public:
    Image();
    ~Image();
    void DefineColor(const string&);
    void DefinetoColor(const string&);
    void DefineInput(const string&, const string&);
    void DefineOutput(const string&, const string&);
    void read();
    void convertRGB();
    void convert();
    void write();
    void setPicture(int,int);
private:
    Colors color;
    Colors toColor;
    string input;
    bool multi_input;
    string output;
    bool multi_output;
    int width;
    int height;
    Point *buff;
};

#endif //COLOCCONVERTION_HEADER_H
