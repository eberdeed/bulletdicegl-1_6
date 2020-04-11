/**********************************************************
 *   \class diffspecreflect.frag:  A shader to render the colors
 *   for an OpenGL program.  It passes along refraction
 *   and reflection it was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2020
 * ********************************************************/ 
/** \class diffspecreflect.frag
  *  Fragment shader.
  */
#version 300 es

#define NR_SPOT_LIGHTS 3

#define NR_POINT_LIGHTS 6  

precision highp float;

struct Location
{
    vec3 Normal;
    vec3 BiNormal;
    vec3 Position;
    vec2 TexCoord;
    float Index1;
};
in Location locval;


out vec4 color;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;
//! Texture (optional)
uniform highp sampler2DArray wallTex;

//! Shader variables
vec3 normal;
vec4 texVal;
vec3 texVec;
/** \brief Calculate the color based on light and camera
  *  position as well as surface normal.
  */
// function prototypes
vec3 CalcDirLight(vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir);

SpotLight spotter;
void main()
{
    texVec = vec3(locval.TexCoord.x, locval.TexCoord.y, locval.Index1);

    normal = normalize(locval.Normal);
    texVal = texture(wallTex, texVec);
    //! \brief Vector describing direction from camera to object.
    vec3 I = -normalize(viewPos - locval.Position);

    color = vec4(CalcDirLight(I), 1.0);
    for (int x = 0; x < NR_SPOT_LIGHTS; x++)
    {
        color += vec4(CalcSpotLight(spotLights[x], I), 1.0);
    }
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        color += vec4(CalcPointLight(pointLights[i], I), 1.0);
    }

    //! Debug stuff
    //color = texVal;
 }  

// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(0.0, 50.0, 10.0) - locval.Position);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient  = 0.2 * texVal.xyz;
    vec3 diffuse  = 1.4 * diff * texVal.xyz;
    return (ambient + diffuse);
} 

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(light.position - locval.Position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * texVal.xyz;
    vec3 diffuse = light.diffuse * diff * texVal.xyz;
    //float attenuation = 0.9;
    ambient *= attenuation;
    diffuse *= attenuation;
    return (ambient + diffuse);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    float diff = max(dot(normal, light.direction), 0.0);
    // attenuation
    float distance = length(light.position - locval.Position);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //float intensity = 1.0;
    float attenuation = 0.9;
    // combine results
    vec3 ambient = light.ambient * texVal.xyz;
    vec3 diffuse = light.diffuse * diff * texVal.xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    return (ambient + diffuse);
}
