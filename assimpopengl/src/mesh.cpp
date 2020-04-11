/**********************************************************
 *   Mesh:  A class to be a base class for MeshTex 
 *   (textured meshes) and MeshVert (untextured meshes).  
 *   This class isolates that acquisition of data from a 
 *   3d asset file using assimp.
 *   This is an abstract class that is not to be instantiated.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com
 *   March 2020 San Diego, California USA
 * ********************************************************/

#include "../include/mesh.h"

Mesh::Mesh()
{
    cout << "\n\n\tCreating Mesh.\n\n";
    return;
}

Mesh::~Mesh()
{
    cout << "\n\n\tDestroying Mesh.\n\n";
    return;
}

void Mesh::dumpData()
{
    cout << "\n\nIn abstract class.\n";
    return;
}

void Mesh::setupMesh()
{
    cout << "\n\nIn abstract class.\n";
    return;
}

void Mesh::setData(Vertex *vertices, GLuint *indices, GLuint texture, int vertSize, int indexSize, int texSize)
{
    cout << "\n\nIn abstract class.\n";
    return;
}

void Mesh::setData(Vertex1 *vertices, GLuint *indices, int vertSize, int indexSize)
{
    cout << "\n\nIn abstract class.\n";
    return;
}

void Mesh::Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex, bool diffOnly) 
{
    cout << "\n\nIn abstract class.\n";
    return;
}

void Mesh::setType(string val)
{
    type = val;
}

string Mesh::getType()
{
    return type;
}
