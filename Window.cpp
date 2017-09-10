//
// Created by belisariops on 6/15/16.
//

#include "Window.h"
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
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(this->gWindow);
}

bool SetOpenGLAttributes()
{


	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are     disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}


void Window :: init() {

    //SetOpenGLAttributes();
    //Initialize SDL
    //Initialize FreeGLUT
    int *x = (int *)malloc(sizeof(int));
    *x = 0;
    char **asd = (char **)malloc(sizeof(char *));
    *asd = 0;
    glutInit(x,asd);

    //Create OpenGL 3.2 context
    glutInitContextVersion( 3, 2 );

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

    //Render quad
    if( gRenderQuad )
    {
        //Bind program
        //glUseProgram( gProgramID );

        //Enable vertex arrays
        glEnableClientState( GL_VERTEX_ARRAY );

        //Set vertex data
        glVertexPointer( 2, GL_FLOAT, 0, gQuadVertices );

        //Draw quad using vertex data
        glDrawArrays( GL_QUADS, 0, 4 );

        //Disable vertex arrays
        glDisableClientState( GL_VERTEX_ARRAY );


        //Unbind program
        //glUseProgram( NULL );
    }
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

Window* Window ::getInstance() {
    if (window == NULL)
        window = new Window(640,480);
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
    //Success flag
    bool success = true;

    //Generate program
    gProgramID = glCreateProgram();

    //Create vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

    //Get vertex source
    const GLchar* vertexShaderSource[] =
            {
                    "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
            };

    //Set vertex source
    glShaderSource( vertexShader, 1, vertexShaderSource, NULL );

    //Compile vertex source
    glCompileShader( vertexShader );

    //Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
    if( vShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile vertex shader %d!\n", vertexShader );
        printShaderLog( vertexShader );
        success = false;
    }
    else
    {
        //Attach vertex shader to program
        glAttachShader( gProgramID, vertexShader );


        //Create fragment shader
        GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

        //Get fragment source
        const GLchar* fragmentShaderSource[] =
                {
                        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
                };

        //Set fragment source
        glShaderSource( fragmentShader, 1, fragmentShaderSource, NULL );

        //Compile fragment source
        glCompileShader( fragmentShader );

        //Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
        if( fShaderCompiled != GL_TRUE )
        {
            printf( "Unable to compile fragment shader %d!\n", fragmentShader );
            printShaderLog( fragmentShader );
            success = false;
        }
        else
        {
            //Attach fragment shader to program
            glAttachShader( gProgramID, fragmentShader );


            //Link program
            glLinkProgram( gProgramID );

            //Check for errors
            GLint programSuccess = GL_TRUE;
            glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
            if( programSuccess != GL_TRUE )
            {
                printf( "Error linking program %d!\n", gProgramID );
                printProgramLog( gProgramID );
                success = false;
            }
            else
            {
                //Get vertex attribute location
                gVertexPos2DLocation = glGetAttribLocation( gProgramID, "LVertexPos2D" );
                if( gVertexPos2DLocation == -1 )
                {
                    printf( "LVertexPos2D is not a valid glsl program variable!\n" );
                    success = false;
                }
                else
                {
                    //Initialize clear color
                    glClearColor( 0.f, 0.f, 0.f, 1.f );

//                    //VBO data
//                    GLfloat vertexData[] =
//                            {
//                                    -0.5f, -0.5f,
//                                    0.5f, -0.5f,
//                                    0.5f,  0.5f,
//                                    -0.5f,  0.5f
//                            };
//
//
//
//                    //IBO data
//                    GLuint indexData[] = { 0, 1, 2, 3 };
//
//                    //Create VBO
//                    glGenBuffers( 1, &gVBO );
//                    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
//                    glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
//
//                    //Create IBO
//                    glGenBuffers( 1, &gIBO );
//                    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
//                    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
                    //Set quad verticies
                    gQuadVertices[ 0 ].x = -0.5f;
                    gQuadVertices[ 0 ].y = -0.5f;

                    gQuadVertices[ 1 ].x = 0.5f;
                    gQuadVertices[ 1 ].y = -0.5f;

                    gQuadVertices[ 2 ].x = 0.5f;
                    gQuadVertices[ 2 ].y = 0.5f;

                    gQuadVertices[ 3 ].x = -0.5f;
                    gQuadVertices[ 3 ].y = 0.5f;
                }
            }
        }
    }
    return success  ;
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
