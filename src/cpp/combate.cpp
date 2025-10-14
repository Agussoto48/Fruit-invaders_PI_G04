#include "include/combate.h"
#define cellSize 100

Combate::Combate(){
    enemigos = crearEnemigos();
    direccionEnemigos = 1;
    //ultimoDisparoEnemigo = 0;
}

Combate::~Combate(){
    Enemigo::UnloadImages();
}

void Combate::Update(){
    moverEnemigos();

    //disparoEnemigo();
    //for(auto& disparo: enemigoDisparos){
    //  disparo.Update();
    //}
}

void Combate::Draw(){
    jugador.Draw();

    for(auto& enemigo: enemigos){
        enemigo.Draw();
    }

    //for(auto& disparo: disparoEnemigos){
    //  disparo.Draw();
    //}
}
void Combate::Inputs(){
    if(IsKeyDown(KEY_LEFT)){
        jugador.MoveLeft();
    } else if(IsKeyDown(KEY_RIGHT)){
        jugador.MoveRight();
    }
}

std::vector<Enemigo> Combate::crearEnemigos(){
    std::vector<Enemigo> enemigos;
    for(int fila = 0; fila<5; ++fila){
        for(int columna=0; columna < 11; ++columna){

            int enemigoType;
            if(fila==0){
                enemigoType = 3;
            }
            else{
                if (fila ==1 || fila ==2){
                    enemigoType = 2;
                }
                else{
                    enemigoType = 1;
                }
            }

            float x = 75 + columna * cellSize;
            float y = 110 + fila * cellSize;
            enemigos.push_back(Enemigo(enemigoType, {x, y}));
        }
    }
    return enemigos;
}

void Combate::moverEnemigos(){
    for(auto& enemigo: enemigos){
        if(enemigo.position.x + enemigo.enemigoImages[enemigo.type-1].width > GetScreenWidth()){
            direccionEnemigos = -1;
            moverAbajoEnemigos(4);
        }
        if(enemigo.position.x < 0 ){
            direccionEnemigos = 1;
            moverAbajoEnemigos(4);
        }
        enemigo.Update(direccionEnemigos);
    }
}

void Combate::moverAbajoEnemigos(int distance){
    for(auto& enemigo: enemigos){
        enemigo.position.y += distance;
    }
}

//void Combate::disparoEnemigo(){

    //double tiempoActual = GetTime();
    // if(tiempoActual - ultimoDisparoEnemigo >= disparoEnemigoIntervalo && !aliens.empty()){
        // int randomIndex = GetRandomValue(0, enemigo.size()-1);
        // Enemigo& enemigo = enemigos[randomIndex];
        // enemigoDisparos.push_back(Disparo({enemigo.position.x + enemigo.enemigoImages[enemigo.type-1].width/2,
        //                                  enemigo.position.y + enemigo.enemigoImages[enemigo.type-1].height}, 6));
        // ultimoDisparoEnemigo = GetTime();
    //}
    
//}