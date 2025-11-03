#pragma once
#include <raylib.h>

class Disparo{
    private:
        Texture2D cuchillo;
        Vector2 position;
        int speed;

    public:
        Disparo(Vector2 position, int speed);
        void Update();
        void Draw();
        bool active;
        Rectangle getRect();
};