#ifndef __IMAGE_H_INCLUDED
#define __IMAGE_H_INCLUDED

#include "h/Math101.h"

//-- ARGB, 32-bit
class Image
{
public:
    Image(int w, int h);
   ~Image();

    int Width () const { return m_width;  }
    int Height() const { return m_height; }

    void Clear(int color = 0);
    int* GetData() { return m_data; }

    void SetPixel(int x, int y, int color); // ARGB
    void SetPixel(int x, int y, vec3 color);
    void SetPixel(int x, int y, int r, int g, int b);
    void SetPixel(int x, int y, float r, float g, float b);

    void SaveToBMP(const char *filename);

protected:
    const int m_width  = -1;
    const int m_height = -1;

    int* m_data = nullptr;
};

#endif