#include "header.h"

Image::Image()
{
    buff = nullptr;
    width = 0;
    height = 0;
    color = RGB;
    toColor = RGB;
    multi_input = false;
    multi_output = false;
    input = " ";
    output = " ";
}

Image::~Image()
{
    delete [] buff;
}

void Image::setPicture(int w, int h)
{
    width = w;
    height = h;
    buff = new Point[width * height];
}

void Image::DefineColor(const string& input)
{
    if (input == "RGB")
        color = RGB;
    else if (input == "HSL")
        color = HSL;
    else if (input == "HSV")
        color = HSV;
    else if (input == "YCbCr.601")
        color = YCbCr_601;
    else if (input == "YCbCr.709")
        color = YCbCr_709;
    else if (input == "YCoCg")
        color = YCoCg;
    else if (input == "CMY")
        color = CMY;
    else
        throw myException("Bad color format");
}


void Image::DefinetoColor(const string & input)
{
    if (input == "RGB")
        toColor = RGB;
    else if (input == "HSL")
        toColor = HSL;
    else if (input == "HSV")
        toColor = HSV;
    else if (input == "YCbCr.601")
        toColor = YCbCr_601;
    else if (input == "YCbCr.709")
        toColor = YCbCr_709;
    else if (input == "YCoCg")
        toColor = YCoCg;
    else if (input == "CMY")
        toColor = CMY;
    else
        throw myException("Bad color format");
}

void Image::DefineInput(const string &num, const string &in)
{
    input = in;
    if (num == "3")
        multi_input = true;
    else if (num != "1")
        throw myException("Bad inputs number");
}

void Image::DefineOutput(const string&num, const string&out)
{
    output = out;
    if (num == "3")
        multi_output = true;
    else if (num != "1")
        throw myException("Bad outputs number");
}

void Image::read()
{
    char format;
    int w, h, initw, inith, scale;
    if (multi_input)
    {
        FILE *in1, *in2, *in3;
        input.insert(input.size() - 4, "_1");
        if(!(in1 = fopen(input.c_str(), "rb")))
            throw myException("Could not open 1st file\n");
        input.replace(input.size() - 5, 1, "2");
        if(!(in2 = fopen(input.c_str(), "rb")))
            throw myException("Could not open 2nd file\n");
        input.replace(input.size() - 5, 1, "3");
        if(!(in3 = fopen(input.c_str(), "rb")))
            throw myException("Could not open 3rd file\n");
        if (fscanf(in1, "P%c\n%i %i\n%i\n", &format, &initw, &inith, &scale) != 4 || format != '5')
            throw myException("Problems reading 1st file");
        if (fscanf(in2, "P%c\n%i %i\n%i\n", &format, &w, &h, &scale) != 4 || format != '5' || w != initw || h != inith)
            throw myException("Problems reading 2nd file");
        if (fscanf(in3, "P%c\n%i %i\n%i\n", &format, &w, &h, &scale) != 4 || format != '5' || w != initw || h != inith)
            throw myException("Problems reading 3rd file");
        width = w;
        height = h;
        buff = new Point[width * height];
        for (int i = 0; i < width * height; i++)
        {
            buff[i].X = fgetc(in1);
            buff[i].Y = fgetc(in2);
            buff[i].Z = fgetc(in3);
        }
        fclose(in1);
        fclose(in2);
        fclose(in3);
    }
    else
    {
        FILE *in;
        if(!(in = fopen(input.c_str(), "rb")))
            throw myException("Error reading File ");
        if (fscanf(in, "P%c\n%i %i\n%i\n", &format, &initw, &inith, &scale) != 4 || format != '6')
            throw myException("Problems reading 1st file");
        width = initw;
        height = inith;
        buff = new Point[width * height];
        for (int i = 0; i < width * height; i++)
        {
            buff[i].X = fgetc(in);
            buff[i].Y = fgetc(in);
            buff[i].Z = fgetc(in);
        }
        fclose(in);
    }
}

void Image::write()
{
    if (multi_output)
    {
        FILE *out1, *out2, *out3;
        output.insert(output.size() - 4, "_1");
        out1 = fopen(output.c_str(), "wb");
        output.replace(output.size() - 5, 1, "2");
        out2 = fopen(output.c_str(), "wb");
        output.replace(output.size() - 5, 1, "3");
        out3 = fopen(output.c_str(), "wb");
        fprintf(out1, "P5\n%i %i\n255\n",  width, height);
        fprintf(out2, "P5\n%i %i\n255\n",  width, height);
        fprintf(out3, "P5\n%i %i\n255\n",  width, height);
        for (int i = 0; i < width * height; i++)
        {
            fputc(buff[i].X, out1);
            fputc(buff[i].Y, out2);
            fputc(buff[i].Z, out3);
        }
        fclose(out1);
        fclose(out2);
        fclose(out3);
    }
    else
    {
        FILE *out;
        int count;
        out = fopen(output.c_str(), "wb");
        fprintf(out, "P6\n%i %i\n255\n",  width, height);
        count = fwrite(buff, sizeof(char), width * height * 3, out);
        if (count != width * height * 3)
            throw myException("Error during writing");
        fclose(out);
    }
}

void Image::convertRGB()
{
    if (color == RGB)
        return;
    switch(color)
    {
        case HSL:
        case HSV:
        {
            double H, S, L, C, X, m, H_D;
            for (int i = 0; i < height * width; i++)
            {
                H = (buff[i].X / 255.0) * 360.0;
                S = buff[i].Y / 255.0;
                L = buff[i].Z / 255.0;
                H_D = H/ 60;
                if (color == HSL) {
                    C = (1 - abs(2 * L - 1)) * S;
                    X = C * (1 - abs(fmod(H_D, 2) - 1));
                    m = L - C / 2.0;
                }
                else
                {
                    C = S * L;
                    X = C * (1.0 - abs(fmod(H_D, 2) - 1.0));
                    m = L - C;
                }

                m *= 255.0;
                if ((H_D >= 0) && (H_D <= 1))
                {
                    buff[i].X = C * 255.0 + m;
                    buff[i].Y = X * 255.0 + m;
                    buff[i].Z = 0 + m;
                }
                if ((H_D > 1) && (H_D <= 2))
                {
                    buff[i].X = X * 255.0 + m;
                    buff[i].Y = C * 255.0 + m;
                    buff[i].Z = 0 + m;
                }
                if ((H_D > 2) && (H_D <= 3))
                {
                    buff[i].X = 0 + m;
                    buff[i].Y = C * 255.0 + m;
                    buff[i].Z = X * 255.0 + m;
                }
                if ((H_D > 3) && (H_D <= 4))
                {
                    buff[i].X = 0 + m;
                    buff[i].Y = X * 255.0 + m;
                    buff[i].Z = C * 255.0 + m;
                }
                if ((H_D > 4) && (H_D <= 5))
                {
                    buff[i].X = X * 255.0 + m;
                    buff[i].Y = 0 + m;
                    buff[i].Z = C * 255.0 + m;
                }
                if ((H_D > 5) && (H_D <= 6))
                {
                    buff[i].X = C * 255.0 + m;
                    buff[i].Y = 0 + m;
                    buff[i].Z = X * 255.0 + m;
                }

            }
        }
        break;
        case YCbCr_601:
        case YCbCr_709:
        {
            double Kr, Kg, Kb, Y, Cb, Cr, R, G, B;
            if (color == YCbCr_601) {
                Kr = 0.299;
                Kg = 0.587;
                Kb = 0.114;
            }
            else
            {
                Kr = 0.0722;
                Kg = 0.2126;
                Kb = 0.7152;
            }
            for (int i = 0; i < width * height; i++) {
                Y = buff[i].X / 255.0;
                Cb = (buff[i].Y / 255.0) - 0.5;
                Cr = (buff[i].Z / 255.0) - 0.5;
                R = (Y + Cr * (2.0 - 2.0 * Kr));
                G = (Y - (Kb / Kg) * (2.0 - 2.0 * Kb) * Cb - (Kr / Kg) * (2.0 - 2.0 * Kr) * Cr);
                B = (Y + (2.0 - 2.0 * Kb) * Cb);
                if (R < 0)
                    R = 0;
                if (G < 0)
                    G = 0;
                if (B < 0)
                    B = 0;
                if (R > 1)
                    R = 1;
                if (G > 1)
                    G = 1;
                if (B > 1)
                    B = 1;
                buff[i].X = R * 255.0;
                buff[i].Y = G * 255.0;
                buff[i].Z = B * 255.0;
            }
        }
        break;
        case YCoCg:
        {
            double Y, Co, Cg, R, G, B;
            for (int i = 0; i < width * height; i++)
            {
                Y = buff[i].X / 255.0;
                Co = (buff[i].Y / 255.0) - 0.5;
                Cg = (buff[i].Z / 255.0) - 0.5;
                R = Y + Co - Cg;
                G = Y + Cg;
                B = Y - Co - Cg;
                buff[i].X = Y * 255.0;
                buff[i].Y = (Co + 0.5) * 255.0;
                buff[i].Z = (Cg + 0.5) * 255.0;
                if (R < 0)
                    R = 0;
                if (G < 0)
                    G = 0;
                if (B < 0)
                    B = 0;
                if (R > 1)
                    R = 1;
                if (G > 1)
                    G = 1;
                if (B > 1)
                    B = 1;
                buff[i].X = R * 255.0;
                buff[i].Y = G * 255.0;
                buff[i].Z = B * 255.0;
            }
        }
        break;
        case CMY:
        {
            double C, M, Y, R, G, B;
            for (int i = 0; i < width * height; i++)
            {
                C = buff[i].X / 255.0;
                M = buff[i].Y / 255.0;
                Y = buff[i].Z / 255.0;
                R = 1 - C;
                G = 1 - M;
                B = 1 - Y;
                buff[i].X = R * 255.0;
                buff[i].Y = G * 255.0;
                buff[i].Z = B * 255.0;
            }
        }
        break;
        default:
            throw myException("wrong color");
    }

}

void Image::convert()
{
    if (color == toColor)
        return;
    convertRGB();
    if (toColor == RGB)
        return;
    switch(toColor)
    {
        case HSL:
        case HSV:
        {
            double R, G, B, Max, Min, V, C, L, H, S;
            for (int i = 0; i < width * height; i++) {
                R = buff[i].X / 255.0;
                G = buff[i].Y / 255.0;
                B = buff[i].Z / 255.0;
                Max = max(R, max(G, B));
                Min = min(R, min(G, B));
                V = Max;
                C = Max - Min;
                L = V - C / 2.0;
                if (C == 0)
                    H = 0;
                else
                {
                    if (V == R)
                        H = (60.0) * ((G - B) / C);
                    else if (V == G)
                        H = (60.0) * (2 + (B - R) / C);
                    else if (V == B)
                        H = (60.0) * (4 + (R - G) / C);
                    else
                        H = 0;
                }
                if (toColor == HSV)
                {
                    S = (V == 0) ? 0 : C / V;
                    buff[i].Z = V * 255.0;
                }
                if (toColor == HSL) {
                    S = ((L == 0) || (L == 1)) ? 0 : ((V - L) / min(L, 1 - L));
                    buff[i].Z = L * 255.0;
                }
                buff[i].X = (H/360.0)*255.0;
                buff[i].Y = S * 255.0;
            }
        }
        break;
        case YCbCr_601:
        case YCbCr_709:
        {
            double Kr, Kg, Kb, Y, Cb, Cr, R, G, B;
            if (toColor == YCbCr_601) {
                Kr = 0.299;
                Kg = 0.587;
                Kb = 0.114;
            }
            else
            {
                Kr = 0.0722;
                Kg = 0.2126;
                Kb = 0.7152;
            }
            for (int i = 0; i < width * height; i++)
            {
                R = buff[i].X / 255.0;
                G = buff[i].Y / 255.0;
                B = buff[i].Z / 255.0;
                Y = Kr * R + Kg * G + Kb * B;
                Cb = 0.5 * ((B - Y) / (1.0 - Kb));
                Cr = 0.5 * ((R - Y) / (1.0 - Kr));
                buff[i].X = Y * 255.0;
                buff[i].Y = (Cb + 0.5)*255.0;
                buff[i].Z = (Cr + 0.5)*255.0;
            }
        }
        break;
        case YCoCg:
        {
            double Y, Co, Cg, R, G, B;
            for (int i = 0; i < width * height; i++)
            {
                R = buff[i].X / 255.0;
                G = buff[i].Y / 255.0;
                B = buff[i].Z / 255.0;
                Y = R / 4 + G / 2 + B / 4;
                Co = R / 2 - B / 2;
                Cg = -R / 4 + G / 2 - B / 4;
                buff[i].X = Y * 255.0;
                buff[i].Y = (Co + 0.5) * 255.0;
                buff[i].Z = (Cg + 0.5) * 255.0;
            }
        }
        break;
        case CMY:
        {
            double C, M, Y, R, G, B;
            for (int i = 0; i < width * height; i++)
            {
                R = buff[i].X / 255.0;
                G = buff[i].Y / 255.0;
                B = buff[i].Z / 255.0;
                C = 1 - R;
                M = 1 - G;
                Y = 1 - B;
                buff[i].X = C * 255.0;
                buff[i].Y = M * 255.0;
                buff[i].Z = Y * 255.0;
            }
        }
        break;
        default:
            throw myException("wrong color");
    }


}
