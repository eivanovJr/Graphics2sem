#include "header.h"


int main(int argc, char**argv)
{
    string arg, io, toColor;
    int count;
    try
    {
        if (argc != 11)
            throw myException("Bad arguments count\n");
        Image myImage;
        for (int i = 1; i < 11; i++)
        {
            arg = argv[i];
            if (arg == "-f")
            {
                i++;
                arg = argv[i];
                myImage.DefineColor(arg);
            }
            else if (arg == "-t")
            {
                i++;
                arg = argv[i];
                myImage.DefinetoColor(arg);
            }
            else if (arg == "-i")
            {
                i++;
                arg = argv[i];
                i++;
                io = argv[i];
                myImage.DefineInput(arg, io);
            }
            else if (arg == "-o")
            {
                i++;
                arg = argv[i];
                i++;
                io = argv[i];
                myImage.DefineOutput(arg, io);
            }
            else
                throw myException("no -f -t -o -i");
        }
        myImage.read();
        myImage.convert();
        myImage.write();
    }
    catch (const myException &err)
    {
        cerr << err.getValue();
        return 1;
    }
    return 0;
}
