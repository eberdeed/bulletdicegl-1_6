/**********************************************************
 *   MeshVert:  A class that is derived from Mesh. This class
 *   is for untextured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MESHVERT_H
#define MESHVERT_H

#include "commonheader.h"
#include "mesh.h"
#include "shader.h"

//! Forward declarations so it can be used as a library.
struct Vertex1;
struct PointLight;
struct SpotLight;

/** \class MeshVert A class to encapsulate the 
 *  display of 3D assets acquired by the Model 
 *  class using the Assimp library.  This 
 *  class is for meshes that do not contain any 
 *  textures.
 */
class MeshVert : public Mesh
{
public:
    //! \brief Empty constructor.
    MeshVert();
    //! \brief Destructor destroying a vertex array and an index buffer.
    ~MeshVert();
    /*  Functions  */
    //! \brief Pass data to be displayed here from the Model class.
    void setData(Vertex1 *vertices, GLuint *indices, vec3 color, int vertSize, int indexSize);
    //! \brief Display the data.
    void Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, int  startIndex = 0);
    //! Class global variables.
    /*  Mesh Data  */
    //! The vertex array.
    Vertex1 *vertices;
    //! The index array.
    GLuint *indices;
    //! \brief Debugging function.
    void dumpData();
    /*  Render data  */
    //! The OpenGL buffer object handles.
    GLuint VAO, VBO, EBO;
    //! The upper bounds of the vertex and index arrays respectively.
    int vertSize, indexSize;
    //! The default color of the object.
    vec3 colordiff = vec3(1.0f, 1.0f, 1.0f);
    /*  Functions    */
    //! \brief Create the vertex array buffer, buffer object and index buffers.
    void setupMesh();
};


#endif // MESHVERT_H
