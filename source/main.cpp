///Prueba de libnds por kevin20012
#include "GameManager.h"

/* El sistema de coordenadas va desde 0 a 255 para el eje x y de 0 a 191 para el eje y
// esto es para ambas pantallas del ds
*/
GameManager* kGameManager;

int main(void){

    kGameManager = GameManager::Instacia();

    kGameManager->Run();

    /*
    mmInitDefaultMem((mm_addr)soundbank_bin);

    //mmLoad( MOD_FLATOUTLIES );

    mmLoadEffect(SFX_COMER);

    mm_sound_effect comer = {
		{ SFX_COMER } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		125,	// panning
	};

    mm_sfxhand sonido = 0;
    mmEffectEx(&comer);

    */

}

