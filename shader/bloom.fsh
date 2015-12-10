//bloom.fsh
#version 120
uniform sampler2D renderedTexture;
varying vec2 texcoords;

int samples = 12;
float blur = 0.001;
float gain = 4.0;

void main(void) {
    float texr = 0.0;
    float texg = 0.0;
    float texb = 0.0;
    vec3 texblur = vec3(0.0); 
    vec3 tex = texture2D(renderedTexture, texcoords).rgb;
    float fsamples = float(samples);

    for(int i = -samples; i < samples; ++i) {
        float fi = float(i);

        texr += texture2D(renderedTexture, vec2(texcoords.x-(fi*blur) + (blur*4.0), texcoords.y)).r/fsamples;
        texg += texture2D(renderedTexture, vec2(texcoords.x-(fi*blur), texcoords.y)).g/fsamples;
        texb += texture2D(renderedTexture, vec2(texcoords.x-(fi*blur) - (blur*4.0), texcoords.y)).b/fsamples;             
        texblur += texture2D(renderedTexture, vec2(texcoords.x-(fi*blur), texcoords.y-(fi*blur))).rgb/fsamples;
        texblur += texture2D(renderedTexture, vec2(texcoords.x+(fi*blur), texcoords.y-(fi*blur))).rgb/fsamples;     
    };
          
    vec3 chroma = vec3(texr, texg, texb)-1.0;                 
    vec3 lum = (texblur/2.0+chroma); 
    vec3 stencil = max(lum , 0.0);         
      
    gl_FragColor = vec4(tex+stencil/gain, 1.0);
}