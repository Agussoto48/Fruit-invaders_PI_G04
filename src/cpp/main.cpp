#include<raylib.h>
#include "include/combate.h"
#include <iostream>


int main() {


    int windowWidth = 1200;
    int windowHeight = 960;
    InitWindow(windowWidth, windowHeight, "Fruit Invaders"); 
    Texture2D background = LoadTexture("sprites/kitchen.png");

    SetTargetFPS(60);

    Combate combate;

    while(!WindowShouldClose()){

        combate.Inputs();

        BeginDrawing();
        ClearBackground(GRAY);
        DrawTexturePro(
            background,                                      // textura
            { 0, 0, (float)background.width, (float)background.height }, // área original
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // destino en pantalla
            { 0, 0 }, 0.0f, Fade(GRAY, 0.9f)
        );
        combate.Draw();
        EndDrawing();
    }


    CloseWindow();

    return 0;
}
