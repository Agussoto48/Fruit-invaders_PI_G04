#include "include/player.h"
#define ESCALA 0.2f

Player::Player()
{
    chef = LoadTexture("sprites/Chef.png");
    position.x = (GetScreenWidth() - chef.width * ESCALA) / 2;
    position.y = GetScreenHeight() - (chef.height * ESCALA);
    tiempoDisparo = 0;
}
Player::~Player(){
}

void Player::Draw(){
    DrawTextureEx(chef, position, 0.0 , ESCALA , WHITE);
}

//Estos dos metodos implementar con encsamblador
void Player::MoveLeft(){
    position.x -= 7;
    if(position.x < 0){
        position.x = 0;
    }
}
void Player::MoveRight(){
    position.x += 7; 
    if(position.x > (GetScreenWidth() - (chef.width * ESCALA))){
        position.x = GetScreenWidth() - (chef.width * ESCALA);
    }
}

void Player::Disparar(){
    if(GetTime() - tiempoDisparo >= 0.70){
        disparos.push_back(Disparo({position.x, position.y},-6));
        tiempoDisparo = GetTime();
    }
}

Rectangle Player::getRect() {
    return {position.x, position.y, float(chef.width), float(chef.height)};
}