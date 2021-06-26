#version 330 core

/* Attributes */
// layout(location = 0) out vec4 FragColor;
out vec4 FragColor;

#define MAX_LIGHTS 10

/* Frag input */
in VERT_OUT
{	
	vec2 TexCoords;	
	float texH;
	flat int numLights;

	// Tangent-space
	vec3 FragPos;
	vec3 LightPos[MAX_LIGHTS];
	vec3 LightDir[MAX_LIGHTS];
	vec3 ViewPos;
} fragIn;

/* Objects */
// light types
const uint DIRECTIONAL = 0u;
const uint POINT	   = 1u;
const uint SPOT		   = 2u;
const uint DISABLED	   = 3u;
const float shininess  = 32.f;

struct Light
{	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
	vec3 color;	
	uint type;

	float linear;
	float quadratic;

	// Spot light	
	float cutoff;
	float outerCutoff;
}; 

struct Texture
{
	sampler2D diffuse;
	sampler2D normal;
};

// Uniforms
uniform Light light[MAX_LIGHTS];
uniform Texture texture1; // grass
uniform Texture texture2; // mud


/* Light calculations */

vec3 DirectionalLight(Light light, int i, vec3 normal, vec3 textureColor)
{
	vec3 result = light.ambient * textureColor;

	// vec3 toLight = normalize(fragIn.LightPos[i] - fragIn.FragPos);
	vec3 toLight = normalize(-fragIn.LightPos[i]);
	float diffuseFactor = max(dot(toLight, normal), 0.0);
	result += light.diffuse * diffuseFactor * textureColor;

	vec3 halfwayDir = normalize(toLight + normalize(fragIn.ViewPos - fragIn.FragPos));
	float spec = pow(max(dot(normal, halfwayDir), 0.f), shininess);
	result += spec * light.specular;

	return result;
}


vec3 PointLight(Light light, int i, vec3 normal, vec3 textureColor)
{	
	vec3 toLight = normalize(fragIn.LightPos[i] - fragIn.FragPos);

	float distanceToViewer = length(fragIn.LightPos[i] - fragIn.FragPos);
	float attenuation = 1.f / (1.f + light.linear*distanceToViewer + light.quadratic*(distanceToViewer*distanceToViewer));
	

	vec3 ambient = light.ambient * textureColor * attenuation;

	float diffuseFactor = max(dot(toLight, normal), 0.f);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor * attenuation;

	vec3 viewDir = normalize(fragIn.ViewPos - fragIn.FragPos);	

	vec3 halfwayDir = normalize(toLight + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.f), shininess);

	return ambient + diffuse + spec * light.specular * light.color;
}

#define outerCutoff(theta) theta
vec3 SpotLight(Light light, int i, vec3 normal, vec3 textureColor)
{	
	vec3 toLight = normalize(fragIn.LightPos[i] - fragIn.FragPos);	
	float theta = dot(-toLight, fragIn.LightDir[i]);

	vec3 result = light.ambient * textureColor; // ambient	
	
	// diffuse
    float diff = max(dot(normal, toLight), 0.0);
    vec3 diffuse = light.diffuse * diff * textureColor;
        
    // specular
    vec3 viewDir = normalize(fragIn.ViewPos - fragIn.FragPos);
    vec3 halfwayDir = normalize(toLight + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * textureColor;
        
    // smoothing
    float d = length(fragIn.LightPos[i] - fragIn.FragPos);		
    float attenuation = 1.f / (1.f + light.linear * d + light.quadratic * (d * d));        	 
	float epsilon = light.cutoff - light.outerCutoff;
	float S = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0) * attenuation;
    
	result *= S;
    result += S * diffuse;
    result += S * specular;
            
    return result;
		
}

void main()
{				
	vec3 normal = mix(texture(texture2.normal, fragIn.TexCoords), texture(texture1.normal, fragIn.TexCoords), fragIn.texH).rgb;
	normal = normalize(normal * 2.f - 1.f);
	vec3 textureColor = mix(texture(texture2.diffuse, fragIn.TexCoords), texture(texture1.diffuse, fragIn.TexCoords), fragIn.texH).rgb;	
	vec3 result = vec3(0.f);

	for(int i = 0; i < fragIn.numLights; i++)
	// for(int i = 0; i < numLightsOut; i++)
	{		
		switch(light[i].type)
		{
			case DIRECTIONAL: { result += DirectionalLight(light[i], i, normal, textureColor); break; }
			case POINT:		  { result += PointLight(light[i], i, normal, textureColor); break; }
			case SPOT:		  { result += SpotLight(light[i], i, normal, textureColor); break; } 
			default: break;
		}
	}
	

	FragColor = vec4(result, 1.0f);
}

