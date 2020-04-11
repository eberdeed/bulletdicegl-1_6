/**********************************************************
 *   Mesh:  A class to be a base class for MeshTex 
 *   (textured meshes) and MeshVert (untextured meshes).  
 *   This class isolates that acquisition of data from a 
 *   3d asset file using assimp.
 *   This is an abstract class that is not to be instantiated,
 *   but only used as a pointer for the derived classes.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com
 *   03/2020 San Diego, California USA
 * ********************************************************/

#ifndef MESH_H
#define MESH_H


#include "commonheader.h"
#include "shader.h"

//! Forward declarations so it can be used as a library.
struct Vertex;
struct Vertex1;
struct PointLight;
struct SpotLight;
/** \class Mesh A class that is a base class for the two classes
 * MeshTex (textured meshes) and MeshVert (untextured meshes).
 */
class Mesh {
public:
    //! \brief Constructor echoing its creaion (should not occur).
    Mesh();
    //! \brief Destructor echoing its destruction (should not occur).
    ~Mesh();
    /*  Functions  */
    //! \brief Set data for MeshTex.
    void setData(Vertex *vertices, GLuint *indices, GLuint texture, int vertSize, int indexSize, int texSize);
    //! \brief Set data for MeshVert.
    void setData(Vertex1 *vertices, GLuint *indices, int vertSize, int indexSize);
    //! \brief A virtual function implemented and used by both classes.
    virtual void Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex = 0, bool diffOnly = true);
    //! \brief A convenience function to pass messages.
    string getType();
    //! \brief A convenience function to post messages.
    void setType(string val);
    //! \brief Data checking.
    void dumpData();
    //! \brief Set the vertex array and index buffer.
    virtual void setupMesh();
    //! \brief Message data.
    string type;
    //! \brief The object's opaqueness.
    float opacity;
    
};


#endif // MESH_H
