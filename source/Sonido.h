#ifndef SONIDO_H
#define SONIDO_H

class Sonido{

private:
    static Sonido* instanciaSonido;

public:
    static Sonido* Instancia();
    static void Release();

private:
    Sonido();
    ~Sonido();

};


#endif // SONIDO_H
