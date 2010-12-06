#ifndef RENDERINGOPTIONS_H
#define RENDERINGOPTIONS_H

#include <QColor>

/**
  * @brief Struct for storing rendering options.
  *
  * This is used to communicate rendering options between
  * VolumeRenderer and MainWindow classes
  */
struct RenderingOptions
{
    QColor light, ambient, diffuse, specular;
    float k1, k2, exponent;
    unsigned int N;

    RenderingOptions();
};


#endif // RENDERINGOPTIONS_H
