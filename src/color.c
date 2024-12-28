#include "color.h"

Vec3 unpackRGB(unsigned int packedRGB) {
    Vec3 color;
    color.x = ((packedRGB >> 16) & 0xFF) / 255.0f; // Red
    color.y = ((packedRGB >> 8) & 0xFF) / 255.0f;  // Green
    color.z = (packedRGB & 0xFF) / 255.0f;         // Blue
    return color;
}

void writeColour(FILE *ppmFile, Vec3 color) {
    int r = (int)(color.x * 255.0f);
    int g = (int)(color.y * 255.0f);
    int b = (int)(color.z * 255.0f);    //no .f & int

    r = r < 0 ? 0 : (r > 255 ? 255 : r);
    g = g < 0 ? 0 : (g > 255 ? 255 : g);
    b = b < 0 ? 0 : (b > 255 ? 255 : b);

    fprintf(ppmFile, "%d %d %d ", r, g, b);
}

int compareColor(const void *a, const void *b)
{
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }

    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}
