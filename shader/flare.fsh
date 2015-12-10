//flare.fsh
#version 120
uniform sampler2D screen;
varying vec2 texcoords;

float _threshold = 0.8;
int nsamples = 5;
float dispersal = 0.3;
float halo_width = 0.5;

vec2 flipTexcoords(vec2 texcoords) {
	return -texcoords + 1.0;
}

float _vignette(vec2 coords, float halo_width) {
	float dist = distance(coords, vec2(0.5,0.5));
	dist = smoothstep(halo_width - 0.5, halo_width, dist);
	return clamp(dist, 0.0, 1.0);
}

vec3 threshold(sampler2D tex, vec2 coords, float _threshold) {
	vec3 col = texture2D(tex, coords).rgb;
	float gain = 1.2;

	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lum = dot(col.rgb, luma);
	float thresh = max((lum-_threshold)*gain, 0.0);
	return mix(vec3(0.0), col, thresh);
}

vec3 distort(sampler2D tex, vec2 sample_center, vec2 direction, vec3 distortion) {
	vec3 col = vec3(0.0);

	col.r = threshold(tex, sample_center + direction * distortion.r, _threshold).r;
	col.g = threshold(tex, sample_center + direction * distortion.g, _threshold).g;
	col.b = threshold(tex, sample_center + direction * distortion.b, _threshold).b;

	return col;
}

vec4 lensFlare() {
	vec3 chroma_distortion = vec3(0.01, 0.03, 0.05) * 0.2;
	vec2 texcoord = clamp(texcoords, 0.002, 0.998);
	vec2 center = vec2(0.5);
	vec2 sample_vec = (center - texcoord) * dispersal;
	vec2 halo_vec = normalize(sample_vec) * halo_width;

	vec3 result = distort(screen, texcoord + halo_vec, halo_vec, chroma_distortion).rgb;
	result *= _vignette(texcoord, halo_width);

	for(int i = 0; i < nsamples; ++i) {
		vec2 offset = sample_vec * float(i);
		result += distort(screen, texcoord + offset, offset, chroma_distortion).rgb;
	}

   	return vec4(result, 1);
}


void main(void) {
	gl_FragColor = lensFlare();
	gl_FragColor.a = 1.0;
}
