//
// Created by belisariops on 6/15/16.
//

#ifndef PATTERNS_WINDOW_H
#define PATTERNS_WINDOW_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "VertexPos.h"
#include "ShaderProgram.h"

class Window {
    static Window* window ;
public:
    ~Window();
    bool getQuitState();
    void setQuitState(bool boolean);
    static Window* getInstance(int width, int height);
    SDL_Renderer* getRenderer();
    void update();
    int getScreenTicks();
    int loadShader();
    int loadMedia();
    void handleKeys(unsigned char key, int x, int y);
    void render();

private:
    VertexPos gQuadVertices[4];
    //Graphics program
    GLuint gProgramID = 0;
    GLint gVertexPos2DLocation = -1;
    GLuint gVBO = 0;
    GLuint gVBO2 = 0;
    GLuint gIBO = 0;
    GLuint gIBO2 = 0;
    //Render flag
    bool gRenderQuad = true;
    bool quitState;
    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;
    int SCREEN_FPS;
    int SCREEN_TICK_PER_FRAME;
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
    //OpenGL context handler
    SDL_GLContext mainContext;
    //The window renderer
    SDL_Renderer* gRenderer;
    //ShaderProgram
    ShaderProgram *shaderProgram;
    Window(int width,int height);
    void init();
    bool initGL();
    void printShaderLog(GLuint shader);
    void printProgramLog(GLuint id);
};


#endif //PATTERNS_WINDOW_H
