//#version 440 core
//
//in vec3 outColor;
//in vec2 outTexCoord;
//
//out vec4 FragColor;
//
//uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform sampler2D texture2;
//
//uniform float smilePercentage;
//
//void main()
//{
//	vec4 finalColor0 = mix(texture(texture0, outTexCoord), texture(texture1, outTexCoord), smilePercentage);
//	vec4 finalColor1 = mix(texture(texture0, outTexCoord), texture(texture2, outTexCoord), smilePercentage);
//
//	FragColor = mix(finalColor0, finalColor1, 0.5);
//}

#version 440 core

uniform vec2 u_resolution;  // The resolution of the window
uniform vec2 u_center;      // The center of the Mandelbrot set
uniform float u_zoom;       // The zoom level
uniform int u_maxIterations;


out vec4 color;

void main() {
    // Calculate the pixel coordinates in the complex plane
    vec2 c = ((gl_FragCoord.xy / u_resolution) - 0.5) * u_zoom + u_center;

    // Set the initial value of z to 0
    vec2 z = vec2(0, 0);

    // Iterate over the Mandelbrot set
    int i;
    for (i = 0; i < u_maxIterations; i++) {
        // Calculate z = z^2 + c
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;

        // If z is outside the circle with radius 2, stop iterating
        if (dot(z, z) > 4.0) {
            break;
        }
    }

    if (i == u_maxIterations - 1)
    {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }

    else
    {
        color = vec4(float(i) / u_maxIterations, float(i) / u_maxIterations, float(i) / u_maxIterations, 1.0);
    }
}