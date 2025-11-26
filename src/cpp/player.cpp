#include <iostream>
#include "include/player.h"

#define ESCALA 0.3f

enum direcciones
{
    IZQ = 0,
    DER = 1
};
extern "C" void moverJugador(float, Vector2*, int);

Player::Player()
{
    chef = LoadTexture("sprites/Chef.png");
    position.x = (GetScreenWidth() - chef.width * ESCALA) / 2;
    position.y = GetScreenHeight() - (chef.height * ESCALA);
    tiempoDisparo = 0.0;
}

Player::~Player(){
}

void Player::Draw(){
    DrawTextureEx(chef, position, 0.0 , 0.3f , WHITE);
}

void Player::MoveLeft(){
    float leftLimit = 0.0f;
    moverJugador(leftLimit, &position, IZQ);
}

void Player::MoveRight(){
    float rightLimit = (float)(GetScreenWidth() - (chef.width * ESCALA));
    moverJugador(rightLimit, &position, DER);
}

void Player::MoveFromJoystick(int xValue) {
    std::cout << "Joystick X: " << xValue << std::endl;
    
    const int deadZoneLow = 450;
    const int deadZoneHigh = 550;
    const int center = 512;
    
    std::cout << xValue << std::endl;

    if (xValue < deadZoneLow) {
        std::cout << "Moving LEFT" << std::endl;
        MoveRight();
    } else if (xValue > deadZoneHigh) {
        std::cout << "Moving RIGHT" << std::endl;
        MoveLeft();
    } else {
        std::cout << "In DEAD ZONE" << std::endl;
    }
}

bool Player::ShouldShootFromJoystick(int buttonState) {
    return buttonState == 0;
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