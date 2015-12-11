//ground.vsh
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform vec3 sunDirection;

//Shadow stuff
uniform mat4 depthBiasMVP;
varying vec4 shadowCoord;

//Sky scattering stuff
uniform float rayleighCoefficient;
uniform float mieCoefficient;
uniform float mieDirectionalG;
uniform float turbidity;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 worldSpacePosition;
varying vec3 sunEGround;
varying vec4 fragmentNormal;
varying vec2 texcoord;

varying vec3 betaR;
varying vec3 betaM;

// scattering functions
// only
const float E = 1000.0;
const float pi = 3.141592653589793238462643383279502884197169;
const vec3 lambda = vec3(680E-9, 550E-9, 450E-9);
const vec3 K = vec3(0.686, 0.678, 0.666);
const vec3 up = vec3(0, 1, 0);
const float rayleighZenithLength = 8.4E3;
const float mieZenithLength = 1.25E3;
const float cutoffAngle = pi/1.95;
const float steepness = 1.5;

const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at
						  // 288.15K and 1013mb (sea level -45 celsius)

const float pn = 0.035;	// depolatization factor for standard air
const float v = 4.0;

vec3 totalRayleigh(vec3 lambda) {
	return (8.0 * pow(pi, 3.0) * pow(pow(n, 2.0) - 1.0, 2.0) * (6.0 + 3.0 * pn)) / (3.0 * N * pow(lambda, vec3(4.0)) * (6.0 - 7.0 * pn));
}

float rayleighPhase(float cosTheta) {
	return (3.0 / (16.0*pi)) * (1.0 + pow(cosTheta, 2.0));
//	return (1.0 / (3.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (3.0 / 4.0) * (1.0 + pow(cosTheta, 2));
}

vec3 totalMie(vec3 lambda, vec3 K, float T) {
	float c = (0.2 * T ) * 10E-18;
	return 0.434 * c * pi * pow((2.0 * pi) / lambda, vec3(v - 2.0)) * K;
}

float sunIntensity(float zenithAngleCos) {
	return E * max(0.0, 1.0 - exp(-((cutoffAngle - acos(zenithAngleCos))/steepness)));
}

float hgPhase(float cosTheta, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0*g*cosTheta + pow(g, 2.0), 1.5));
}

//main

void main(void) {
	//float sunfade = 1.0-clamp(1.0-exp(-(sunDirection.z/500.0)),0.0,1.0);
    //rayleighCoefficient = rayleighCoefficient - (1.0* (1.0-sunfade));
	float sunE = sunIntensity(dot(sunDirection, vec3(0, 1, 0)));

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
	worldSpacePosition = (modelMatrix * vec4(vertex, 1.0)).xyz;
	fragmentNormal = vec4(normal, 1.0);
	texcoord = uv;

	betaR = totalRayleigh(lambda) * rayleighCoefficient;
	betaM = totalMie(lambda, K, turbidity) * mieCoefficient;
	float sunAngle = acos(max(0.0, dot(up, sunDirection)));
	float sR = rayleighZenithLength / (cos(sunAngle) + 0.15 * pow(93.885 - ((sunAngle * 180.0) / pi), -1.253));
	float sM = mieZenithLength / (cos(sunAngle) + 0.15 * pow(93.885 - ((sunAngle * 180.0) / pi), -1.253));

	vec3 Fex = exp(-(betaR * sR + betaM * sM));
	float cosTheta = 1.0;

	float rPhase = rayleighPhase(cosTheta);
	vec3 betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;

	vec3 Lin = ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex);
	vec3 sunI = vec3(sunIntensity(dot(sunDirection, up)));

	sunEGround = sunI * Fex + sunI * Lin;

	//shadow stuff
	shadowCoord = depthBiasMVP * vec4(worldSpacePosition, 1.0);

	gl_FrontColor = gl_Color;
}
