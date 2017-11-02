//
// Created by belisariops on 10/31/17.
//

#ifndef SHADERS_FRACTALPERLINNOISE_H
#define SHADERS_FRACTALPERLINNOISE_H


#include "ShaderProgram.h"

class FractalPerlinNoiseProgram : public ShaderProgram  {

    //Attribute locations
    GLuint mVertexPos2DLocation;
    GLuint mMultiColorLocation;

public:
    void enableVertexPointer();

    void enableColorPointer();

    void setVertexPointer(GLsizei stride, const GLvoid* data );

    void setTimePointer(GLsizei stride, const GLvoid* data );

    void disableColorPointer();

    void disableVertexPointer();

    int loadProgram() override;
};


#endif //SHADERS_FRACTALPERLINNOISE_H
