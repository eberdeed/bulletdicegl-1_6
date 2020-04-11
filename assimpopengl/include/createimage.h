/**********************************************************
 *   CreateImage:  A class to encapsulate loading an image
 *   into a GLvoid blob.  Designed for OpenGL. This class
 *   assumes that there is an images directory with the 
 *   graphics in it.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   March 2020 San Diego, California USA
 * ********************************************************/

#define LIMIT 71
#ifndef CREATEIMAGE_H
#define CREATEIMAGE_H
#include "commonheader.h"

using namespace std;

/* \class CreateImage : Using Free Image Plus, this class loads an 
 * image into memory, converts it to a 32 bit format with alpha, 
 * and then passes it to an array of unsigned characters, which 
 * is then passed using a cast to a GLvoid blob. Optionally the 
 * image can be turned into an OpenGL buffer object. It provides
 * for a single image, a vector of images and a sky box containing
 * six images. 
 */
class CreateImage
{
public:
    CreateImage();
    ~CreateImage();
    //!  Load image and convert it.
    bool setImage(string imagefile);
    //! Accessor functions for the given image's dimensions and data.
    GLsizei getWidth();
    GLsizei getHeight();
    GLvoid *getData();
    //! Return an OpenGL buffer object.
    GLuint textureObject();
    //! Return an OpenGL sky box object.
    void createSkyBoxTex(GLuint &textureID, string filenames[6]);
    //! Create an array of images for an OpenGL Texture2DArray object.
    void create2DTexArray(GLuint &textureID, vector<string>filenames);
protected:
    //! Class global variables.
    //! The Free Image Plus image object.
    fipImage txtImage;
    //! A line of image data.
    BYTE *picLine;
    //! Image dimensions.
    GLsizei width;
    GLsizei height;
    int size = 0;
    //! Image data.
    unsigned char *pixels = NULL;
    //! Line and byte counters.
    int count, line;
    //! A little tiny bit of debug info.
    bool debug1 = false;
};
#endif // CreateImage.h
