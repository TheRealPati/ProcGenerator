#include "App.hpp"
#include "Utils.hpp"

#define PLAIN

int main(int argc, char * argv[]) {

    Randomizer randomizer;
    App app(randomizer);
    if(app.initWindow() == 1)
        return EXIT_FAILURE;
    app.initShaders();
    app.sceneSetup();

    app.loop();
    return EXIT_SUCCESS;
}