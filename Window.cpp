//
// Created by belisariops on 6/15/16.
//

#include "Window.h"
#include "PearlingNoiseProgram.h"
#include "Position.h"
#include <iostream>
#include <GL/freeglut.h>

using namespace std;



Window* Window::window=NULL;

Window :: Window(int width, int height) {
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
    SCREEN_FPS = 60;
    SCREEN_TICK_PER_FRAME = 1000/SCREEN_FPS;
    quitState =false;
    init();
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(this->gWindow);
    this->shaderProgram = new PearlingNoiseProgram();
}

bool SetOpenGLAttributes()
{


	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are     disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}


void Window :: init() {

    //Initialize SDL
    //Initialize FreeGLUT
    int *x = (int *)malloc(sizeof(int));
    *x = 0;
    char **asd = (char **)malloc(sizeof(char *));
    *asd = 0;
    glutInit(x,asd);

    //Create OpenGL 3.2 context
    glutInitContextVersion( 3, 0 );

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        SetOpenGLAttributes();
        //Create window
        gWindow = SDL_CreateWindow( "Shaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Create renderer for window
            //this->gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            this->mainContext = SDL_GL_CreateContext(this->gWindow);    
            if( this->mainContext == NULL )
            {

                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Initialize GLEW
                glewExperimental = GL_TRUE;
                GLenum glewError = glewInit();
                if( glewError != GLEW_OK )
                {
                    printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
                }

                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 )
                {
                    printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
                }

                //Initialize OpenGL
                if( !initGL() )
                {
                    printf( "Unable to initialize OpenGL!\n" );
                }
            }

        }
    }

}

void Window :: render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset transformations
    glLoadIdentity();

    //Solid cyan quad in the center
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Enable vertex arrays
    glEnableClientState( GL_VERTEX_ARRAY );

    //Set vertex data
    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    glVertexPointer( 2, GL_FLOAT, 0, NULL );

    //Draw quad using vertex data and index data
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

    //Disable vertex arrays
    glDisableClientState( GL_VERTEX_ARRAY );
}

Window :: ~Window() {

    //Destroy window
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

Window* Window ::getInstance(int width, int height) {
    if (window == NULL)
        window = new Window(width, height);
    return window;
}

bool Window ::getQuitState() {
    return this->quitState;
}

void Window ::setQuitState(bool boolean) {
    this->quitState = boolean;
}

void Window :: update() {
    SDL_GL_SwapWindow( gWindow );
}

SDL_Renderer * Window::getRenderer() {
    return this->gRenderer;
}

int Window ::getScreenTicks() {
    return this->SCREEN_TICK_PER_FRAME;
}

bool Window::initGL() {
    //Initialize GLEW
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return false;
    }

    //Make sure OpenGL 3.0 is supported
    if( !GLEW_VERSION_3_0 )
    {
        printf( "OpenGL 3.0 not supported!\n" );
        return false;
    }

    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    return true;
}

void Window::printShaderLog(GLuint shader) {
    //Make sure name is shader
    if( glIsShader( shader ) )
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

        //Allocate string
        char* infoLog = new char[ maxLength ];

        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a shader\n", shader );
    }

}

void Window::printProgramLog(GLuint program) {
    //Make sure name is shader
    if( glIsProgram( program ) )
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

        //Allocate string
        char* infoLog = new char[ maxLength ];

        //Get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a program\n", program );
    }

}

void Window :: handleKeys( unsigned char key, int x, int y )
{
    //Toggle quad
    if( key == 'q' )
    {
        gRenderQuad = !gRenderQuad;
    }
}



int Window::loadShader() {
    //Load basic shader program
    if( this->shaderProgram->loadProgram() )
    {
        printf( "Unable to load basic shader!\n" );
        return 1;
    }

    //Bind basic shader program
    this->shaderProgram->bind();
    return 0;
}

int Window::loadMedia() {
    //VBO data
    Position quadVertices[ 4 ];
    GLuint indices[ 4 ];

    //Set quad vertices
    quadVertices[ 0 ].x = -50.f;
    quadVertices[ 0 ].y = -50.f;

    quadVertices[ 1 ].x =  50.f;
    quadVertices[ 1 ].y = -50.f;

    quadVertices[ 2 ].x =  50.f;
    quadVertices[ 2 ].y =  50.f;

    quadVertices[ 3 ].x = -50.f;
    quadVertices[ 3 ].y =  50.f;

    //Set rendering indices
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;
    indices[ 3 ] = 3;

    //Create VBO
    glGenBuffers( 1, &gVBO );
    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(Position), quadVertices, GL_STATIC_DRAW );

    //Create IBO
    glGenBuffers( 1, &gIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW );

    return 0;
}
