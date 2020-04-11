/**********************************************************
 *   MeshVert:  A class that is derived from Mesh. This class
 *   is for untextured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#include "../include/meshvert.h"

MeshVert::MeshVert()
{
    cout << "\n\n\tCreating MeshVert.\n\n";
    return;
}
    
//! Destroy array and buffers.
MeshVert::~MeshVert()
{
    cout << "\n\n\tDestroying MeshVert.\n\n";
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

//!  Pass along data to be displayed from the Model class.
void MeshVert::setData(Vertex1 *vertices, GLuint *indices, vec3 color, int vertSize, int indexSize)
{
    this->vertices = vertices;
    this->indices = indices;
    this->colordiff = color;
    this->vertSize = vertSize;
    this->indexSize = indexSize;
    
    setupMesh();
    //dumpData();
}

void MeshVert::dumpData()
{
    cout << "\n\nVertices, Normals and TexCoords: \n\n";
    for (int x = 0; x < vertSize; x++)
    {
        cout << "\nVertex: " << vertices[x].Position[0] << ", " << vertices[x].Position[1] << ", " << vertices[x].Position[2] <<
        " Normal: " << vertices[x].Normal[0] << ", " << vertices[x].Normal[1] << ", " << vertices[x].Normal[2];
    }
    cout << "\n\n\nIndices: \n\n";
    for (int x = 0; x < indexSize; x++)
    {
        cout << indices[x];
        if (((x + 1) % 25) == 0)
        {
            cout << "\n\n";
        }
    }
    cout << "\n\n";
}

//! Allocate array and buffers.
void MeshVert::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex1), vertices, GL_STATIC_DRAW);  
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}  

//! Draw object.
void MeshVert::Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex) 
{
    shader->Use();
    shader->setBool("diffOnly", false);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    shader->setVec3("viewPos", viewPos);
    //! No texture present.
    shader->setBool("isDiffuse1", false);
    shader->setFloat("shininess", 1.0f);
    shader->setVec3("colordiff", colordiff);
    shader->setFloat("opacity", opacity);
    for (int x = 0; x < lights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("pointLights[" + ss.str() + "].position", lights[x].position);
        shader->setVec3("pointLights[" + ss.str() + "].ambient", lights[x].ambient);
        shader->setVec3("pointLights[" + ss.str() + "].diffuse", lights[x].diffuse);
        shader->setVec3("pointLights[" + ss.str() + "].specular", lights[x].specular);
        shader->setFloat("pointLights[" + ss.str() + "].constant", lights[x].constant);
        shader->setFloat("pointLights[" + ss.str() + "].linear", lights[x].linear);
        shader->setFloat("pointLights[" + ss.str() + "].quadratic", lights[x].quadratic);
    }
    for (int x = 0; x < spotLights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("spotLights[" + ss.str() + "].position", spotLights[x].position);
        shader->setVec3("spotLights[" + ss.str() + "].direction", spotLights[x].direction);
        shader->setFloat("spotLights[" + ss.str() + "].cutOff", spotLights[x].cutOff);
        shader->setFloat("spotLights[" + ss.str() + "].outerCutOff", spotLights[x].outerCutOff);
        shader->setVec3("spotLights[" + ss.str() + "].ambient", spotLights[x].ambient);
        shader->setVec3("spotLights[" + ss.str() + "].diffuse", spotLights[x].diffuse);
        shader->setVec3("spotLights[" + ss.str() + "].specular", spotLights[x].specular);
        shader->setFloat("spotLights[" + ss.str() + "].constant", spotLights[x].constant);
        shader->setFloat("spotLights[" + ss.str() + "].linear", spotLights[x].linear);
        shader->setFloat("spotLights[" + ss.str() + "].quadratic", spotLights[x].quadratic);
    }
    cout << "\n\n\tOpacity:  " << opacity << "  Color Vector:  " 
    << colordiff.x << ", " << colordiff.y << ", " 
    << colordiff.z << "\n\n";
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
