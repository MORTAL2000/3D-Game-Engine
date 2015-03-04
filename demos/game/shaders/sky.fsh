
varying vec3 worldSpacePosition;

uniform mat3 normalMatrix;
uniform vec3 cameraPosition;
uniform vec3 sunDirection;
uniform float rayleighCoefficient;
uniform float mieCoefficient;
uniform float mieDirectionalG;
uniform float turbidity;

// Constants for atmospheric scattering

const float e = 2.71828182845904523536028747135266249775724709369995957;
const float pi = 3.141592653589793238462643383279502884197169;

const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at
						  // 288.15K and 1013mb (sea level -45 celsius)

const float pn = 0.035;	// depolatization factor for standard air
const vec3 lambda = vec3(680E-9, 550E-9, 450E-9);

// mie stuff
// K coefficient for the primaries
const vec3 K = vec3(0.686, 0.678, 0.666);
const float v = 4.0;

// optical length at zenith for molecules
const float rayleighZenithLength = 8.4E3;
const float mieZenithLength = 1.25E3;
const vec3 up = vec3(0, 1, 0);

const float E = 1000.0;
const float sunAngularDiameterCos = 0.999956676946448443553574619906976478926848692873900859324;

// earth shadow hack
const float cutoffAngle = pi/1.95;
const float steepness = 1.5;

vec3 totalRayleigh(vec3 lambda)
{
	return (8.0 * pow(pi, 3.0) * pow(pow(n, 2.0) - 1.0, 2) * (6.0 + 3.0 * pn)) / (3.0 * N * pow(lambda, vec3(4.0)) * (6.0 - 7.0 * pn));
}

float rayleighPhase(float cosTheta)
{
	return (3.0f / (16.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (1.0f / (3.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (3.0 / 4.0) * (1.0 + pow(cosTheta, 2));
}

vec3 totalMie(vec3 lambda, vec3 K, float T)
{
	float c = (0.2 * T ) * 10E-18;
	return 0.434 * c * pi * pow((2 * pi) / lambda, vec3(v - 2)) * K;
}

float hgPhase(float cosTheta, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2)) / pow(1.0 - 2.0*g*cosTheta + pow(g, 2), 1.5));
}

float sunIntensity(float zenithAngleCos)
{
	return E * max(0.0, 1.0 - exp(-((cutoffAngle - acos(zenithAngleCos))/steepness)));
}

float logLuminance(vec4 c)
{
	return log(c.r * 0.2126 + c.g * 0.7152 + c.b * 0.0722);
}

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float EE = 0.02;
float F = 0.30;
float W = 1000.0;

vec3 Uncharted2Tonemap(vec3 x)
{
   return ((x*(A*x+C*B)+D*EE)/(x*(A*x+B)+D*F))-EE/F;
}

void main (void) {
	//float sunfade = 1.0-clamp(1.0-exp(-(sunDirection.z/500.0)),0.0,1.0);
    //rayleighCoefficient = rayleighCoefficient - (1.0* (1.0-sunfade));

	float sunE = sunIntensity(dot(sunDirection, vec3(0, 1, 0)));

	//extinction
	//rayleigh coefficients
	vec3 betaR = totalRayleigh(lambda) * rayleighCoefficient;

	//mie coefficient
	vec3 betaM = totalMie(lambda, K, turbidity) * mieCoefficient;

	//optical length
	//cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos(max(0, dot(up, normalize(worldSpacePosition - vec3(0, 0, 0)))));
	float sR = rayleighZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0) / pi), -1.253));
	float sM = mieZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0) / pi), -1.253));

	// combined extinction factor
	vec3 Fex = exp(-(betaR * sR + betaM * sM));

	//in scattering
	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);
	float rPhase = rayleighPhase(cosTheta*0.5+0.5);
	vec3 betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;

	//vec3 Lin = sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex;

	vec3 Lin = pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex), vec3(1.3));
    Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(1.0/2.0)), clamp(pow(1.0-dot(up, sunDirection),5.0), 0.0, 1.0));

	//nightsky
	vec3 direction = normalize(worldSpacePosition - cameraPosition);
	float theta = acos(direction.y);
	float phi = atan(direction.z, direction.x);
	vec2 uv = vec2(phi, theta);

	vec3 L0 = vec3(0.1) * Fex;

	//composition + solar disc
	//if(cosTheta > sunAngularDiameterCos)
	float sundisk = smoothstep(sunAngularDiameterCos, sunAngularDiameterCos+0.00002, cosTheta);
	L0 += sunE * 1000 * Fex * sundisk;

	vec3 whiteScale = 1.0/Uncharted2Tonemap(vec3(W));

    vec3 texColor = (Lin+L0);
    texColor *= 0.02;
    texColor += vec3(0.0,0.001,0.0025)*0.3;

    vec3 curr = Uncharted2Tonemap(texColor);
    vec3 color = curr*whiteScale;
	vec3 retColor = color;

	gl_FragColor.rgb = retColor;
	gl_FragColor.w = logLuminance(gl_FragColor);
}
