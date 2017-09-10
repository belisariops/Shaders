#include <SDL2/SDL.h>

#include "Window.h"

int main() {
    Window* myWindow = Window :: getInstance();
    //Enable text input
    SDL_StartTextInput();
    while (!myWindow->getQuitState()) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                myWindow->setQuitState(true);
            }

                //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT )
            {
                int x = 0, y = 0;
                SDL_GetMouseState( &x, &y );
                myWindow->handleKeys( e.text.text[ 0 ], x, y );
            }
        }
        //Render quad
        myWindow->render();

        //Update screen
        myWindow->update();


    }
    //Disable text input
    SDL_StopTextInput();
    delete(myWindow);
    return 0;
}
