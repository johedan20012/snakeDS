#ifndef FRUTA_H
#define FRUTA_H

#include <nds.h>
#include "MathHelper.h"

class Fruta{

private:
    u16* mapaEnVram;

public:
    vec2 pos;

public:
    Fruta(u16* ptrMapaEnVram);
    ~Fruta();

    void Render();
    void Reubicar();
};

#endif // FRUTA_H
