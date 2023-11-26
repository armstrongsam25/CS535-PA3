#version 430

in vec3 varyingNormal, varyingLightDir, varyingLight1Dir, varyingVertPos, varyingHalfVec, varyingHalfVec1;
in vec4 shadow_coord;
out vec4 fragColor;
 
struct PositionalLight
{	vec4 ambient, diffuse, specular;
	vec3 position;
};

struct Material
{	vec4 ambient, diffuse, specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform PositionalLight light1;
uniform Material material;
uniform mat4 mv_matrix; 
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;
layout (binding=0) uniform sampler2DShadow shadowTex;

float lookup(float x, float y)
{  	float t = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
	return t;
}

void main(void)
{	float shadowFactor=0.0;
	
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 H = normalize(varyingHalfVec);

	vec3 L1 = normalize(varyingLight1Dir);
	vec3 N1 = normalize(varyingNormal);
	vec3 V1 = normalize(-varyingVertPos);
	vec3 H1 = normalize(varyingHalfVec1);

	float swidth = 2.5;
	vec2 o = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
	shadowFactor += lookup(-1.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup(-1.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor = shadowFactor / 5.0;

	vec4 shadowColor = globalAmbient * (material.ambient
				+ light.ambient + light1.ambient) * material.ambient;
	
	vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L,N),0.0)
				+ light.specular * material.specular
				* pow(max(dot(H,N),0.0),material.shininess*3.0);
	
	vec4 lightedColor1 = light1.diffuse * material.diffuse * max(dot(L1,N1),0.0)
				+ light1.specular * material.specular
				* pow(max(dot(H1,N1),0.0),material.shininess);
	
	fragColor = vec4((shadowColor.xyz + shadowFactor*(lightedColor.xyz + lightedColor1.xyz)),1.0);
}
