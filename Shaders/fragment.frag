#version 330 core
 
out vec4 pixelColor;
in vec2 coord;
 
uniform sampler1D Pallet;

uniform float CrealOf;
uniform float CimagOf;
uniform int MaxIterations;
 
void main()
{
    float   real  = coord.x;
    float   imag  = coord.y;
 
    float   Creal = real;  
    float   Cimag = imag;  
 
    float r2 = 0.0;
 
    float iter;
 
    for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
    {
        float tempreal = real;
 
        real = (tempreal * tempreal) - (imag * imag) + Creal;
        imag = 2.0 * tempreal * imag + Cimag;
        r2   = (real * real) + (imag * imag);
    }
 
    vec4 color;
 
    highp int iterint = int(iter);
 
    //color.y = (iter == MaxIterations ? 0.0 : iter) / 100.0;
    //color.xz = vec2(0.0f);
    color =  texture1D(Pallet, (iter == MaxIterations ? 0.0 : iter) / 100.0);
 
    pixelColor = vec4(color.xyz, 1.0);
}
