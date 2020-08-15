#include "Fruta.h"

Fruta::Fruta(u16* ptrMapaEnVram){
    mapaEnVram = ptrMapaEnVram;

    pos.x = (rand() %16)*2;
    pos.y = (rand() %12)*2;
}

Fruta::~Fruta(){
}

void Fruta::Render(){
    mapaEnVram[offsetDeMapa(pos.x,pos.y,32)] = 19;
    mapaEnVram[offsetDeMapa(pos.x+1,pos.y,32)] = 20;
    mapaEnVram[offsetDeMapa(pos.x,pos.y+1,32)] = 21;
    mapaEnVram[offsetDeMapa(pos.x+1,pos.y+1,32)] = FLIPH_TILE(21);
}

void Fruta::Reubicar(){
    pos.x = (rand() %16)*2;
    pos.y = (rand() %12)*2;
}
