#include "include/player.h"
#define ESCALA 0.2f

Player::Player()
{
    chef = LoadTexture("sprites/Chef.png");
    position.x = (GetScreenWidth() - chef.width * ESCALA) / 2;
    position.y = GetScreenHeight() - (chef.height * ESCALA);
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