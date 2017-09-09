//
// Created by belisariops on 6/15/16.
//

#include "Window.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
using namespace std;

Window* Window::window=NULL;

Window :: Window(int width, int height) {
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
    SCREEN_FPS = 60;
    SCREEN_TICK_PER_FRAME = 1000/SCREEN_FPS;
    quitState =false;
    init();
}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}


void Window :: init() {


    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "Shaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
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

                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {            	
            	SetOpenGLAttributes();

               SDL_GL_SetSwapInterval(1);
            }

        }
    }


}

Window :: ~Window() {

    //Destroy window
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
    SDL_RenderPresent(this->gRenderer);
}

SDL_Renderer * Window::getRenderer() {
    return this->gRenderer;
}

int Window ::getScreenTicks() {
    return this->SCREEN_TICK_PER_FRAME;
}