#include "include/player.h"
#define ESCALA 0.3f
enum direcciones
{
    IZQ = 0,
    DER = 1
};
extern "C" void moverJugador(float, Vector2*, int);

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
    moverJugador(leftLimit, &position, IZQ);
}
/**
 * @brief Funcion para mover el personaje a la derecha, llamando a una funcion en asm
 */
void Player::MoveRight(){
    float rightLimit = (float)(GetScreenWidth() - (chef.width * ESCALA));
    moverJugador(rightLimit, &position, DER);
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
    rectangle.x = position.x + ancho/3;
    rectangle.y = position.y + 25;
    rectangle.width = ancho / 3.5;
    rectangle.height = alto / 1.5;

    return rectangle;
}