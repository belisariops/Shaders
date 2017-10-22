//
// Created by belisariops on 10/18/17.
//

#include "ClassicPerlinNoiseProgram.h"

int ClassicPerlinNoiseProgram::loadProgram() {
    //Generate program
    mProgramID = glCreateProgram();

    //Load vertex shader
    GLuint vertexShader = loadShaderFromFile( "ClassicPerlinNoise.vert", GL_VERTEX_SHADER );

    //Check for errors
    if( vertexShader == 0 )
    {
        glDeleteProgram( mProgramID );
        mProgramID = 0;
        return 1;
    }

    //Attach vertex shader to program
    glAttachShader( mProgramID, vertexShader );


    //Create fragment shader
    GLuint fragmentShader = loadShaderFromFile( "ClassicPerlinNoise.frag", GL_FRAGMENT_SHADER );

    //Check for errors
    if( fragmentShader == 0 )
    {
        glDeleteShader( vertexShader );
        glDeleteProgram( mProgramID );
        mProgramID = 0;
        return 1;
    }

    //Attach fragment shader to program
    glAttachShader( mProgramID, fragmentShader );

    //Link program
    glLinkProgram( mProgramID );

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv( mProgramID, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE )
    {
        printf( "Error linking program %d!\n", mProgramID );
        printProgramLog( mProgramID );
        glDeleteShader( vertexShader );
        glDeleteShader( fragmentShader );
        glDeleteProgram( mProgramID );
        mProgramID = 0;
        return 1;
    }

    //Clean up excess shader references
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    mVertexPos2DLocation = glGetAttribLocation( mProgramID, "VertexPos" );
    if( mVertexPos2DLocation == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "VertexPos" );
    }

    mMultiColorLocation = glGetAttribLocation( mProgramID, "MultiColor" );
    if( mMultiColorLocation == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "MultiColor" );
    }

    return 0;
}

void ClassicPerlinNoiseProgram::setVertexPointer(GLsizei stride, const GLvoid* data ) {
    glVertexAttribPointer(mVertexPos2DLocation,2,GL_FLOAT,GL_FALSE,stride,data);
}

void ClassicPerlinNoiseProgram::setColorPointer(GLsizei stride, const GLvoid *data) {
    glVertexAttribPointer( mMultiColorLocation, 4, GL_FLOAT, GL_FALSE, stride, data );
}

void ClassicPerlinNoiseProgram::enableVertexPointer() {
    glEnableVertexAttribArray( mVertexPos2DLocation );
}

void ClassicPerlinNoiseProgram::disableVertexPointer() {
    glDisableVertexAttribArray( mVertexPos2DLocation );
}

void ClassicPerlinNoiseProgram::enableColorPointer() {
    glEnableVertexAttribArray( mMultiColorLocation );
}

void ClassicPerlinNoiseProgram::disableColorPointer() {
    glDisableVertexAttribArray( mMultiColorLocation );
}




