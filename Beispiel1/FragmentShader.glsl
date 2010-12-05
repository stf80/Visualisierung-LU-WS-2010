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
uniform sampler1D transferSampler;

uniform vec4 c_light;
uniform vec4 c_ambient;
uniform vec4 c_diffuse;
uniform vec4 c_specular;
uniform float c_exponent;

uniform float k1, k2;

void main(void)
{
    vec3 center = n0 * sqrt(3.0);
    vec3 sampleDistance = n0 * 2.0 * sqrt(3.0) / float(N - 1);
    vec3 sampleLocation = center + u * gl_TexCoord[0].x + v * gl_TexCoord[0].y;

    // set background color
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Render samples back-to-front
    for (int i = N-1; i >= 0; --i, sampleLocation -= sampleDistance)
    {

        // if sample location is outside volume, continue (opacity will be 0)
        vec3 volumeExtent = volumeSize * 0.5;
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

        // calculate gradient/normal in cell
        float dx = texture3D(sampler, vec3(tex3DCoord.xyz + vec3(0.001, 0.0, 0.0))).a - texture3D(sampler, vec3(tex3DCoord.xyz - vec3(0.001, 0.0, 0.0))).a;
        float dy = texture3D(sampler, vec3(tex3DCoord.xyz + vec3(0.0, 0.001, 0.0))).a - texture3D(sampler, vec3(tex3DCoord.xyz - vec3(0.0, 0.001, 0.0))).a;
        float dz = texture3D(sampler, vec3(tex3DCoord.xyz + vec3(0.0, 0.0, 0.001))).a - texture3D(sampler, vec3(tex3DCoord.xyz - vec3(0.0, 0.0, 0.001))).a;
        vec3 N = normalize(vec3(dx, dy, dz)); // approx normal vector
        vec3 L = normalize(gl_LightSource[0].position.xyz - sampleLocation); // vector to light source
        vec3 V = normalize(n0); // vector to observer
        vec3 H = normalize(V + L);

        // map density value to color and opacity using transfer function
        vec4 transferColor = texture1D(transferSampler, density.a);
        vec4 sampleColor = transferColor;

        // calculate final fragment color with shading
        //float kDist = float(i)/float(N);
        //float kVal = k1 + k2*kDist;
        vec4 shadingColor = c_light*c_ambient + c_light*c_diffuse*max(dot(N,L), 0.0) + c_light*c_specular*max(pow(dot(N,H),c_exponent),0.0);

        vec4 finalColor = sampleColor * shadingColor;

        float alpha = transferColor.a * density.a;

        // Blend
        gl_FragColor = vec4(alpha * finalColor.rgb + (1.0 - alpha) * gl_FragColor.rgb, 1.0);
    }
}
