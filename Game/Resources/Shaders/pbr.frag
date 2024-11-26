#version 330 core
/* Parts of this code were taken from https://learnopengl.com/PBR/Lighting */

/* Output */
out vec4 FragColor;
layout (location = 1) out vec3 VelocityOutput;

/* Input from vertex */
in VERT_OUT
{
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
    vec3 Velocity;
} fragIn;

// material parameters
struct Material
{
    sampler2D albedo;
    sampler2D normal;    
    sampler2D ao;
};

// lights
#define MAX_LIGHTS 4
struct Light
{
    vec3 position;
    vec3 color;
};

/* Uniforms */
uniform Material material;
uniform Light light[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, fragIn.TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fragIn.WorldPos);
    vec3 Q2  = dFdy(fragIn.WorldPos);
    vec2 st1 = dFdx(fragIn.TexCoords);
    vec2 st2 = dFdy(fragIn.TexCoords);

    vec3 N   = normalize(fragIn.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main()
{		
    vec3 albedo     = pow(texture(material.albedo, fragIn.TexCoords).rgb, vec3(2.2));
    vec3 aoMap =  texture(material.ao, fragIn.TexCoords).rgb;
    float ao        = aoMap.r;
    float roughness = aoMap.g;
    float metallic  = aoMap.b;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - fragIn.WorldPos);

    // calculate reflectance at normal incidence   
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numLights; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(light[i].position - fragIn.WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(light[i].position - fragIn.WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
    VelocityOutput = fragIn.Velocity;
}