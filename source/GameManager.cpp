#include "GameManager.h"

GameManager* GameManager::sInstancia = nullptr;

GameManager* GameManager::Instacia(){
    if(sInstancia == nullptr){
        sInstancia = new GameManager();
    }

    return sInstancia;
}

GameManager::GameManager(){

}

GameManager::~GameManager(){
}

void GameManager::Liberar(){
    delete sInstancia;
    sInstancia = nullptr;
}

void GameManager::Run(){
    srand(time(nullptr));

    powerOn(POWER_ALL_2D); ///Prende todos los engine 2D

    ///Mapea el banco A y B para la memoria de los fondos del engine principal y mapea el banco C a la memoria de
    ///fondo del engine secundario
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    ///Mapea l banco E al inicio de la memoria de video par asprites
    vramSetBankE(VRAM_E_MAIN_SPRITE);

    ///Pone a ambos engines en modo 5
    videoSetMode(MODE_5_2D|
                 DISPLAY_BG2_ACTIVE);

    videoSetModeSub(MODE_5_2D|
                    DISPLAY_BG0_ACTIVE);

    ///Inicializa el OAM(Object Attribute Memory) necesario para los sprites, del engine principal
    oamInit(&oamMain,SpriteMapping_Bmp_1D_128, false);

    PrintConsole *console = consoleInit(0,1, BgType_Text4bpp, BgSize_T_256x256, 1, 1, false, false);

	ConsoleFont font;

	font.gfx = (u16*)fontTiles;
	font.pal = (u16*)fontPal;
	font.numChars = 95;
	font.numColors =  fontPalLen / 2;
	font.bpp = 4;
	font.asciiOffset = 32;
	font.convertSingleColor = false;

	consoleSetFont(console, &font);

	if(!fatInitDefault()){
        iprintf("No se pudo iniciar libfat xD");
    }

    fondo2 = bgInit(2,BgType_Bmp8, BgSize_B8_128x128, 0,0);
    dmaCopy(logoInicioBitmap, bgGetGfxPtr(fondo2), logoInicioBitmapLen);
    dmaCopy(logoInicioPal, BG_PALETTE+16, logoInicioPalLen);
    BG_PALETTE[0] = BG_PALETTE[16];
    BG_PALETTE[1] = BG_PALETTE[17];
    BG_PALETTE[2] = BG_PALETTE[18];
    bgSetScroll(fondo2,-83,-81);
    bgUpdate();

    for(int i  = 0; i<5; i++){
        int offset = (i*2) + 19;
        BG_PALETTE[offset] = BG_PALETTE[offset-2];
        BG_PALETTE[offset+1] = BG_PALETTE[offset-1];
        for(int j = 0; j<2; j++){
            u16 hola = BG_PALETTE[offset+j];
            int r = hola&31;
            int g = (hola&992)>>5;
            int b = (hola&31744)>>10;
            hola &= (1<<15);
            r /= 2;
            g /= 2;
            b /= 2;
            BG_PALETTE[offset+j] = hola | r | (g<<5) | (b<<10);
        }
    }

    for(int i  = 5; i>=0; i--){
        BG_PALETTE[1] = BG_PALETTE[17+(i*2)];
        BG_PALETTE[2] = BG_PALETTE[18+(i*2)];
        bgUpdate();
        esperar(5);
    }

    esperar(100);

    for(int i  = 0; i<6; i++){
        BG_PALETTE[1] = BG_PALETTE[17+(i*2)];
        BG_PALETTE[2] = BG_PALETTE[18+(i*2)];
        bgUpdate();
        esperar(5);
    }

    fondo2 = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    fondo2Sub = bgInitSub(0, BgType_Text4bpp, BgSize_T_256x256, 0, 2);

    dmaCopy(fondoInicioBitmap, bgGetGfxPtr(fondo2), 256*256*2);
    dmaCopy(fondoJuegoSubTiles, bgGetGfxPtr(fondo2Sub), fondoJuegoSubTilesLen);
    dmaCopy(fondoJuegoSubMap, bgGetMapPtr(fondo2Sub), fondoJuegoSubMapLen);
    dmaCopy(fondoJuegoSubPal, BG_PALETTE_SUB +16, fondoJuegoSubPalLen);
    bgSetPriority(fondo2Sub,3);

    ///Leo el highscore guardado

    FILE *highScoreFile = fopen("snake.sav", "rb");
    if(highScoreFile){
        fread(&mPuntajeAlto, sizeof(int), 1,highScoreFile);
        fclose(highScoreFile);
    }

    while(1){
        Update();
        Render();

        swiWaitForVBlank();
    }
}

void GameManager::Update(){
    scanKeys();

    if(mEstado == inicio){
        if(keysDown() & KEY_START){
            mEstado = jugando;
            serp = new Serpiente((u16*)bgGetMapPtr(fondo3),oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp));
            fruta = new Fruta((u16*)bgGetMapPtr(fondo3));

            //dmaCopy(snakeSpritePal, SPRITE_PALETTE, snakeSpritePalLen);
            dmaCopy(snakeSpriteBitmap, serp->sprPtr, snakeSpriteBitmapLen);
            //dmaFillHalfWords(ARGB16(1,31,0,0), serp->sprPtr, 16*16*2);
            mPuntajeAct = 0;

            bgHide(fondo2);

            fondo2 = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 1, 5);
            dmaCopy(fondoJuegoTiles, bgGetGfxPtr(fondo2), fondoJuegoTilesLen);
            dmaCopy(fondoJuegoPal, BG_PALETTE+16, fondoJuegoPalLen);
            dmaCopy(fondoJuegoMap,bgGetMapPtr(fondo2), fondoJuegoMapLen);
            bgSetPriority(fondo2,1);

            fondo3 = bgInit(1, BgType_Text4bpp, BgSize_T_256x256, 0, 4);
            dmaCopy(snakeTiles, bgGetGfxPtr(fondo3), snakeTilesLen);
            dmaCopy(snakePal, BG_PALETTE, snakePalLen);
            dmaFillHalfWords(0,bgGetMapPtr(fondo3),2048);
            bgSetPriority(fondo3,0);
        }
    }else if(mEstado == jugando){

        if(fruta->pos.x == serp->cuerpo[serp->cuerpo.size()-1]->posX && fruta->pos.y == serp->cuerpo[serp->cuerpo.size()-1]->posY){
            serp->Crecer();
            fruta->Reubicar();
            mPuntajeAct += 10;
            if(mPuntajeAct > mPuntajeAlto) mPuntajeAlto = mPuntajeAct;
            //mmEffectEx(&comer);
        }
        serp->Update();

        if(serp->estaMuerta()){
            /*mEstado = gameover;
            ///Guardo el highscore guardado

            FILE *highScoreFile = fopen("snake.sav", "wb");
            if(highScoreFile){
                fwrite(&mPuntajeAlto, sizeof(int), 1,highScoreFile);
                fclose(highScoreFile);
            }*/
        }

        if(keysDown() & KEY_A){
            serp->alfa ++;
        }
        if(keysDown() & KEY_B){
            serp->alfa --;
        }
    }else if(mEstado == gameover){
        if(keysDown() > 0){
            delete serp;
            delete fruta;
            mEstado = inicio;
        }
    }
}

void GameManager::Render(){
    if(mEstado == jugando){

        oamUpdate(&oamMain);
        iprintf("\x1b[2J");
        iprintf("Canal alfa: %d",serp->alfa);
        /*iprintf("\x1b[2J");
        iprintf("\x1b[1;2HPuntaje");
        iprintf("\x1b[3;4H%d",mPuntajeAct);
        iprintf("\x1b[1;16HPuntaje maximo");
        iprintf("\x1b[3;22H%d",mPuntajeAlto);*/

        serp->Render();

        fruta->Render();
    }

    bgUpdate();
}
