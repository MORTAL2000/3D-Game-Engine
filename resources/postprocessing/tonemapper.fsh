//tonemapper.fsh
#version 120
uniform sampler2D renderedTexture;
uniform vec2 inverseTextureSize;
varying vec2 texcoords;

vec4 tonemap(vec4 hdr) {
	float logLuminance = -2.3;
	float contrast = 7.0;
	float bias = 0.4;

	vec3 c = hdr.rgb;
	float low = exp(logLuminance - bias - contrast * 0.5);
	float high = exp(logLuminance - bias + contrast * 0.5);

	vec3 ldr = (hdr.rgb - low) / (high - low);
	return vec4(ldr, hdr.a);
}

vec4 hejl(vec4 color) {
	vec4 x = max(vec4(0.0), color - vec4(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

float dither(float v, float r)
{
	float val = v * 255.0;
	float ival = floor(val);
	float diff = val - ival;
	return (ival + float(r < diff)) / 255.0;
}

vec3 dither3(vec3 v, vec2 coord)
{
	float r = (fract(sin(dot(coord, vec2(12.9898,78.233))) * 43758.5453));
 	return vec3(dither(v.x, r), dither(v.y, r), dither(v.z, r));
}

vec3 ColorGrade(vec3 color)
{
	vec3 c = color.rgb;

	//warm
	c.rgb = max(vec3(0.0f), c.rgb * 1.1f - 0.1f);

		c.r *= 7.0f;
		c.r /= c.r + 2.5f;

		c.g = c.g;

		c.b *= 0.7f;

	// //cool

		// 	c.r *= 1.0f;
		// 	c.g *= 1.2f;
		// 	c.b *= 1.5f;

	return c.rgb;
}

vec3 Vignette2(vec3 color)
{
	float dist = distance(texcoords.xy, vec2(0.5f)) * 2.0f;
	dist /= 1.5142f;
	dist = pow(dist, 1.1f);
	color.rgb *= 1.0f - dist;
	return color;
}

void main(void) {
	vec4 color = texture2D(renderedTexture, texcoords);

	//FXAA
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MIN = 1.0/128.0;
	float FXAA_REDUCE_MUL = 1.0/8.0;

	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaTL = dot(luma, texture2D(renderedTexture, texcoords.xy + (vec2(-1.0, -1.0) * inverseTextureSize.xy)).xyz);
	float lumaTR = dot(luma, texture2D(renderedTexture, texcoords.xy + (vec2(1.0, -1.0) * inverseTextureSize.xy)).xyz);
	float lumaBL = dot(luma, texture2D(renderedTexture, texcoords.xy + (vec2(-1.0, 1.0) * inverseTextureSize.xy)).xyz);
	float lumaBR = dot(luma, texture2D(renderedTexture, texcoords.xy + (vec2(1.0, 1.0) * inverseTextureSize.xy)).xyz);
	float lumaM  = dot(luma, texture2D(renderedTexture, texcoords.xy).xyz);
	vec2 direction = vec2(-((lumaTL + lumaTR) - (lumaBL + lumaBR)), (lumaTL + lumaBL) - (lumaTR + lumaBR));
	float reduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (FXAA_REDUCE_MUL * 0.25), FXAA_REDUCE_MIN);
	float temp = 1.0/(min(abs(direction.x), abs(direction.y)) + reduce);
	direction = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), direction * temp)) * inverseTextureSize.xy;

	vec3 result1 = 0.5 * (texture2D(renderedTexture, texcoords.xy + direction * vec2(1.0/3.0 - 0.5)).xyz +
						   texture2D(renderedTexture, texcoords.xy + direction * vec2(2.0/3.0 - 0.5)).xyz);
	vec3 result2 = 0.5 * result1 + 0.25 * (texture2D(renderedTexture, texcoords.xy + direction * vec2(0.0/3.0 - 0.5)).xyz +
								           texture2D(renderedTexture, texcoords.xy + direction * vec2(3.0/3.0 - 0.5)).xyz);
	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		color = vec4(result1, 1.0);
	else
		color = vec4(result2, 1.0);

	// vignetting and tonemapping
	float attenuation = 5.0;
	float power = 4.7;
	float delta = distance(texcoords, vec2(0.5, 0.5));
	float darkening = 1.0 - pow(delta, power) * attenuation;
	color = color * vec4(vec3(darkening), 1.0);
	//color.rgb = Vignette2(color.rgb);

	//remove banding artifacts
	color.rgb = dither3(color.rgb, texcoords.xy);

	gl_FragColor = tonemap(color);
}
