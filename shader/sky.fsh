#version 120

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

varying vec3 worldSpacePosition;
varying vec2 texcoords;

uniform vec3 cameraPosition;
uniform vec3 sunDirection;
uniform float rayleighCoefficient;
uniform float mieCoefficient;
uniform float mieDirectionalG;
uniform float turbidity;
uniform float delta;

const float far = 1000;
const float near = 0.1;

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
	return (8.0 * pow(pi, 3.0) * pow(pow(n, 2.0) - 1.0, 2.0) * (6.0 + 3.0 * pn)) / (3.0 * N * pow(lambda, vec3(4.0)) * (6.0 - 7.0 * pn));
}

float rayleighPhase(float cosTheta)
{
//	return (3.0 / (16.0*pi)) * (1.0 + pow(cosTheta, 2));
//	return (1.0f / (3.0*pi)) * (1.0 + pow(cosTheta, 2));
	return (3.0 / 4.0) * (1.0 + pow(cosTheta, 2));
}

vec3 totalMie(vec3 lambda, vec3 K, float T)
{
	float c = (0.2 * T ) * 10E-18;
	return 0.434 * c * pi * pow((2.0 * pi) / lambda, vec3(v - 2.0)) * K;
}

float hgPhase(float cosTheta, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0*g*cosTheta + pow(g, 2.0), 1.5));
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


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v) {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
}

float getCoverage(float coverage, float density, float clouds)
{
	float cloud = clamp(clouds - (1.0f - coverage), 0.0f, 1.0f -density) / (1.0f - density);
		cloud = max(0.0f, clouds * 1.1f - 0.1f);
	return cloud;
}

vec4 getCloudSpacePosition(vec2 coord, float depth, float distanceMult)
{
	float linDepth = depth;
	float expDepth = (far * (linDepth - near)) / (linDepth * (far - near));
	mat4 projInverse = 1 / projectionMatrix;
	mat4 modelViewInverse = 1 / modelMatrix;

	vec4 viewPos = projInverse * vec4(coord.s * 2.0f - 1.0f, coord.t * 2.0f - 1.0f, 2.0f * expDepth - 1.0f, 1.0f);
	viewPos /= viewPos.w;
	vec4 worldPos = modelViewInverse * viewPos;

	worldPos.xyz *= distanceMult;
	worldPos.xyz += cameraPosition.xyz;
	return worldPos;
}

float calculateSunglow()
{
	vec3 npos = normalize(worldSpacePosition.xyz);
	vec3 halfVector2 = normalize(-sunDirection + npos);
	float factor = 1.0f - dot(halfVector2, npos);
	return factor * factor * factor * factor;
}

vec4 clouds(vec3 color, vec3 worldPosition)
{
	float cloudHeight = 470.0f;
	float cloudDepth  = 1060.0f;
	float cloudUpperHeight = cloudHeight + (cloudDepth / 2.0f);
	float cloudLowerHeight = cloudHeight - (cloudDepth / 2.0f);

	if (worldPosition.y < cloudLowerHeight || worldPosition.y > cloudUpperHeight)
		return vec4(0.0);

	vec3 p = worldPosition.xyz / 150.0f;

	float t = delta;
	p.x -= t * 0.01f;

	vec3 p1 = p * vec3(1.0f, 0.5f, 1.0f)  + vec3(0.0f, t * 0.01f, 0.0f);
	float noise  =  snoise(p * vec3(1.0f, 0.5f, 1.0f) + vec3(0.0f, t * 0.01f, 0.0f));	p *= 3.0f;	p.x -= t * 0.057f;	vec3 p2 = p;
		  noise += (1.0f - abs(snoise(p) * 1.0f - 0.5f)) * 0.2f;				p *= 3.0f;	p.xz -= t * 0.035f;	vec3 p3 = p;
		  noise += (1.0f - abs(snoise(p) * 3.0f - 1.0f)) * 0.045f;				p *= 3.0f;	p.xz -= t * 0.035f;
		  noise += (1.0f - abs(snoise(p) * 3.0f - 1.0f)) * 0.015f;
		  noise /= 1.475f;

	float lightOffset = 0.45f;
	float heightGradient = clamp(( - (cloudLowerHeight - worldPosition.y) / (cloudDepth * 1.0f)), 0.0f, 1.0f);
	float heightGradient2 = clamp(( - (cloudLowerHeight - (worldPosition.y + sunDirection.y * lightOffset * 30.0f)) / (cloudDepth * 1.0f)), 0.0f, 1.0f);

	float cloudAltitudeWeight = 1.0f - clamp(distance(worldPosition.y, cloudHeight) / (cloudDepth / 2.0f), 0.0f, 1.0f);
		cloudAltitudeWeight *= 1.0f - heightGradient;

	float cloudAltitudeWeight2 = 1.0f - clamp(distance(worldPosition.y + sunDirection.y * lightOffset * 30.0f, cloudHeight) / (cloudDepth / 2.0f), 0.0f, 1.0f);
		cloudAltitudeWeight2 *= 1.0f - heightGradient2;

	noise *= cloudAltitudeWeight;

	float density = 0.8f;
	float coverage = 0.39f;
		coverage = mix(coverage, 0.77f, 0.0);
	float dist = length(worldPosition.xz - cameraPosition.xz);
		  coverage *= max(0.0f, 1.0f - dist / 40000.0f);
	noise = getCoverage(coverage, density, noise);

	float sundiff = snoise(p1 + sunDirection.xyz * lightOffset);
			  sundiff += (1.0f - abs(snoise(p2 + sunDirection.xyz * lightOffset) * 1.0f - 0.5f)) * 0.4f;
			  sundiff *= 1.1f;
			  sundiff *= cloudAltitudeWeight2;
			  sundiff = -getCoverage(coverage * 1.0f, 0.0f, sundiff);
	float firstOrder = pow(clamp(sundiff * 1.0f + 1.15f, 0.0f, 1.0f), 56.0f);
	float secondOrder = pow(clamp(sundiff * 1.0f + 1.1f, 0.0f, 1.0f), 11.0f);
	float directLightFalloff = mix(firstOrder, secondOrder, 0.5);

	float sunglow = calculateSunglow();
	float anisoBackFactor = mix(clamp(pow(noise, 1.6f) * 2.5f, 0.0f, 1.0f), 1.0f, pow(sunglow, 1.0f));
		directLightFalloff *= anisoBackFactor;
		directLightFalloff *= mix(11.5f, 1.0f, pow(sunglow, 0.5f));

	vec3 cloudColor = vec3(0.9);

	vec3 color0 = mix(color, cloudColor, vec3(min(1.0f, directLightFalloff)));
	vec4 result = vec4(color0.rgb, noise * heightGradient2);
	return result;
}

vec3 calculateClouds(vec3 color)
{
	float cloudDensity = 0.6f;

	float startingRayDepth = far - 5.0f;
	float rayDepth = startingRayDepth;
	float rayIncrement = far / 20.0f;
	float cloudDistanceMult = 400.0f / far;

	while(rayDepth > 0.0f)
	{
		//vec4 rayPosition = getCloudSpacePosition(texcoords.xy, rayDepth, cloudDistanceMult);
		vec3 rayPosition = worldSpacePosition;
		vec4 proximity = clouds(color, rayPosition.xyz);
		proximity.a *= cloudDensity;

		color.rgb = mix(color.rgb, proximity.rgb, vec3(min(1.0f, proximity.a * cloudDensity)));
		rayDepth -= rayIncrement;
	}

	return color.rgb;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

void main (void) {
	//float sunfade = 1.0-clamp(1.0-exp(-(sunDirectionection.z/500.0)),0.0,1.0);
    //rayleighCoefficient = rayleighCoefficient - (1.0* (1.0-sunfade));
	float sunE = sunIntensity(dot(sunDirection, vec3(0.0, 1.0, 0.0)));

	//extinction
	//rayleigh coefficients
	vec3 betaR = totalRayleigh(lambda) * rayleighCoefficient;

	//mie coefficient
	vec3 betaM = totalMie(lambda, K, turbidity) * mieCoefficient;

	//optical length
	//cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos(max(0.0, dot(up, normalize(worldSpacePosition - vec3(0.0, 0.0, 0.0)))));
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

	vec3 Lin = pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex), vec3(1.2));
    Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(1.0/1.6)), clamp(pow(1.0-dot(up, sunDirection),5.0), 0.0, 1.0));

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

    vec3 texColor = (Lin+L0);
    texColor *= 0.002;
    // texColor += vec3(0.0,0.001,0.0025)*0.3;

    vec3 curr = Uncharted2Tonemap(texColor);
	vec3 whiteScale = 1.0/Uncharted2Tonemap(vec3(W));
    vec3 color = curr*whiteScale;
    float gammaFac = 1.0/1.2;
    vec3 retColor = pow(color, vec3(gammaFac));
	clamp(retColor, 0.0, 1.0);

	//DISABLE IF TO BEASTY
	//retColor = calculateClouds(retColor);

	gl_FragColor.rgb = retColor;
	gl_FragColor.w = logLuminance(gl_FragColor);
}
