#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fat.h>


///Archivos .h locales
#include "FunctionHelper.h"
#include "Serpiente.h"
#include "Fruta.h"

///Fondos
#include "logoInicio.h"
#include "fondoInicio.h"
//#include "fondoInicioSub.h"
#include "fondoJuego.h"
#include "fondoJuegoSub.h"
#include "snake.h"

///Fuentes de texto
#include "font.h"

///Sprites
#include "snakeSprite.h"

///Bancos de sonido
#include "soundbank.h"
#include "soundbank_bin.h"

class GameManager{

public:
    enum ESTADO {
        inicio, jugando, gameover
    };

private:

    int mPuntajeAlto = 100;
    int mPuntajeAct = 0;

    static GameManager* sInstancia;

    ESTADO mEstado = inicio;

    Serpiente* serp;
    Fruta* fruta;

    int fondo2,fondo2Sub,fondo3;

public:

    static GameManager* Instacia();
    static void Liberar();

    void Run();
private:

    GameManager();
    ~GameManager();

    void Update();
    void Render();

};


#endif // GAMEMANAGER_H
