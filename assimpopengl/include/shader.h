/*******************************************************************
 * Shader:  A class to encapsulate the creation and use of a set of
 * shaders.  Note this class requires a seperate "shaders" directory
 *  to store the shaders in.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * March 2020 San Diego, California USA
 * ****************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include "commonheader.h"
#include <iostream>
#include <string>
// GLM The OpenGL math library
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
/** \class Shader A class to encapsulate the uploading, compiling, 
 * linking and use of a shader.  Note this class requires a seperate 
 * "shaders" directory to store the shaders in.  Further, this class 
 * will create a shader binary and reload it.  The binary will be 
 * recreated if it ceases to validate as a shader program.
 */

class Shader
{
public:
    /** \brief The constructor signals the creation of the class.
     */
    Shader();
    /** \brief The destructor signals the destruction of the class,
     *  and deletes the shader program.
     */  
    ~Shader();
    /** \brief Read and build the shader program from two files,
     *  and provide the file name for the created binary.
     *  Delete the two shader objects afterwards.
     */
    void initShader(string vertexPath, string fragmentPath, 
    string outputFile);
    /** \brief Create the vertex or fragment shader from a file.
     */
    unsigned int createShader(unsigned int type, string fpath);
    /** \brief Use the program object for display.
     */
    void Use();
    /** \brief Create the shader program binary save it to a file,
     * and delete the binary pointer afterward.
     */
    bool createBinary();
    /** \brief Utility uniform functions that set values in the shader(s).
     */
    void setBool(const string name, bool value) const;  
    void setInt(const string name, int value) const;   
    void setFloat(const string name, float value) const;
    void setVec2(const std::string name, vec2 value) const;
    void setVec3(const std::string name, vec3 value) const;
    void setVec4(const string name, vec4 value) const; 
    void setMat4(const string name, mat4 value) const;    
    //! The shader program object.
    GLuint Program;
    //! Class global variables.
    int success = 0;
    int infoLength = 0;
    GLuint vertex, fragment;
    int progLength = 0;
    int progLenRet = 0;
    //! Formats for the stored binary program.
    GLenum *valFormats;
    GLenum format;
    int response = 0;
    //! The binary program pointer.
    unsigned char *binary;
    /**! The output file name.  The final stored binary is stored
     * int the .config directory of the user's home directory.
     */
    string outputFile;

};
  
#endif //SHADER_H
