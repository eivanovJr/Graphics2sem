#include "header.h"

enum ARGUMENTS
{
    FILENAME = 1, OUTPUT, GRADIENT, DITHERING, BIT, GAMMA
};

void drawGradient(uchar *buff, int width, int height);
void dither(uchar *buff, int width, int height, unsigned int bit, unsigned int dithering, double gamma);

int main(int argc, char**argv)
{
    unsigned int gradient, bit, dithering;
    double gamma;
    try {
        if (argc != 7)
            throw myException("Arguments count error\n");
        FILE* inputFile = nullptr;
        FILE* outputFile = nullptr;
        for(int i = 1; i < argc; i++)
        {
            switch(i)
            {
                case FILENAME :
                    if(!(inputFile = fopen(argv[i], "rb")))
                        throw myException("Could not open the file\n");
                    break;
                case OUTPUT:
                    outputFile = fopen(argv[i], "wb");
                    break;
                case GRADIENT:
                    gradient = atoi(argv[i]);
                    break;
                case DITHERING:
                    dithering = atoi(argv[i]);
                case BIT:
                    bit = atoi(argv[i]);
                    break;
                case GAMMA:
                    gamma = atof(argv[i]);
                    break;
                default:
                    throw myException("Bad number of arguments\n");
            }
        }
        int width, height, scale, elements;
        char format;
        fscanf(inputFile, "P%c\n%i %i\n%i\n", &format, &width, &height, &scale);
        if (format != '5'|| scale != 255)
            throw myException("Wrong fromat / scale\n");
        uchar *buff = new uchar[width * height];
        if (gradient == 0)
        {
            elements = fread(buff, 1, width * height, inputFile);
            if (elements != width * height)
                throw myException("Read error\n");
        }
        else
        {
            drawGradient(buff, width, height);
        }
        fclose(inputFile);
        dither(buff, width, height, bit, dithering, gamma);
        fprintf(outputFile, "P%c\n%i %i\n%i\n", format, width, height, scale);
        fwrite(buff, 1, width * height, outputFile);
        fclose(outputFile);
        delete[] buff;
    }
    catch (const myException &err)
    {
        cerr << err.getValue();
        return 1;
    }
    return 0;
}
