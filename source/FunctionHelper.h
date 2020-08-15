#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H
#include <nds.h>

///Pone el CPU ARM9 en modo sleep durante nFrames
inline void esperar(unsigned int nFrames){
    for(unsigned int a=0; a<nFrames; a++){
        swiWaitForVBlank();
    }
}


#endif // FUNCTIONHELPER_H

