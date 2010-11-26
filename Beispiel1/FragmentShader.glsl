// n0, u, v are normal, up, right vectors of image plane
// n0 points towards the origin
// the lengths of u, v equals the distance between horizontally resp. vertically neighboured pixels in
// world coordinates
//
// We use parallel projection: all view rays are parallel to n0

//
// Volume data is centered around the origin and scaled such that the longest edge has a length of 1.

// It follows that the bounding sphere of the volume data has a radius of at most sqrt(3).
// The view plane can therefore be placed at distance sqrt(3) from the origin, and the view rays need
// only be sampled in the interval [0, 2*sqrt(3)].

// Given an equidistant sampling along the ray with N samples, the sample locations are
// s_i = center + u * x + v * y + i * 2 * sqrt(3) / (N-1), with 0 <= i < N
// where center is the center of the image plane, center = - n0 * sqrt(3)
// x and y are pixel locations of the view ray
//

uniform vec3 n0, u, v;
uniform vec3 volumeSize; // size of volume data
uniform vec3 volumeResolution; // number of sample points in volume data
uniform int N;
uniform sampler3D sampler;

void main(void)
{
    vec3 center = n0 * sqrt(3);
    vec3 sampleDistance = n0 * 2 * sqrt(3) / (N - 1);
    vec3 sampleLocation = center + u * gl_TexCoord[0].x + v * gl_TexCoord[0].y;

    // set background color
    gl_FragColor = vec4(0,0,0,1.f);

    // Render samples back-to-front
    for (int i = N-1; i >= 0; --i, sampleLocation -= sampleDistance)
    {

        // if sample location is outside volume, continue (opacity will be 0)
        vec3 volumeExtent = volumeSize * 0.5f;
        if (abs(sampleLocation.x) >= volumeExtent.x
            || abs(sampleLocation.y) >= volumeExtent.y
            || abs(sampleLocation.z) >= volumeExtent.z)
        {
            continue;
        }

        // interpolate density value
        vec3 tmp = sampleLocation + volumeExtent;
        vec3 tex3DCoord = vec3(tmp.x / volumeSize.x, tmp.y / volumeSize.y, tmp.z / volumeSize.z);

        vec4 density = texture3D(sampler, tex3DCoord);

        // calculate grid cell for sample location and relative location within cell.
        // for N data values along a unit axis there are N-1 grid cells.
        // points on boundary planes between grid cells always belong to the cell with the lower index.
        ivec3 gridCell;
        vec3 locationInCell;

        tmp = sampleLocation + volumeExtent;
        tmp *= volumeSize * volumeResolution;

        gridCell = ivec3(floor(tmp));
        locationInCell = tmp - vec3(gridCell);

        // TODO: calculate gradient/normal in cell
        vec3 sampleNormal; // estimated normal at sample point

        // TODO: map density value to color and opacity using transfer function
        vec4 sampleColor = vec4(1, 1, 1, density.a); // color and opacity of sample

        // TODO: calculate final fragment color with shading
               //

        // Blend
        gl_FragColor = vec4(sampleColor.a * sampleColor.rgb + (1 - sampleColor.a) * gl_FragColor.rgb, 1);
    }
}
