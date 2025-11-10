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

    // Constructores
    Enemigo(EnemigoTipo type, Vector2 position);

    // Métodos
    void Update(int direction); //actualiza la posicion del enemigo, ya esta en ensambla
    bool MoveDown(int distance);  // Metodo en ensamblador para mover arriba y abajo
    void Draw(); //pone las imagenes de los enemigos
    EnemigoTipo GetType(); //da el tipo de enemigo, sandia, manzana o pineapple
    static void UnloadImages(); //quita las imagenes cuando se cierra el juego por optimizacion
    Rectangle getRect(); //esto es el hitbox
};
