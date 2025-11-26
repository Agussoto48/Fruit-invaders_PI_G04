#pragma once
#include<raylib.h>

enum class EnemigoTipo{
    MANZANA = 0,
    PINA = 1,
    SANDIA = 2
};

class Enemigo{
private:
    
public:
    static Texture2D enemigoImages[3];
    Vector2 position;
    EnemigoTipo type;

    Enemigo(EnemigoTipo type, Vector2 position);

    void Update(int direction);
    bool MoveDown(int distance);
    void Draw();
    EnemigoTipo GetType();
    static void UnloadImages();
    Rectangle getRect();
};
