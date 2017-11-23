#version 130

in vec3 ex_Position;
in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 ex_ShadowCoord;

out vec4 out_Color;

uniform vec3 sunDirection;
uniform float ambientLight = 0.3f;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler;

void main(void)
{
	vec2 poissonDisk[16] = vec2[]( 
		vec2( -0.94201624, -0.39906216 ), 
		vec2( +0.94558609, -0.76890725 ), 
		vec2( -0.09418410, -0.92938870 ), 
		vec2( +0.34495938, +0.29387760 ), 
		vec2( -0.91588581, +0.45771432 ), 
		vec2( -0.81544232, -0.87912464 ), 
		vec2( -0.38277543, +0.27676845 ), 
		vec2( +0.97484398, +0.75648379 ), 
		vec2( +0.44323325, -0.97511554 ), 
		vec2( +0.53742981, -0.47373420 ), 
		vec2( -0.26496911, -0.41893023 ), 
		vec2( +0.79197514, +0.19090188 ), 
		vec2( -0.24188840, +0.99706507 ), 
		vec2( -0.81409955, +0.91437590 ), 
		vec2( +0.19984126, +0.78641367 ), 
		vec2( +0.14383161, -0.14100790 ) 
	);

	float shadowVisibility = 1.0f;

	for(int i = 0; i < 16; i++)
	{
		if(texture(shadowSampler, ex_ShadowCoord.xy + poissonDisk[i] / 500.0f).z  <  ex_ShadowCoord.z + 0.003f)
		{
			shadowVisibility -= 0.02f;
		}
	}

    float sunDiffuse = max(dot(ex_Normal, sunDirection), 0.0f);
	vec3 materialColour = vec3(texture(textureSampler, ex_TexCoord));

	float finalLightingAmount = min(sunDiffuse + ambientLight, 1.0f) * shadowVisibility;

    out_Color = vec4(materialColour * finalLightingAmount, 1.0f);
}