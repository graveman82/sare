#include <SDL.h>
#include <conio.h>
#include <stdio.h>
#include <memory>
#include "engine.h"

int main(int argc, char* args[]) {
    std::unique_ptr<sare::Engine> engine_ptr(new sare::Engine());
    if (!sare::Engine::instance()) {
        return 0;
    }
    sare::Engine::instance().Run(argc, args);
    printf("Press any key to quit...\n");
    getch();
    return 0;
}
