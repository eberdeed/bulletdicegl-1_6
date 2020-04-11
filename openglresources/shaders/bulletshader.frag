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

#define NR_POINT_LIGHTS 6  
#define NR_SPOT_LIGHTS 3

precision highp float;

struct Location
{
    vec3 Normal;
    vec3 BiNormal;
    vec3 Position;
    vec2 TexCoord;
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

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform samplerCube SkyBox;
uniform vec3 viewPos;
//! Texture (optional)
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_binormal1;
//! Specular intensity
uniform float shininess;
//! Toggle for texture
uniform bool isDiffuse;
uniform bool isSpecular;
uniform bool isBiNormal;
uniform bool diffOnly;
//! Object transparency
uniform float opacity;
uniform vec3 colordiff;

//! Shader variables
vec3 skyLight;
vec3 normal;
vec4 texVal, specVal;

/** \brief Calculate the color based on light and camera
  *  position as well as surface normal.
  */
// function prototypes
vec3 CalcDirLight(vec3 light, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir);

SpotLight spotter;
void main()
{
    if (isBiNormal)
    {
        // obtain normal from normal map in range [0,1]
        normal = texture(texture_binormal1, locval.TexCoord).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    }
    else
    {
        normal = normalize(locval.Normal);
    }
    if (isDiffuse)
    {
        texVal = texture(texture_diffuse1, locval.TexCoord);
    }
    else
    {
        texVal = vec4(colordiff, opacity);
    }
    if (isSpecular)
    {
        specVal = texture(texture_specular1, locval.TexCoord);
    }
    else
    {
        specVal = vec4(colordiff, opacity);
    }
    //! \brief Vector describing direction from camera to object.
    vec3 I = -normalize(viewPos - locval.Position);
    vec3 R = reflect(I, normal);
    skyLight = vec4(texture(SkyBox, R)).xyz;
    color = vec4(CalcDirLight(skyLight, I), opacity);
    for (int x = 0; x < NR_SPOT_LIGHTS; x++)
    {
        color += vec4(CalcSpotLight(spotLights[x], I), opacity);
    }
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        color += vec4(CalcPointLight(pointLights[i], I), opacity);
    }
    //! Debug stuff
    /*
    if (isDiffuse)
    {
        color = texture(texture_diffuse1, locval.TexCoord);
    }
    else
    {
        color = vec4(1.0, 1.0, 1.0, opacity);
    }
    */
}  

// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 light, vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(0.0, 50.0, 10.0) - locval.Position);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Combine results
    vec3 ambient  = 0.2 * texVal.xyz;
    vec3 diffuse  = 1.4 * diff * texVal.xyz;
    vec3 specular = spec * specVal.xyz;
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        return vec3(ambient + diffuse + specular);
    }
} 

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - locval.Position);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * texVal.xyz;
    vec3 diffuse = light.diffuse * diff * texVal.xyz;
    vec3 specular = light.specular * spec * specVal.xyz;
    float attenuation = 0.9;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        return vec3(ambient + diffuse + specular);
    }
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    float diff = max(dot(normal, light.direction), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
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
    vec3 specular = light.specular * spec * specVal.xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        return vec3(ambient + diffuse + specular);
    }
}
