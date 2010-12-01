#include "renderingoptions.h"

RenderingOptions::RenderingOptions()
{
        // TODO set useful defaults
        light = QColor(1, 1, 1);
        ambient = QColor(1, 1, 1);
        diffuse = QColor(1, 1, 1);
        specular = QColor(1, 1, 1);
        k1 = 1;
        k2 = 1;
        exponent = 16;
        N = 100;

}
