#include <raylib.h>
#include "include/combate.h"
#include "include/menuInicio.h"
#include "include/gameOver.h"
#include "include/pausado.h"
#include "include/musica.h"
#include <string>

#define ESCALA 0.2f

int main()
{
    int windowWidth = 1200;
    int windowHeight = 960;
    InitWindow(windowWidth, windowHeight, "Fruit Invaders");

    Texture2D background = LoadTexture("sprites/kitchen.png");
    Texture2D cuttingBoard = LoadTexture("sprites/Cuttin_Board.png");
    Texture2D scoreIcono = LoadTexture("sprites/Score.png");
    Texture2D chefVidas = LoadTexture("sprites/Chef.png");
    Font font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);

    Musica musica;
    musica.CambiarMenu();
    musica.SetVolumen(0.5f);

    Color grisSuave = {50, 50, 50, 255};
    int frameCounter = 0;
    float aumento_musica = 0.1f;
    int nivel_actual = 1; //para trackear el nivel

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
            {
                // Leer inputs y actualizar
                musica.Update();
                frameCounter++;

                if (menuInicio.run)
                {
                    menuInicio.Inputs();
                    menuInicio.Update();
                    if (!menuInicio.run)
                    {
                        musica.CambiarCombate();
                        musica.SetVolumen(0.5f);
                        combate.run = true;
                    }
                }

                if (pausa.run)
                {
                    musica.SetVolumen(0.2f);
                    pausa.Inputs();
                    pausa.Update();
                    if (!pausa.run)
                    {
                        musica.SetVolumen(0.5f);
                        combate.pausado = false;
                    }
                    if (pausa.exit)
                    {
                        pausa.Reset();
                        combate.Reset();
                        lastScore = 0;
                        musica.CambiarMenu();
                        musica.SetVolumen(0.5f);
                        menuInicio.run = true;
                    }
                }

                if (combate.run && !combate.pausado)
                {
                    combate.Inputs();
                    combate.Update();
                    if (nivel_actual < combate.level)
                    {
                        musica.SetVelocidad(1.0 + aumento_musica);
                        aumento_musica += 0.1f;
                        nivel_actual = combate.level;
                    }
                    if (!combate.run)
                    {
                        colorBackground = grisSuave;
                        lastScore = combate.score;
                        combate.Reset();
                        musica.SetVelocidad(1.0f);
                        musica.CambiarMenu();
                        musica.SetVolumen(0.5f);
                        aumento_musica = 0.1f;
                        nivel_actual = 1; //resetear
                        gameOver.run = true;
                    }
                    if (combate.pausado)
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

                // Imprimir en pantalla
                BeginDrawing();
                ClearBackground(GRAY);
                DrawTexturePro(
                    background,
                    {0, 0, (float)background.width, (float)background.height},
                    {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                    {0, 0}, 0.0f, Fade(colorBackground, 0.9f));

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
                    if (frameCounter >= 20)
                    {
                        pausa.animacionCuchillo();
                        frameCounter = 0;
                    }
                    pausa.Draw();
                }

                if (combate.run && !combate.pausado)
                {
                    // Dibujar la tabla de cortar rotada 180 grados con ancho y largo personalizados
                    if (cuttingBoard.id != 0)
                    {
                        // Controles separados para ancho y largo
                        float boardScaleWidth = 4.55f; // Escala horizontal (ancho)
                        float boardScaleHeight = 1.7f; // Escala vertical (largo)

                        float centerX = GetScreenWidth() / 2.0f;
                        float centerY = GetScreenHeight() - 455.0f;

                        Rectangle source = {0, 0, (float)cuttingBoard.width, (float)cuttingBoard.height};
                        Rectangle dest = {
                            centerX,
                            centerY,
                            cuttingBoard.width * boardScaleWidth,  // Ancho personalizado
                            cuttingBoard.height * boardScaleHeight // Largo personalizado
                        };

                        // Rotar 180 grados con el origen en el centro
                        DrawTexturePro(cuttingBoard, source, dest,
                                       {dest.width / 2, dest.height / 2},
                                       180.0f,
                                       WHITE);
                    }

                    combate.Draw();

                    std::string levelText = "Level ";
                    if (combate.level < 10)
                    {
                        levelText += "0";
                    }
                    levelText += std::to_string(combate.level);
                    DrawTextEx(font, levelText.c_str(), {1050, 20}, 34, 2, YELLOW);

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
    }
    UnloadTexture(background);
    UnloadTexture(cuttingBoard);
    UnloadTexture(scoreIcono);
    UnloadTexture(chefVidas);
    CloseWindow();

    return 0;
}