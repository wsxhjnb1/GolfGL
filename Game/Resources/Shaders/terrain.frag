#version 330 core

layout(location = 0) out vec4 FragColor;

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	float texH;
} fragIn;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
}; 

uniform Light light;
uniform sampler2D texture1;
uniform sampler2D texture2;


/* Light calculations */

vec3 DirectionalLight(Light light, vec3 normal, vec3 textureColor)
{
	vec3 ambient = light.ambient * textureColor;

	vec3 toLight = normalize(-light.direction);

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor;

	return ambient + diffuse;
}

#define CONSTANT (1.0)
#define LINEAR (0.007)
#define QUADRATIC (0.0002)

vec3 PointLight(Light light, vec3 normal, vec3 textureColor)
{	
	vec3 toLight = normalize(light.position - fragIn.FragPos);

	float distanceToViewer = length(light.position - fragIn.FragPos);
	float attenuation = 1.0f / (CONSTANT + LINEAR*distanceToViewer + QUADRATIC*(distanceToViewer*distanceToViewer));

	vec3 ambient = light.ambient * textureColor * attenuation;

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor * attenuation;

	return ambient + diffuse;
}


void main()
{
	vec3 normal = normalize(fragIn.Normal);
	vec3 textureColor = mix(texture(texture2, fragIn.TexCoords), texture(texture1, fragIn.TexCoords), fragIn.texH).rgb;	
	
	// vec3 result = PointLight(light, normal, textureColor);	
	vec3 result = DirectionalLight(light, normal, textureColor);

	FragColor = vec4(result, 1.0f);
}

