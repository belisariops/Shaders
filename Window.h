//
// Created by belisariops on 6/15/16.
//

#ifndef PATTERNS_WINDOW_H
#define PATTERNS_WINDOW_H

#include <SDL2/SDL.h>

class Window {
    static Window* window ;
public:
    ~Window();
    bool getQuitState();
    void setQuitState(bool boolean);
    static Window* getInstance();
    SDL_Renderer* getRenderer();
    void update();
    int getScreenTicks();
private:
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
    Window(int width,int height);
    void init();
};


#endif //PATTERNS_WINDOW_H
