#include "App.hpp"

#define PLAIN

int main(int argc, char * argv[]) {

    App app;
    if(app.initWindow() == 1)
        return EXIT_FAILURE;
    app.initShaders();
    app.sceneSetup();

    app.loop();
    return EXIT_SUCCESS;
}