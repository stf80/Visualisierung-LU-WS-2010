#ifndef RENDERINGOPTIONS_H
#define RENDERINGOPTIONS_H

#include <QColor>

struct RenderingOptions
{
    QColor light, ambient, diffuse, specular;
    float k1, k2, exponent;
    unsigned int N;

    RenderingOptions();
};


#endif // RENDERINGOPTIONS_H
