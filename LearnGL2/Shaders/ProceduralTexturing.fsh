const char *ProceduralTexturingFragmentShader = STRINGIFY
(

precision highp float;

varying highp vec2 v_texCoord;

void main()
{
    vec4 color0 = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 color1 = vec4(0.0, 1.0, 0.0, 1.0);
    
    int frequency = 2;

    /*vec2 tcmod = mod(v_texCoord * float(frequency), 1.0);
    
    if (tcmod.s < 0.5)
    {
        if (tcmod.t < 0.5)
            gl_FragColor = color1;
        else
            gl_FragColor = color0;
    }
    else
    {
        if (tcmod.t < 0.5)
            gl_FragColor = color0;
        else
            gl_FragColor = color1;
    }*/
    
    vec2 texCoord = mod(floor(v_texCoord * float(frequency * 2)), 2.0);
    float delta = abs(texCoord.x - texCoord.y);
    
    gl_FragColor = mix(color1, color0, delta);
}

);