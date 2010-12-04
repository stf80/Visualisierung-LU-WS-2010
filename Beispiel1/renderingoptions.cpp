#include "renderingoptions.h"

RenderingOptions::RenderingOptions()
{
        // TODO set useful defaults
        light = QColor(255, 255, 255);
        ambient = QColor(55, 55, 55);
        diffuse = QColor(200, 200, 200);
        specular = QColor(255, 255, 255);
        k1 = 1;
        k2 = 1;
        exponent = 16.0;
        N = 100;
}
