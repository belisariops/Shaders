#version 130
//varying vec4 vColor;
//Multicolor attribute
in vec4 multiColor;
in float time;

//Final color
out vec4 fragColor;


// 2D Random
// Pseudo-random function from https://thebookofshaders.com/10/
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

vec3 noise (in vec2 position) {
    // Integer of the fragment positon to get the "cell" that the fragment is in
    vec2 i = floor(position);

    // Fractional part of the position, represents the position within the "cell"
    vec2 f = fract(position);

    // Get random values for each of the "vertex" of the cell that the fragment its in
    float a = random(i + vec2(-1.0,-1.0));
    float b = random(i + vec2(1.0, -1.0));
    float c = random(i + vec2(1.0, 1.0));
    float d = random(i + vec2(-1.0, 1.0));

    // Polinomical curve for interpolation
    vec2 curve = f*f*(3.0-2.0*f);

    // Linearly interpolate between a and c, with the third been the value to use for the interpolation
    float n =  mix(a,c,curve.x +
            (c - a)* curve.y * (1.0 - curve.x) +
            (d - b) * curve.x * curve.y);
    return vec3(n);
}



void main (void)
{
   fragColor = multiColor;

   //Resolution of the noise
   float scaleValue = 4.0;

   //Scale position of the fragment
   vec2 scalePosition = vec2(gl_FragCoord.xy*scaleValue);

   // Calculate noise function
   vec3 noise = noise(scalePosition);

   //Color that is given as output to the color buffer
   fragColor = vec4(noise, 1.0);



}