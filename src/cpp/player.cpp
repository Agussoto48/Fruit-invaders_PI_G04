#include "include/player.h"
#define ESCALA 0.3f
extern "C" void moverJugadorIzquierda(float, Vector2*);
extern "C" void moverJugadorDerecha(float, Vector2*);

// Constructores y destructores
Player::Player()
{
    chef = LoadTexture("sprites/Chef.png");
    position.x = (GetScreenWidth() - chef.width * ESCALA) / 2;
    position.y = GetScreenHeight() - (chef.height * ESCALA);
}
Player::~Player(){
}

/**
 * @brief Funcion para imprimir el personaje en la ventana
 */
void Player::Draw(){
    DrawTextureEx(chef, position, 0.0 , 0.3f , WHITE);
}
/**
 * @brief Funcion para mover el personaje a la izquierda, llamando a una funcion en asm
 */
void Player::MoveLeft(){
    float leftLimit = 0.0f;
    moverJugadorIzquierda(leftLimit, &position);
}
/**
 * @brief Funcion para mover el personaje a la derecha, llamando a una funcion en asm
 */
void Player::MoveRight(){
    float rightLimit = (float)(GetScreenWidth() - (chef.width * ESCALA));
    moverJugadorDerecha(rightLimit, &position);
}

void Player::Disparar(){
    if(GetTime() - tiempoDisparo >= 0.70){
        disparos.push_back(Disparo({position.x, position.y},-6));
        tiempoDisparo = GetTime();
    }
}

Rectangle Player::getRect() {
    Rectangle rectangle;

    float ancho = chef.width * ESCALA;
    float alto =  chef.height * ESCALA;
    rectangle.x = position.x + ancho/4;
    rectangle.y = position.y;
    rectangle.width = ancho / 2;
    rectangle.height = alto;

    return rectangle;
}