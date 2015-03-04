//ground.fsh
uniform sampler2D _texture;
uniform int textured;

//Shadow stuff
uniform sampler2D shadowMap;
uniform int hasShadowMap;
varying vec4 shadowCoord;

//Sky scattering stuff
uniform vec3 sunDirection;
uniform vec3 cameraPosition;
uniform float mieDirectionalG;

varying vec3 worldSpacePosition;
varying vec3 sunEGround;
varying vec4 fragmentNormal;
varying vec2 texcoord;

varying vec3 betaR;
varying vec3 betaM;

const float pi = 3.141592653589793238462643383279502884197169;
const float E = 1000.0;
const float cutoffAngle = pi/1.95;
const float steepness = 1.5;
const vec3 up = vec3(0, 1, 0);

vec2 rand(vec2 coord) {
    float noiseX = (fract(sin(dot(coord, vec2(12.9898,78.233))) * 43758.5453));
    float noiseY = (fract(sin(dot(coord, vec2(12.9898,78.233)*2.0)) * 43758.5453));
    return vec2(noiseX,noiseY);
}

float rayleighPhase(float cosTheta) {
	return (3.0 / (16.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (1.0f / (3.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (3.0 / 4.0) * (1.0 + pow(cosTheta, 2));
}

float hgPhase(float cosTheta, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2)) / pow(1.0 - 2.0*g*cosTheta + pow(g, 2), 1.5));
}

float sunIntensity(float zenithAngleCos) {
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

//Shadow stuff 2
float getShadowFactor(sampler2D map, vec4 coords)
{
	if(hasShadowMap == 0) return 1;

	float visibility = 1.0;
    float bias = 0.000488; // 1/2048

	if(texture2D(map, coords.xy).z < (coords.z-bias))
	visibility = 0.3;

	return visibility;
}

float compare(sampler2D map, vec4 coords)
{
	if(hasShadowMap == 0) return 1;

	float value = 0;
	float texel = 0.00024; //1/4096
	for(int x = -2; x <= 2; x++) {
		for(int y = -2; y <= 2; y++) {
			vec2 rnd = rand(coords.xy);
			vec2 _offset = (vec2(x,y)+rnd)*texel;
			vec4 offset = vec4(_offset, 0, 0);
			value += getShadowFactor(map, coords+offset);
		}
	}
	return value/25;
}

vec4 fog(vec4 color) {
	vec4 fog_color = vec4(1.0, 1.0, 1.0, 0.0);
	float fog_coord = (gl_FragCoord.z / gl_FragCoord.w) / 1000;
	float density = 2.0;
	float fog = fog_coord * density;

	return mix(fog_color, color, clamp(1.0-fog, 0.0, 1.0));
}

//Main computation
void main() {

	vec4 tex = vec4(1.0);
	if(textured > 0)
    {
		tex = texture2D(_texture, texcoord);
    	if(tex == vec4(1.0, 0.0, 1.0, 1.0) || tex.a == 0)
		discard;
	}

	float sunE = sunIntensity(dot(sunDirection, vec3(0, 1, 0)));
	vec3 normal = fragmentNormal.xyz;

	vec3 ambientIntensity = sunEGround * 0.07;
	vec3 diffuseColor = vec3(0.8, 0.8, 0.8);
	vec3 ambientTerm = diffuseColor * ambientIntensity;
	float diffuseFactor = max(dot(sunDirection, normal), 0);
	vec3 diffuseTerm = diffuseColor * diffuseFactor * sunEGround;

	float distance = length(worldSpacePosition - cameraPosition);
	vec3 Fex = exp(-((betaR + betaM) * distance));

	vec4 L0 = vec4(diffuseTerm + ambientTerm, 1);
	//L0 += sunE * 1000 * Fex;

	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);

	float rPhase = rayleighPhase(cosTheta);
	vec3 betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;

	vec3 Lin = vec3(sunIntensity(dot(sunDirection, up))) * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex);
	Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(1.0/2.2)), clamp(pow(1.0-dot(up, sunDirection),5.0), 0.0, 1.0));

	// composition
	vec4 color = L0 * vec4(Fex, 1) + vec4(Lin, 0);
	color.rgb *= 0.02;
	color.rgb += vec3(0.0,0.001,0.0025)*0.3;

	vec3 whiteScale = 1.0/Uncharted2Tonemap(vec3(W));
	vec3 curr = Uncharted2Tonemap(color.rgb);
	vec3 final = curr*whiteScale;

    float gammaFac = 1.0/2.2;
    vec3 retColor = pow(final, vec3(gammaFac));
    retColor = clamp(retColor.rgb, 0.0, 1.0);

    if(textured > 0) retColor *= tex;

    //shadow stuff
    float visibility = compare(shadowMap, shadowCoord);
    //float visibility = getShadowFactor(shadowMap, shadowCoord);

    vec4 fragment = vec4(retColor, 1.0) * visibility;

	//final apply
	gl_FragColor = fog(fragment);
	gl_FragColor.w = logLuminance(gl_FragColor);
}
