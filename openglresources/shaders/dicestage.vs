/**********************************************************
 *   diffspecreflect.vs:  A shader to render the form
 *   for an OpenGL program.  It passes along texture and
 *   normals, and it was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 * ********************************************************/ 
/** \class diffspecreflect.vs
  *  Vertex shader.
  */

#version 300 es

#define TILES 2500

precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 binormal;
layout (location = 3) in vec2 texCoord;

struct Location
{
    vec3 Normal;
    vec3 BiNormal;
    vec3 Position;
    vec2 TexCoord;
    float Index1;
};
out Location locval;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0f);
    locval.Normal = normal;
    locval.Position = position;
    locval.TexCoord = texCoord;
    locval.BiNormal = binormal;
    if (gl_VertexID < (TILES * 6))
    {
        locval.Index1 = 0.0;
    }
    else if (gl_VertexID < (TILES * 12))
    {
        locval.Index1 = 1.0;
    }
    else
    {
        locval.Index1 = 2.0;
    }
} 
