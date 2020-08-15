#include "Serpiente.h"

Serpiente::Serpiente(u16* mapEnVram, u16* oamMemory){
    velocidadY = 0;
    velocidadX = 2;

    direccionCabeza = derecha;

    muerta = false;

    velMovimiento = 15; /// 16 pixeles por cada 15 frame(S)
    contMovimiento = 0;

    moverse = true;

    tam = 3;

    //memVramMap = mapEnVram;
    mapaTiles = mapEnVram;
    sprPtr = oamMemory;

   // oamSet(&oamMain, 0, 0, 0, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color,
     //      sprPtr, -1, false, false, false, false, false);
    oamSet(&oamMain,0,0,0,0,15,SpriteSize_16x16, SpriteColorFormat_Bmp,
           sprPtr, -1, false, false, false, false, false);
    oamSetAlpha(&oamMain,0,alfa);

    oamSetAffineIndex(&oamMain,0,0,true);

    for(int i = 0; i < 6; i+=2){
        CuerpoSerpiente* nuevo = new CuerpoSerpiente();
        nuevo->posX = 2+i;
        nuevo->posY = 2;
        cuerpo.push_back(nuevo);
    }
}

Serpiente::~Serpiente(){
    for(unsigned int i = 0; i < cuerpo.size(); i++){
        delete cuerpo[i];
        cuerpo[i] = nullptr;
    }
}

void Serpiente::Render(){
    ///Reinicio del "buffer" del mapa de tiles
    for(int i = 0; i <1024; i++){
        mapaTiles[i] = 0;
    }
    oamSetAlpha(&oamMain,0,alfa);
    ///Render de cada parte del cuerpo de la serpiente
    for(uint i = 0; i < cuerpo.size(); i++){
        int colUno =  offsetDeMapa(cuerpo[i]->posX,cuerpo[i]->posY,32);
        int colDos =  offsetDeMapa(cuerpo[i]->posX,cuerpo[i]->posY+1,32);

        if(i == cuerpo.size() -1){  ///Render de la cabeza del cuerpo, tiene 4 posiciones diferentes
            oamSetXY(&oamMain,0,(cuerpo[i]->posX*8) -4,(cuerpo[i]->posY*8)-4);

            int dx = cuerpo[i]->posX - cuerpo[i-1]->posX;
            int dy = cuerpo[i]->posY - cuerpo[i-1]->posY;

            dx /= 2;
            dy /= 2;

            if(dx == 1){
                oamRotateScale (&oamMain, 0, anguloAEntero32B(-90), 1<<8, 1<<8);
                /*mapaTiles[colUno] = 5;
                mapaTiles[colUno+1] = 4;
                mapaTiles[colDos] = 6;
                mapaTiles[colDos+1] = FLIPV_TILE(4);*/
            }else if(dx == -1){
                oamRotateScale (&oamMain, 0, anguloAEntero32B(90), 1<<8, 1<<8);
                /*mapaTiles[colUno] = FLIPH_TILE(4);
                mapaTiles[colUno+1] = FLIPH_TILE(5);
                mapaTiles[colDos] = FLIPHV_TILE(4);
                mapaTiles[colDos+1] = FLIPH_TILE(6);*/
            }else if(dy == -1){

                oamRotateScale (&oamMain, 0, anguloAEntero32B(180), 1<<8, 1<<8);
                /*mapaTiles[colUno] = 1;
                mapaTiles[colUno+1] = FLIPH_TILE(1);
                mapaTiles[colDos] = 2;
                mapaTiles[colDos+1] = 3;*/
            }else{
                oamRotateScale (&oamMain, 0, 0, 1<<8, 1<<8);
                /*oamSetXY(&oamMain,0,cuerpo[i]->posX*8,cuerpo[i]->posY*8);*/
                /*mapaTiles[colUno] = FLIPV_TILE(2);
                mapaTiles[colUno+1] = FLIPV_TILE(3);
                mapaTiles[colDos] = FLIPV_TILE(1);
                mapaTiles[colDos+1] = FLIPHV_TILE(1);*/
            }
        }else if(i == 0){  ///Render de la cola de la serpiente, tiene 4 rotaciones diferentes

            int dx = cuerpo[i]->posX - cuerpo[i+1]->posX;
            int dy = cuerpo[i]->posY - cuerpo[i+1]->posY;

            dx /= 2;
            dy /= 2;

            if(dx == 1){
                    mapaTiles[colUno] = FLIPH_TILE(17);
                    mapaTiles[colUno+1] = 0;
                    mapaTiles[colDos] = FLIPH_TILE(18);
                    mapaTiles[colDos+1] = 0;
            }else if(dx == -1){
                    mapaTiles[colUno] = 0;
                    mapaTiles[colUno+1] = 17;
                    mapaTiles[colDos] = 0;
                    mapaTiles[colDos+1] = 18;
            }else if(dy == -1){
                    mapaTiles[colUno] = 0;
                    mapaTiles[colUno+1] = 0;
                    mapaTiles[colDos] = FLIPV_TILE(15);
                    mapaTiles[colDos+1] = FLIPV_TILE(16);
            }else{
                    mapaTiles[colUno] = 15;
                    mapaTiles[colUno+1] = 16;
                    mapaTiles[colDos] = 0;
                    mapaTiles[colDos+1] = 0;
            }
        }else{
            int numSprite = 0; /// Este numero codifica en que lugar esta la siguiente parte del cuerpo y la anterior
            /*
                        *   # de bit                                          # de bit   4  3  2  1
                    x   y      2                                       2^(# de bit -1)   8  4  2  1
                    0   -1                          ¿Hay cuerpo en esa posicion? (1/0)   0  0  1  1  = 3
       #bit    *        *           *    #bit
         1   x  y   Parte Actual  x  y     4       Por ejemplo si el cuerpo esta asi:
            -1  0                +1  0                       %        * = cuerpo[i], cuerpo actual
                        *   #bit                             *%       % = cuerpo[i(+/-) 1], cuerpos adyacentes
                    x   y     3                    entonces numSprite sera igual a 10
                    0   +1
            */

            for(int j = -1; j<=1; j+=2){
                int dx = cuerpo[i+j]->posX - cuerpo[i]->posX;
                int dy = cuerpo[i+j]->posY - cuerpo[i]->posY;

                dx /= 2;
                dy /= 2;

                int posiciones = (1 << (dx+1)) + (((dy+1) & 2) >> 1);
                numSprite |= 1 << (posiciones-1);
            }


            switch(numSprite){
                case 3:
                    mapaTiles[colUno] = FLIPHV_TILE(14);
                    mapaTiles[colUno+1] = FLIPHV_TILE(13);
                    mapaTiles[colDos] = FLIPHV_TILE(12);
                    mapaTiles[colDos+1] = FLIPHV_TILE(11);
                    break;
                case 5:
                    mapaTiles[colUno] = FLIPH_TILE(12);
                    mapaTiles[colUno+1] = FLIPH_TILE(11);
                    mapaTiles[colDos] = FLIPH_TILE(14);
                    mapaTiles[colDos+1] = FLIPH_TILE(13);
                    break;
                case 6:
                    mapaTiles[colUno] = FLIPHV_TILE(10);
                    mapaTiles[colUno+1] = FLIPHV_TILE(9);
                    mapaTiles[colDos] = 9;
                    mapaTiles[colDos+1] = 10;
                    break;
                case 9:
                    mapaTiles[colUno] = 7;
                    mapaTiles[colUno+1] = FLIPHV_TILE(8);
                    mapaTiles[colDos] = 8;
                    mapaTiles[colDos+1] = FLIPHV_TILE(7);
                    break;
                case 10:
                    mapaTiles[colUno] = FLIPV_TILE(13);
                    mapaTiles[colUno+1] = FLIPV_TILE(14);
                    mapaTiles[colDos] = FLIPV_TILE(11);
                    mapaTiles[colDos+1] = FLIPV_TILE(12);
                    break;
                case 12:
                    mapaTiles[colUno] = 11;
                    mapaTiles[colUno+1] = 12;
                    mapaTiles[colDos] = 13;
                    mapaTiles[colDos+1] = 14;
                    break;
            }

        }
    }

    ///Copio todo el "buffer" a la memoria de video donde esta el mapa del fondo
    /*for(int i = 0; i<1024; i++){
        memVramMap[i] = mapaTiles[i];
    }*/
    //dmaCopy(mapaTiles, memVramMap, 2048);
}

void Serpiente::Crecer(){
    tam++;
}

void Serpiente::Update(){
    int dx = cuerpo[cuerpo.size()-1]->posX - cuerpo[cuerpo.size()-2]->posX;
    int dy = cuerpo[cuerpo.size()-1]->posY - cuerpo[cuerpo.size()-2]->posY;

    dx /= 2;
    dy /= 2;
    if(keysDown() & KEY_DOWN){
        if(dy != -1){
            velocidadX = 0;
            velocidadY = 2;
            direccionCabeza = abajo;
        }
    }else if(keysDown() & KEY_LEFT){
        if(dx != 1){
            velocidadX = -2;
            velocidadY = 0;
            direccionCabeza = izquierda;
        }
    }else if(keysDown() & KEY_UP){
        if(dy != 1){
            velocidadX = 0;
            velocidadY = -2;
            direccionCabeza = arriba;
        }
    }else if(keysDown() & KEY_RIGHT){
        if(dx != -1){
            velocidadX = 2;
            velocidadY = 0;
            direccionCabeza = derecha;
        }
    }

    contMovimiento++;
    if(contMovimiento>=velMovimiento){
        moverse = true;
        contMovimiento = 0;
    }

    if(!moverse) return;
    moverse = false;

    if(tam <= cuerpo.size()){
        int nuevaX =  cuerpo[cuerpo.size()-1]->posX + 1*velocidadX;
        int nuevaY =  cuerpo[cuerpo.size()-1]->posY + 1*velocidadY;

        if(nuevaX < 0 || nuevaX > 31 || nuevaY < 0 || nuevaY > 23){
            muerta = true;
            return;
        }

        for(unsigned int i = 0; i < cuerpo.size()-1;i++){
            cuerpo[i]->posX = cuerpo[i+1]->posX;
            cuerpo[i]->posY = cuerpo[i+1]->posY;
            if(cuerpo[i]->posX == nuevaX && cuerpo[i]->posY == nuevaY){
                muerta = true;
            }
        }

        cuerpo[cuerpo.size()-1]->posX = nuevaX;
        cuerpo[cuerpo.size()-1]->posY = nuevaY;
    }else{
        CuerpoSerpiente* nuevo = new CuerpoSerpiente();
        nuevo->posX = cuerpo[cuerpo.size()-1]->posX + 1*velocidadX;
        nuevo->posY = cuerpo[cuerpo.size()-1]->posY + 1*velocidadY;


        if(nuevo->posX  < 0 || nuevo->posX  > 31 || nuevo->posY < 0 || nuevo->posY > 23){
            muerta = true;
            delete nuevo;
            return;
        }

        for(unsigned int i = 0; i < cuerpo.size();i++){
            if(cuerpo[i]->posX == nuevo->posX && cuerpo[i]->posY == nuevo->posY){
                muerta = true;
            }
        }

        cuerpo.push_back(nuevo);
    }
}

bool Serpiente::estaMuerta(){
    return muerta;
}

