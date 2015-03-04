//ssao.fsh
#version 120

//textures
uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;

//properties
uniform vec2 clipping;
uniform vec2 dimension;
uniform int samples; // 35
uniform int rings; //3

// passed from vertex shader
varying vec2 texcoords;

#define PI 3.14159265

float near = clipping.x;
float far = clipping.y;

vec2 rand(vec2 coord) {
    float noiseX = (fract(sin(dot(coord, vec2(12.9898,78.233))) * 43758.5453));
    float noiseY = (fract(sin(dot(coord, vec2(12.9898,78.233)*2.0)) * 43758.5453));
    return vec2(noiseX,noiseY)*0.004;
}

float readDepth(vec2 coord) {
    return (2.0 * near) / (far + near - texture2D(depthTexture, coord).x * (far-near));
}

float compareDepths(float depth1, float depth2) {
	float aoCap = 1.0;
	float aoMultiplier = 1000.00;
	float depthTolerance = 0.0;	
	float aorange = 60.0;
	float diff = sqrt(clamp(1.0-(depth1-depth2) / (aorange/(far-near)), 0.0, 1.0));
	float ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;
    return ao; //multiply by factor to increase visibility
}

float computeAO(vec2 coords, int samples, int rings) {
	float ao = 0.0;
	float aspect = float(dimension.x/dimension.y);
	vec2 texCoord = coords;
	vec2 noise = rand(texCoord);
	float depth = readDepth(texCoord);
	float w = (1.0 / float(dimension.x))/clamp(depth,0.05,1.0)+(noise.x*(1.0-noise.x));
    float h = (1.0 / float(dimension.y))/clamp(depth,0.05,1.0)+(noise.y*(1.0-noise.y));
    float fade = 1.0;
    float pw;
    float ph;
    float d;
    float s;
    for(int i = 0; i < rings; i+=1) {
    	fade *= 0.5;
    	for(int j = 0; j < samples * i; j+=1) {
    		float step = PI*2.0/(float(samples) * float(i));
    		pw = (cos(float(j)*step)*float(i));
            ph = (sin(float(j)*step)*float(i))*aspect;
            d = readDepth(vec2(texCoord.s+pw*w,texCoord.t+ph*h));
            ao += compareDepths(depth,d)*fade;
            s += 1.0*fade;
    	}
    }
    ao /= s;
    ao = 1.0-ao;
    return ao;
}

void main(void) {

    int _samples = samples == 0 ? 1 : samples;
    int _rings = rings == 0 ? 1 : rings;

	float ao = computeAO(texcoords, _samples, _rings);
	vec3 black = vec3(0.0, 0.0, 0.0);
	vec3 luminance = vec3(0.1, 0.1, 0.1);
	vec3 threshold = vec3(0.4, 0.4, 0.4);
	vec3 lum = clamp(max(black,luminance-threshold)+max(black,luminance-threshold)+max(black,luminance-threshold), 0.0, 1.0);

	vec4 color = texture2D(renderedTexture, texcoords);
	color = vec4(color.rgb * mix(vec3(ao,ao,ao), vec3(1.0,1.0,1.0), lum), 1.0);

	gl_FragColor = color;
}
