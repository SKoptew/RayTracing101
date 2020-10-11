#define _CRT_SECURE_NO_WARNINGS

#include "h/Image.h"
#include <stdio.h>
#include <windows.h>

Image::Image(int w, int h) :
    m_width(w),
    m_height(h)
{
    m_data = new int[w*h];
}

Image::~Image()
{
    delete[] m_data;
}

void Image::Clear(int color)
{
    for (int i = 0, sz = m_width * m_height; i < sz; ++i)
    {
        m_data[i] = color;
    }
}

void Image::SetPixel(int x, int y, int color)
{
    m_data[y*m_width + x] = color;
}

void Image::SetPixel(int x, int y, vec3 color)
{
    const int _r = int(color.x*255.99f) & 0xff;
    const int _g = int(color.y*255.99f) & 0xff;
    const int _b = int(color.z*255.99f) & 0xff;

    m_data[y*m_width + x] = (0xff << 24) | (_r << 16) | (_g << 8) | _b;
}

void Image::SetPixel(int x, int y, int r, int g, int b)
{
    int value = (0xff << 24) | ((r&0xff) << 16) | ((g&0xff) << 8) | (b&0xff);

    m_data[y*m_width + x] = value;
}

void Image::SetPixel(int x, int y, float r, float g, float b)
{
    int value = (0xff << 24) | (int(r*255.99f) << 16) | (int(g * 255.99f) << 8) | int(b * 255.99f);

    m_data[y*m_width + x] = value;
}

void Image::SaveToBMP(const char* fname)
{
    if (FILE *file = fopen(fname, "wb"))
    {
        BITMAPINFOHEADER bmih;
        bmih.biSize        = sizeof(BITMAPINFOHEADER);
        bmih.biWidth       = m_width;
        bmih.biHeight      = m_height;
        bmih.biPlanes      = 1;
        bmih.biBitCount    = sizeof(int) * 8;
        bmih.biCompression = BI_RGB;
        bmih.biSizeImage   = m_width * m_height * sizeof(int);
        bmih.biClrUsed     = 0;

        BITMAPFILEHEADER bmfh;
        int  nBitsOffset = sizeof(BITMAPFILEHEADER) + bmih.biSize;
        LONG lImageSize  = bmih.biSizeImage;
        LONG lFileSize   = nBitsOffset + lImageSize;
        bmfh.bfType      = 'B' + ('M' << 8);
        bmfh.bfOffBits   = nBitsOffset;
        bmfh.bfSize      = lFileSize;
        bmfh.bfReserved1 = 0;
        bmfh.bfReserved2 = 0;

        fwrite(&bmfh,  1, sizeof(BITMAPFILEHEADER), file);
        fwrite(&bmih,  1, sizeof(BITMAPINFOHEADER), file);
        fwrite(m_data, 1, lImageSize, file);

        fclose(file);
    }
    else
        printf("Cannot open %s\n", fname);
}