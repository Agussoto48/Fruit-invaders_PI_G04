#include <raylib.h>
#include "include/combate.h"
#include "include/menuInicio.h"
#include "include/gameOver.h"
#include "include/pausado.h"
#include <string>
#include <iostream>

#define ESCALA 0.2f

int main()
{

    int windowWidth = 1200;
    int windowHeight = 960;
    InitWindow(windowWidth, windowHeight, "Fruit Invaders");
    Texture2D background = LoadTexture("sprites/kitchen.png");
    Font font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);
    Texture2D chefVidas = LoadTexture("sprites/Chef.png");
    Texture2D scoreIcono = LoadTexture("sprites/score.png");
    Color grisSuave = {50, 50, 50, 255};
    int frameCounter = 0;


    SetTargetFPS(60);
    {
        Combate combate;
        MenuInicio menuInicio;
        GameOver gameOver;
        Pausado pausa;
        int lastScore = 0;
        Color colorBackground = GRAY;

        
        while (!WindowShouldClose() && !menuInicio.quit)
        {
            frameCounter++;
            if (menuInicio.run)
            {
                menuInicio.Inputs();
                menuInicio.Update();
                if (!menuInicio.run)
                {
                    combate.run = true;
                }
            }
            if (pausa.run)
            {
                pausa.Inputs();
                pausa.Update();
                if (!pausa.run)
                {
                    combate.pausado = false;
                }
                if (pausa.exit)
                {
                    pausa.Reset();
                    combate.Reset();
                    lastScore = 0;
                    menuInicio.run = true;
                }
            }
            if (combate.run && !combate.pausado)
            {
                combate.Inputs();
                combate.Update();
                if (!combate.run)
                {
                    colorBackground = grisSuave;
                    lastScore = combate.score;
                    combate.Reset();
                    gameOver.run = true;
                }
                if(combate.pausado)
                {
                    pausa.run = true;
                }
            }
            if (gameOver.run)
            {
                gameOver.Inputs();
                gameOver.Update();
                if (!gameOver.run)
                {
                    colorBackground = GRAY;
                    menuInicio.run = true;
                }
            }

            BeginDrawing();
            ClearBackground(GRAY);
            DrawTexturePro(
                background,                                                // textura
                {0, 0, (float)background.width, (float)background.height}, // área original
                {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, // destino en pantalla
                {0, 0}, 0.0f, Fade(colorBackground, 0.9f)                  // Opacidad con respecto a un color
            );
            if (menuInicio.run)
            {
                if (frameCounter >= 20)
                {
                    menuInicio.animacionCuchillo();
                    frameCounter = 0;
                }
                menuInicio.Draw();
            }
            if (pausa.run)
            {
                if(frameCounter >= 20)
                {
                    pausa.animacionCuchillo();
                    frameCounter = 0;
                }
                pausa.Draw();
            }
            if (combate.run && !combate.pausado)
            {
                DrawTextEx(font, "Level 01", {1050, 20}, 34, 2, YELLOW);
                float posX = (GetScreenWidth() - scoreIcono.width * ESCALA) / 2 - 50;
                float posY = 0.0;
                DrawTextureEx(scoreIcono, {posX, posY}, 0.0f, ESCALA, WHITE);

                std::string numTexto = std::to_string(combate.score);
                int cerosRest = 5 - numTexto.length();
                numTexto = std::string(cerosRest, '0') + numTexto;

                float textX = posX + scoreIcono.width * 0.25f - 20.0f;
                float textY = posY + 35.0f;
                DrawTextEx(font, numTexto.c_str(), {textX, textY}, 34, 2, YELLOW);

                float x = 25.0;
                for (int i = 0; i < combate.lives; i++)
                {
                    DrawTextureEx(chefVidas, {x, 0}, 0.0, ESCALA, WHITE);
                    x += 50;
                }

                combate.Draw();
            }
            if (gameOver.run)
            {
                if (frameCounter >= 20)
                {
                    gameOver.animacionSemillas();
                    frameCounter = 0;
                }
                gameOver.Draw(lastScore);
            }

            EndDrawing();
        }
    }
    UnloadTexture(background);
    CloseWindow();

    return 0;
}
