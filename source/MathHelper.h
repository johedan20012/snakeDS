#ifndef MATHHELPER_H
#define MATHHELPER_H

#define FLIPH_TILE(n) (n | (1 << 10))
#define FLIPV_TILE(n) (n | (1 << 11))
#define FLIPHV_TILE(n) (n | (3 << 10))

struct vec2{
    int x,y;

    vec2(int _x = 0,int _y = 0){
        x = _x;
        y = _y;
    }
};

inline int escalar(int numero,int tamCasilla){
    numero = numero*tamCasilla;
    return numero;
}

inline int anguloAEntero32B(int angulo){
    float res = angulo;
    res /= 360;
    res *= 32767;

    return res;
}

inline int offsetDeMapa(int x,int y, int width){ ///Regresa el offset que se debe añadir al puntero inicial de un mapa con anchura "width"  en la posicion (x,y)
    return (y*width) + x;
}

#endif // MATHHELPER_H
