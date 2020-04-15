#ifndef DITHERING_HEADER_H
#define DITHERING_HEADER_H

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

typedef unsigned char uchar;


class myException : public exception
{
public:
    string value;
    [[nodiscard]] string getValue() const {return value;}
    explicit myException(string str) {value = std::move(str);}
};

#endif //DITHERING_HEADER_H
