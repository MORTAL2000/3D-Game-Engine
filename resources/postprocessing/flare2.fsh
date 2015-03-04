//flare2.fsh
#version 120
uniform sampler2D screen;
varying vec2 texcoords;

int uSamples = 2;
float uDispersal = 0.25;
float uHaloWidth = 0.5;
float uDistortion = 0.2;

vec4 textureDistorted(sampler2D tex, vec2 texcoord, vec2 direction, vec3 distortion)
{
      return vec4(
         texture(tex, texcoord + direction * distortion.r).r,
         texture(tex, texcoord + direction * distortion.g).g,
         texture(tex, texcoord + direction * distortion.b).b,
         1.0
      );
}

void main()
{
    vec2 texcoord = -texcoords + vec2(1.0);
    vec2 texelSize = 1.0 / vec2(textureSize(screen, 0));

    vec2 ghostVec = (vec2(0.5) - texcoord) * uDispersal;
    vec2 haloVec = normalize(ghostVec) * uHaloWidth;

    vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

    // sample ghosts:
    vec4 result = vec4(0.0);
    for (int i = 0; i < uSamples; ++i) {
        vec2 offset = fract(texcoord + ghostVec * float(i));

        float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
        weight = pow(1.0 - weight, 10.0);

        result += textureDistorted(screen, offset, normalize(ghostVec), distortion) * weight;
    }

    float weight = length(vec2(0.5) - fract(texcoord + haloVec)) / length(vec2(0.5));
    weight = pow(1.0 - weight, 5.0);
    result += textureDistorted(screen, fract(texcoord + haloVec), normalize(ghostVec), distortion) * weight;

    gl_FragColor = result;
}
