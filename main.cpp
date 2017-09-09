#include "Window.h"

int main() {
    Window* myWindow = Window :: getInstance();
    while (!myWindow->getQuitState()){
    }
    return 0;
}
