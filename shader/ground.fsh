//ground.fsh
#version 120
uniform sampler2D _texture;
uniform int textured;

uniform sampler2D normalMap;
uniform int hasNormalMap;

//shadow stuff
uniform sampler2D shadowMap;
uniform int hasShadowMap;

//sky scattering stuff
uniform vec3 cameraPosition;
uniform vec3 sunDirection;
uniform float mieDirectionalG;

//variables that are passed to fragment shader
varying vec4 shadowCoord;
varying vec3 worldSpacePosition;
varying vec3 sunEGround;
varying vec3 fragmentNormal;
varying vec2 texcoord;

varying vec3 betaR;
varying vec3 betaM;
varying mat3 tbn;

//constants
const float pi = 3.141592653589793238462643383279502884197169;
const float pi_over_two = 1.57079632679;
const float pi_over_four = 0.78539816339;

const float E = 1000.0;
const float cutoffAngle = pi/1.95;
const float steepness = 1.5;
const vec3 up = vec3(0.0, 1.0, 0.0);

//old random
//vec2 rand(vec2 coord) {
//    float noiseX = (fract(sin(dot(coord, vec2(12.9898,78.233))) * 43758.5453));
//    float noiseY = (fract(sin(dot(coord, vec2(12.9898,78.233)*2.0)) * 43758.5453));
//    return vec2(noiseX,noiseY);
//}
//remove banding artifacts (often seen by spotlights)
//float dither(float v, float r) {
//    float val = v * 255.0;
//    float ival = floor(val);
//    float diff = val - ival;
//    return (ival + float(r < diff)) / 255.0;
//}
//
//vec3 dither3(vec3 v, vec2 coord) {
//    float r = rand_one(coord);
//    return vec3(dither(v.x, r), dither(v.y, r), dither(v.z, r));
//}

//random values
float rand_one(vec2 co) {
    float a = 12.9898;
    float b = 78.233;
    float c = 43758.5453;
    float dt = dot(co, vec2(a,b));
    float sn = mod(dt, pi);
    return fract(sin(sn) * c);
}

vec2 rand(vec2 coord) {
	float r1 = rand_one(coord);
	float r2 = rand_one(coord+vec2(1.0));
	return vec2(r1, r2);
}

//light scattering functions
float rayleighPhase(float cosTheta) {
//	return (3.0 / (16.0*pi)) * (1.0 + pow(cosTheta, 2.0));
//	return (1.0f / (3.0*pi)) * (1.0 + pow(cosTheta, 2));
	return (3.0 / 4.0) * (1.0 + pow(cosTheta, 2.0));
}

float hgPhase(float cosTheta, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0*g*cosTheta + pow(g, 2.0), 1.5));
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

float linearStep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}

//Shadow stuff 2
float sampleBasicShadowMap(sampler2D map, vec2 coords, float compare) {
	float bias = 0.001488;
	return max(step(compare-bias, texture2D(map, coords.xy).r), 0.3);
}

float sampleVarianceShadowMap(sampler2D map, vec2 coords, float compare) {
	float depth = texture2D(map, coords.xy).r;

	float dx = dFdx(depth);
	float dy = dFdy(depth);

	float moments2 = depth * depth + 0.25 * (dx * dx + dy * dy);
	vec2 moments = vec2(depth, moments2);

	float p = step(compare, moments.x); // factor in range [0,1]
	float variance = max(moments.y - moments.x * moments.x, 0.0002);
	float d = compare - moments.x;
	float pMax = linearStep(0.5, 1.0, variance / (variance + d*d));

	float value = min(max(p, pMax), 1.0);
	return max(value, 0.3);
}

float sampleVarianceShadowMapFiltered(sampler2D map, vec2 coords, float compare) {
	int samples = 2;
	float factor = float(samples)*2.0;
	factor *= factor;

	float value = 0.0;
	float texel = 0.0002;
	for(int x = -samples; x < samples; x++) {
		for(int y = -samples; y < samples; y++) {
			vec2 rnd = rand(coords.xy);
			vec2 offset = (vec2(x,y)+rnd)*texel;
			value += sampleVarianceShadowMap(map, coords+offset, compare);
		}
	}
	return value/factor;
}

float getShadowFactor(sampler2D map, vec4 coords) {
	if(hasShadowMap == 0) return 1.0;

	vec3 shadowMapCoords = (coords.xyz / coords.w);
	float visibility = sampleVarianceShadowMapFiltered(map, coords.xy, coords.z);

	return visibility;
}

/////////////////////////

vec4 fog(vec4 color) {
	vec4 fog_color = vec4(0, 0, 0, 0);
	float fog_coord = (gl_FragCoord.z / gl_FragCoord.w) / 10000.0;
	float density = 2.0;
	float fog = fog_coord * density;

	return mix(fog_color, color, clamp(1.0-fog, 0.0, 1.0));
}

//Main computation
void main(void)
{
	vec4 tex = vec4(1.0);
	vec3 normal = fragmentNormal.xyz;
	if(textured > 0) {
		tex = texture2D(_texture, texcoord);
    	if(tex == vec4(1.0, 0.0, 1.0, 1.0) || tex.a == 0.0)
		discard;
	}

    if(hasNormalMap > 0)
    {
        normal = normalize(texture2D(normalMap, texcoord).rgb * 2.0 - 1.0);
        normal = normalize(normal * tbn);
    }

	float zenith = dot(sunDirection, up);
	float sunE = sunIntensity(zenith);

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//physically based rendering

	// constants
	float specular_power = 1000.0;
	float ior = 1.2;
	vec3 specular_color = vec3(0.9);
	vec3 diffuseColor = vec3(0.8);

	// compute vectors
	vec3 lightVector = sunDirection;
	vec3 viewVector = normalize(cameraPosition - worldSpacePosition);
	vec3 halfVector = normalize(lightVector + viewVector);
	float n_dot_l = dot(normal, lightVector);
	float h_dot_l = max(dot(halfVector, sunDirection), 0.0);
	float n_dot_h = max(dot(normal, halfVector), 0.0);
	float n_dot_v = max(dot(normal, viewVector), 0.0);

	// ambient term
	vec3 ambientIntensity = sunEGround * 0.17;
	vec3 ambient_term = diffuseColor * ambientIntensity;

	// diffuse term
	float cosine_term = max(n_dot_l, 0.0);

#define LAMBERTIAN_DIFFUSE
#ifdef LAMBERTIAN_DIFFUSE
	vec3 diffuse_term = diffuseColor * cosine_term * sunEGround;
#else
	//Oren-nayar
	float roughness = 3.1;
	float cos_phi_diff = dot(normalize(viewVector - normal * n_dot_v), normalize(lightVector - normal * cosine_term));
	float cos_alpha = min(cosine_term, n_dot_v);
	float cos_beta = max(cosine_term, n_dot_v);
	float r2 = roughness * roughness;
	float a = 1.0f - 0.5 * r2 / (r2 + 0.33);
	float b_term;
	if(cos_phi_diff >= 0.0) {
		float b = 0.45 * r2 / (r2 + 0.09);
		b_term = b * sqrt((1.0 - cos_alpha * cos_alpha) * (1.0 - cos_beta * cos_beta)) / cos_beta * cos_phi_diff;
	}
	else b_term = 0.0;
	float diffuseFactor = cosine_term * (a+b_term);
	vec3 diffuse_term = diffuseColor * diffuseFactor * sunEGround;
#endif

#define USE_SPECULAR
#ifdef USE_SPECULAR

	// specular term
	float normalization_term = (specular_power + 2.0) / 8.0;
	float blinn_phong = pow(n_dot_h, specular_power);
	float specular_term = normalization_term * blinn_phong;

	// fresnel term
	float n1 = 1.0;
	float n2 = ior;
	float R0 = pow((n1 - n2) / (n1 + n2), 2.0);
	float fresnel_term = R0 + (1.0 - R0) * pow(1.0 - h_dot_l, 5.0);

	//visibility term
	float alpha = 1.0 / (sqrt(pi_over_four * specular_power + pi_over_two));
	float visibility_term = (cosine_term * (1.0 - alpha) + alpha) * (n_dot_v * (1.0 - alpha) + alpha);
	visibility_term = 1.0 / visibility_term;

	vec3 specular = specular_term * cosine_term * (1.0/fresnel_term) * visibility_term * specular_color;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	// final apply
	vec4 texture_term = (textured > 0) ? tex : vec4(1.0);
	vec4 L0 = vec4(diffuse_term + ambient_term + specular, 1.0);
#else
    vec4 texture_term = (textured > 0) ? tex : vec4(1.0);
    vec4 L0 = vec4(diffuse_term + ambient_term, 1.0);
#endif

	// continue scattering
	float distance = length(worldSpacePosition - cameraPosition);
	vec3 Fex = exp(-((betaR + betaM) * distance));
	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);

	float rPhase = rayleighPhase(cosTheta);
	vec3 betaRTheta = betaR * rPhase;
	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;

	vec3 Lin = vec3(sunIntensity(dot(sunDirection, up))) * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex);
	Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(1.0/1.2)), clamp(pow(1.0-dot(up, sunDirection),5.0), 0.0, 1.0));

	//composition
	vec4 color = L0 * vec4(Fex, 1.0) + vec4(Lin, 0.0);
	color.rgb *= 0.005;
	//color.rgb += vec3(0.0,0.001,0.0025)*0.3;

	vec3 whiteScale = 1.0/Uncharted2Tonemap(vec3(W));
	vec3 curr = Uncharted2Tonemap(color.rgb);
	vec3 final = curr*whiteScale;
    float gammaFac = 1.0/1.2;
    vec3 retColor = pow(final, vec3(gammaFac));
    retColor = clamp(retColor.rgb, 0.0, 1.0);

    float visibility = getShadowFactor(shadowMap, shadowCoord);
    vec4 fragment = vec4(retColor, 1.0) * texture_term * visibility;

	gl_FragColor = fragment;
	gl_FragColor.w = logLuminance(fragment);
}
