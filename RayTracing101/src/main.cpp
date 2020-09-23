#include "h/Image.h"

int main(int argc, char* argv[])
{
    auto img = new Image(512, 256);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        float r = float(x) / w;
        float g = float(y) / h;

        img->SetPixel(x, y, r, g, 0.f);
    }

    img->SaveToBMP("out.bmp");

    return 0;
}