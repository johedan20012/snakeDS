#ifndef SERPIENTE_H
#define SERPIENTE_H
#include "MathHelper.h"
#include <vector>
#include <nds.h>

struct CuerpoSerpiente{
    int posX,posY;
};

class Serpiente{

public:
    enum DIRECCION {derecha,izquierda,arriba,abajo};

private:

    DIRECCION direccionCabeza;
    int velMovimiento;
    int contMovimiento;
    bool moverse;
    bool muerta;
    int velocidadX;
    int velocidadY;


    u16* mapaTiles;///memVramMap; ///Esto es un puntero a la parte de la memoria de video donde esta el mapa del fondo 1
  public:  u16* sprPtr;  ///Esto es un puntero a la dirección de memoria donde se guarda la informacion del sprite de la serpiente
                  ///que solo es la cabeza
           int alfa = 0;

public:
    std::vector <CuerpoSerpiente*> cuerpo;
    unsigned int tam;

    //int16_t mapaTiles[1024];

public:
    Serpiente(u16* mapEnVram, u16* oamMemory);
    ~Serpiente();

    void Crecer();
    void Render();
    void Update();
    bool estaMuerta();

};

#endif // SERPIENTE_H
