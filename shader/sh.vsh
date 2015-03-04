//spherical harmonics
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 diffuse_color;

struct SHFactors
{
    vec3 L00;
    vec3 L1m1;
    vec3 L10;
    vec3 L11;
    vec3 L2m2;
    vec3 L2m1;
    vec3 L20;
    vec3 L21;
    vec3 L22;
};

SHFactors sh;

const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
 	vec3 tnorm = normalize(normalMatrix * normal);

	sh.L00 = vec3(2.58, 1.40, 1.74);
	sh.L1m1 = vec3(-1.26, -1.12, -1.94);
	sh.L10 = vec3(-1.08, -0.50, -0.85);
	sh.L11 = vec3(-0.93, -0.17, 0.02);
	sh.L2m2 = vec3(0.35, 0.16, 0.4);
	sh.L2m1 = vec3(0.83, 0.71, 1.51);
	sh.L20 = vec3(-0.50, -0.311, -0.48);
	sh.L21 = vec3(1.78, 0.68, 0.44);
	sh.L22 = vec3(0.67, -0.17, -0.98);
	
	diffuse_color = C1 * sh.L22 * (tnorm.x * tnorm.x - tnorm.y * tnorm.y) +
                    C3 * sh.L20 *  tnorm.z * tnorm.z +
                    C4 * sh.L00 -
                    C5 * sh.L20 +
                    2.0 * C1 * sh.L2m2 * tnorm.x * tnorm.y +
                    2.0 * C1 * sh.L21  * tnorm.x * tnorm.z +
                    2.0 * C1 * sh.L2m1 * tnorm.y * tnorm.z +
                    2.0 * C2 * sh.L11  * tnorm.x +
                 	2.0 * C2 * sh.L1m1 * tnorm.y +
                    2.0 * C2 * sh.L10  * tnorm.z;
}
