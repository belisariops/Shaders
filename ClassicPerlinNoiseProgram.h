//
// Created by belisariops on 10/18/17.
//

#ifndef SHADERS_PEARLINGNOISEPROGRAM_H
#define SHADERS_PEARLINGNOISEPROGRAM_H


#include "ShaderProgram.h"

class ClassicPerlinNoiseProgram : public ShaderProgram {

    //Attribute locations
    GLuint mVertexPos2DLocation;
    GLuint mMultiColorLocation;

public:
    void enableVertexPointer();

    void enableColorPointer();

    void setVertexPointer(GLsizei stride, const GLvoid* data );

    void setColorPointer(GLsizei stride, const GLvoid* data );

    void disableColorPointer();

    void disableVertexPointer();

    int loadProgram() override;
};


#endif //SHADERS_PEARLINGNOISEPROGRAM_H
