#include "Line.h"

enum ARGUMENTS
{
    FILENAME = 1, OUTPUT, BRIGHTNESS, THICKNESS, X_S, Y_S, X_END, Y_END, GAMMA
};

void drawLine(Line myLine, uchar *buff, int width, int height);

int main(int argc, char**argv)
{
    try {
        if (argc < 9)
            throw myException("Arguments count error\n");
        FILE* inputFile = nullptr;
        FILE* outputFile = nullptr;
        Line myLine;
        myLine.gamma = 0;
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
                case BRIGHTNESS:
                    myLine.brightness = atoi(argv[i]);
                    break;
                case THICKNESS:
                    myLine.thickness = atof(argv[i]);
                case X_S:
                    myLine.begin.X = atof(argv[i]);
                    break;
                case Y_S:
                    myLine.begin.Y = atof(argv[i]);
                    break;
                case X_END:
                    myLine.end.X = atof(argv[i]);
                    break;
                case Y_END:
                    myLine.end.Y = atof(argv[i]);
                    break;
                case GAMMA:
                    myLine.gamma = atof(argv[i]);
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
        elements = fread(buff, 1, width * height, inputFile);
        fclose(inputFile);
        if (elements != width * height)
            throw myException("Read error\n");
        drawLine(myLine, buff, width, height);
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
