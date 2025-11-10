#pragma once
#include<raylib.h>

class Enemigo{
private:
    
public:
    static Texture2D enemigoImages[3];
    Vector2 position;
    int type;

    // Constructores
    Enemigo(int type, Vector2 position);

    // Métodos
    void Update(int direction); //actualiza la posicion del enemigo, ya esta en ensambla
    bool MoveDown(int distance);  // Metodo en ensamblador para mover arriba y abajo
    void Draw(); //pone las imagenes de los enemigos
    int GetType(); //da el tipo de enemigo, sandia, manzana o pineapple
    static void UnloadImages(); //quita las imagenes cuando se cierra el juego por optimizacion
    Rectangle getRect(); //esto es el hitbox
};
