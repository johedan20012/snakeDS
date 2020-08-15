#include "Sonido.h"

Sonido* Sonido::instanciaSonido = nullptr;

Sonido* Sonido::Instancia(){
    if(instanciaSonido == nullptr){
        instanciaSonido = new Sonido();
    }

    return instanciaSonido;
}

void Sonido::Release(){
    delete instanciaSonido;
    instanciaSonido = nullptr;
}

Sonido::Sonido(){
}

Sonido::~Sonido(){
}
