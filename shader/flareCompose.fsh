//flareCompose.fsh
#version 120

uniform sampler2D renderedTexture;
uniform sampler2D flareTexture;
uniform sampler2D dust;

uniform vec2 textureDimension;
varying vec2 texcoords;

vec4 blur2(sampler2D tex, vec2 coords, int blurSize) {
	vec4 result = vec4(0.0);
	vec2 texel = 1.0 / textureDimension;
	vec2 hlim = vec2(float(-blurSize) * 0.5 + 0.5);
	for(int x = 0; x < blurSize; x++) {
		for(int y = 0; y < blurSize; y++) {
			vec2 offset = (hlim + vec2(float(x), float(y))) * texel;
			result += texture2D(tex, coords+offset);
		}
	}
	return result / float(blurSize * blurSize);
}

vec4 blur3(sampler2D tex, vec2 coords, float blurSize) {
	float samples[10] =
   	float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);
	vec2 dir = 0.5 - coords;
	float distance = sqrt(dir.x*dir.x+dir.y*dir.y);
	dir = dir/distance;
	vec4 sum = texture2D(tex, coords);

	for(int i = 0; i < 10; i++) {
		sum += texture2D(tex, coords + dir * samples[i]);
	}
	sum *= 1/11.0;
	float t = distance * blurSize;
	t = clamp(t, 0.0, 1.0);
	return mix(texture2D(tex, coords), sum, t);
}

vec4 hejl(vec4 color) {
	vec4 x = max(vec4(0.0), color - vec4(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

float MAX_BLUR_RADIUS = 4096;
int blurRadius = 60;
float blurSigma = 65.2;

vec4 gauss1d(sampler2D srcTex, vec2 srcTexelSize, vec2 origin, int radius, vec2 direction)
{
	int nSamples = int(clamp(float(radius), 1.0, MAX_BLUR_RADIUS) * 0.5);
	if(nSamples == 0) return texture2D(srcTex, origin);

	float SIGMA = float(blurRadius) / 8.0;
	float sig2 = SIGMA * SIGMA;
	const float TWO_PI = 6.2831853071795;
	vec3 gaussInc;
	gaussInc.x = 1.0 / (sqrt(TWO_PI) * SIGMA);
	gaussInc.y = exp(-0.5 / sig2);
	gaussInc.z = gaussInc.y * gaussInc.y;

	vec4 result = texture2D(srcTex, origin) * gaussInc.x;
	for(int i = 1; i < nSamples; i++)
	{
		gaussInc.xy *= gaussInc.yz;
		vec2 offset = float(i) * direction * srcTexelSize;
		result += texture2D(srcTex, origin - offset) * gaussInc.x;
		result += texture2D(srcTex, origin + offset) * gaussInc.x;
	}
	return result;
}

// Crytek source, pass the worldSpacePosition of the vertex as parameter
/*float computeVolumetricFog(vec3 cameraToWorldPos) {

	float cHeightFalloff = 16;
	vec3 cViewPos = cameraPosition;
	float cGlobalDensity = 0.7;


	float cVolFogHeightDensityAtViewer = exp(-cHeightFalloff * cViewPos.z);
	float fogInt = length(cameraToWorldPos) * cVolFogHeightDensityAtViewer;
	const float cSlopeThreshold = 0.01;
	if(abs(cameraToWorldPos.z) > cSlopeThreshold) {
		float t = cHeightFalloff * cameraToWorldPos.z;
		fogInt *= (1.0 - exp(-t)) / t;
	}
	return exp(-cGlobalDensity * fogInt);
}*/

void main(void)
{
	vec4 color = texture2D(renderedTexture, texcoords);
	vec4 mod = texture2D(dust, texcoords);

	vec4 flare1 = gauss1d(flareTexture, vec2(1.0/1280.0, 1.0/720.0), texcoords, blurRadius, vec2(1.0, 0.0));
	vec4 flare2 = gauss1d(flareTexture, vec2(1.0/1280.0, 1.0/720.0), texcoords, blurRadius, vec2(-1.0, 0.0));
	vec4 flare = (flare1 + flare2) * 0.5;

	vec4 overlay = flare * mod * 8;
	//vec4 overlay = blur2(flareTexture, texcoords, 15) * mod * 8.0;
	//vec4 overlay = blur3(flareTexture, texcoords, 5.2) * mod * 8.0;

	gl_FragColor = color + overlay;
}
