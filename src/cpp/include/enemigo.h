#pragma once
#include<raylib.h>


class Enemigo{
private:
    
public:
    static Texture2D enemigoImages[3];
    Vector2 position;
    int type;

    //Constructores
    Enemigo(int type, Vector2 position);

    void Update(int direction);
    void Draw();
    int GetType();
    static void UnloadImages();
    Rectangle getRect();

};