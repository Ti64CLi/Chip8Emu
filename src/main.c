#include <SDL/SDL.h>
#include <os.h>
#include "cpu.h"

void initialiserSDL();
void quitterSDL();
void pausesys();

int menu(int *, int *);

Uint8 chargerJeu(char *);
Uint8 listen(int *, int *, int *);

void dispDebug(Uint16 *, int *);
void hideDebug();
void scroll(Uint16 *, int *, Uint16);
int debugWaitKey();

char *convertOpcodeAsm(int, char *, Uint16);

Uint16 pclast;

int main(int argc, char *argv[])
{
    int fps = 4, vitessecpu = 10, boolDebug = 0, action = 0;
	Uint16 opcode; //l'opcode a afficher
	Uint16 debugline[] = {0,0,0,0,0}; //opcode affiches
	int adresse[] = {0,0,0,0,0}; //adresse des opcodes affiches

	cfg_register_fileext("ch8", "chip8_emu"); //extensions a enregistrer
	cfg_register_fileext("chip8", "chip8_emu"); //dans ndless.cfg.tns

	initialiserSDL(); //initialisation
	initialiserPixel();
	initialiserCpu();
	chargerFont();

	/*nSDL_Font *font = NULL;
	font = nSDL_LoadFont(NSDL_FONT_VGA, 255, 255, 255);*/

    Uint8 continuer=1,demarrer=0,compteur=0;

    if(argc>=2) //Permet de charger un jeu en ligne de commande ou en le plaçant dans l'exécutable
    {
      demarrer=chargerJeu(argv[1]); //charger jeu
    }

    if(demarrer==1) //si le jeu a ete charge
    {
		initialiserEcran();
		continuer = menu(&fps,&vitessecpu); //lancer le menu
		if(continuer == 2)
			boolDebug = 1;
	while(continuer)
	{
	   continuer=listen(&fps, &vitessecpu, &boolDebug); //pour les entrées utilisateur
	   if(continuer == 2)
		   boolDebug = 1;

	   for(compteur=0;compteur<vitessecpu && continuer==1;compteur++)  //Si continuer=0, on quitte l'émulateur
	   {
			pclast = cpu.pc;
			opcode = recupererOpcode(); //recupere l'opcode
			continuer=interpreterOpcode(opcode); //puis l'interprete

            if(boolDebug)
            {
                scroll(debugline, adresse, opcode); //scroll les opcodes a afficher
				dispDebug(debugline, adresse); //affiche le debugger
				action = debugWaitKey(); //attend l'appuie d'une touche valide
            }
			if(action > 0) //ESC est appuye
			{
				boolDebug = 0;
				hideDebug();
				break;
			}
			if(action < 0) //s est appuye
			{
				boolDebug = 0;
				action = 0;
				hideDebug();
			}
	   }
	   if(action > 0)
	   {
		   continuer=menu(&fps, &vitessecpu);
		   action = 0;
	   }

			updateEcran();
			decompter();

           SDL_Delay(fps); //une pausesys de 16 ms

	}
		quitterSDL();
    }
	else //si la rom ne peut etre charge
		show_msgbox("                CHIP8 Emulateur", "Extension .ch8 et .chip8 enregistre.\nIl vous suffit maintenant d'ouvrir\nun fichier .ch8 ou .chip8\npour que l'emulateur se lance et\ncharge automatiquement la rom.");

return EXIT_SUCCESS;
}


void initialiserSDL()
{
    //atexit(quitterSDL);

    if(SDL_Init(SDL_INIT_VIDEO)==-1)
    {
        printf("Erreur lors de l'initialisation de la SDL %s",SDL_GetError());
        exit(EXIT_FAILURE);
    }

}


void quitterSDL()
{
    SDL_FreeSurface(carre[0]);
    SDL_FreeSurface(carre[1]);
	//nSDL_FreeFont(font);
	//SDL_FreeSurface(ecran);
	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

Uint8 chargerJeu(char *nomJeu)
{
    FILE *jeu=NULL;
    jeu=fopen(nomJeu,"rb");

    if(jeu!=NULL)
    {
        fread(&cpu.memoire[ADRESSEDEBUT],sizeof(Uint8)*(TAILLEMEMOIRE-ADRESSEDEBUT), 1, jeu);
        fclose(jeu);
        return 1;
    }
    else
    {
      printf("Problème d'ouverture du fichier");
      return 0;
    }

}



Uint8 listen(int *fps, int *vitessecpu, int *boolDebug)
{
    Uint8 continuer=1;

      while(SDL_PollEvent(&event))
      {
            switch(event.type)
            {
                case SDL_KEYDOWN:{
                                    switch(event.key.keysym.sym)
                                    {
										/*123C
										  456D
										  789E
										  A0BF*/
                                        case SDLK_w:{ cpu.touche[0x0]=1;break;}
                                        case SDLK_a:{ cpu.touche[0x1]=1;break;}
                                        case SDLK_b:{ cpu.touche[0x2]=1;break;}
                                        case SDLK_c:{ cpu.touche[0x3]=1;break;}
                                        case SDLK_h:{ cpu.touche[0x4]=1;break;}
                                        case SDLK_i:{ cpu.touche[0x5]=1;break;}
                                        case SDLK_j:{ cpu.touche[0x6]=1;break;}
                                        case SDLK_o:{ cpu.touche[0x7]=1;break;}
                                        case SDLK_p:{ cpu.touche[0x8]=1;break;}
                                        case SDLK_q:{ cpu.touche[0x9]=1;break;}
                                        case SDLK_v:{ cpu.touche[0xA]=1;break;}
                                        case SDLK_x:{cpu.touche[0xB]=1;break;}
                                        case SDLK_d:{cpu.touche[0xC]=1;break;}
                                        case SDLK_k:{cpu.touche[0xD]=1;break;}
                                        case SDLK_r:{cpu.touche[0xE]=1;break;}
                                        case SDLK_y:{cpu.touche[0xF]=1;break;}
                                        case SDLK_s:{*boolDebug = !(*boolDebug);if(!(*boolDebug)) hideDebug();break;}
                                        case SDLK_TAB:{pausesys();break;}
                                        case SDLK_DELETE:{reset();break;}
										case SDLK_ESCAPE: {continuer=menu(fps,vitessecpu);break;}
                                        default:{ break;}
                                    }
			        break;}
                case SDL_KEYUP:{
                                    switch(event.key.keysym.sym)
                                    {
                                        case SDLK_w:{ cpu.touche[0x0]=0;break;}
                                        case SDLK_a:{ cpu.touche[0x1]=0;break;}
                                        case SDLK_b:{ cpu.touche[0x2]=0;break;}
                                        case SDLK_c:{ cpu.touche[0x3]=0;break;}
                                        case SDLK_h:{ cpu.touche[0x4]=0;break;}
                                        case SDLK_i:{ cpu.touche[0x5]=0;break;}
                                        case SDLK_j:{ cpu.touche[0x6]=0;break;}
                                        case SDLK_o:{ cpu.touche[0x7]=0;break;}
                                        case SDLK_p:{ cpu.touche[0x8]=0;break;}
                                        case SDLK_q:{ cpu.touche[0x9]=0;break;}
                                        case SDLK_v:{ cpu.touche[0xA]=0;break;}
                                        case SDLK_x:{cpu.touche[0xB]=0;break;}
                                        case SDLK_d:{cpu.touche[0xC]=0;break;}
                                        case SDLK_k:{cpu.touche[0xD]=0;break;}
                                        case SDLK_r:{cpu.touche[0xE]=0;break;}
                                        case SDLK_y:{cpu.touche[0xF]=0;break;}
                                        default:{ break;}
                                    }
				break;}

                        default:{ break;}
            }
      }

 return continuer;
}


void pausesys()
{
    Uint8 continuer=1;

    do
    {
        SDL_WaitEvent(&event);

        switch(event.type)
         {

             case SDL_KEYDOWN:

                        if(event.key.keysym.sym==SDLK_TAB)
                           continuer=0;
					    if(event.key.keysym.sym==SDLK_ESCAPE)
							continuer=0;
                    break;

             default:   break;
         }
    }while(continuer==1);

    SDL_Delay(200); //on fait une petite pause pour ne pas prendre le joueur au dépourvu
}

int menu(int *fps, int *vitessecpu)  // merci a Alien_ pour cette fonction. Ameliore par mes soins
{
    nSDL_Font *fontselect = NULL, *font = NULL;
    //SDL_Event event;

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0,0,0));

	font = nSDL_LoadFont(NSDL_FONT_VGA, 255, 255, 255);
    fontselect = nSDL_LoadFont(NSDL_FONT_VGA, 255, 0, 0);
	//nSDL_EnableFontMonospaced(font, SDL_TRUE);
	//nSDL_EnableFontMonospaced(fontselect, SDL_TRUE);
	int ltitre, lfps /*= 0*/, lvitessecpu, lquitter, /*lreset,*/ lretour, lload;
	ltitre = nSDL_GetStringWidth(font, "CHIP8 Emulateur - Configuration");
	lquitter = nSDL_GetStringWidth(font, "5- QUITTER L'EMULATEUR"); lretour = nSDL_GetStringWidth(font, "3- CHARGER LE JEU");
	lload = nSDL_GetStringWidth(font, "4- CHARGER LE JEU AVEC LE DEBUGGER");
	//lreset = nSDL_GetStringWidth(font, "DEL- SUPPRIMER LA SAUVEGARDE");
    int select = 1;
    int ret = 1;
    int continuer = 1;

    while(continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
         {
             case SDL_KEYDOWN:
                    {
                      switch(event.key.keysym.sym)
                      {
						case SDLK_ESCAPE:
						{
							continuer = 0;
							ret = 0;
							break;
						}
                        case SDLK_1:
                        {
                            select = 1;
                            break;
                        }
                        case SDLK_2:
                            {
                                select = 2;
                                break;
                            }
                        case SDLK_5:
                            {
								if(select == 5)
								{
									continuer = 0;
									ret = 0;
								}
								else
									select = 5;
                                break;
                            }
                        case SDLK_3:
                            {
								if(select == 3)
								{
									continuer = 0;
									ret = 1;
								}
								else
									select = 3;
                                break;
                            }
						case SDLK_4:
						{
							if(select == 4)
							{
								continuer = 0;
								ret = 2;
							}
							else
								select = 4;
							break;
						}
                        case SDLK_PLUS:
                            {
                                if(select==1)
                                {
                                    ((*fps) < 50 ? (*fps)++ : 0);
                                }
                                else if(select == 2)
                                {
                                    ((*vitessecpu) < 50 ? (*vitessecpu)++ : 0);
                                }
								SDL_Delay(100);
                                break;
                            }
                        case SDLK_MINUS:
                            {
                                if(select==1)
                                {
                                   ((*fps) > 0 ? (*fps)-- : 0);
                                }
                                else if(select == 2)
                                {
                                    ((*vitessecpu) > 1 ? (*vitessecpu)-- : 0);
                                }
								SDL_Delay(100);
                                break;
                            }
						case SDLK_l:
						{
							if(select == 3)
							{
								continuer = 0;
								ret = 1;
							}
							else if(select == 4)
							{
								continuer = 0;
								ret = 2;
							}
							else if(select == 5)
							{
								continuer = 0;
								ret = 0;
							}
							break;
						}
						/*case SDLK_DELETE:
							{
								//pour reset le jeu en cours
								break;
							}*/
						default:
							break;
                      }
					  break;
                    }
             default:
                 break;
         }
		 SDL_Delay(100);

         SDL_FillRect(ecran,NULL, SDL_MapRGB(ecran->format, 0,0,0));
		 nSDL_DrawString(ecran,font,160-ltitre/2,20,"CHIP8 Emulateur - Configuration");

		 char textfps[] = "              ";
		 char textvitessecpu[] = "                      ";
		 sprintf(textfps, "1- FPS: - %d +", *fps);
		 sprintf(textvitessecpu, "2- VITESSE CPU: - %d +", *vitessecpu);
		 lfps = nSDL_GetStringWidth((select == 1 ? (fontselect) : (font)), textfps);
		 lvitessecpu = nSDL_GetStringWidth((select == 1 ? (font) : (fontselect)), textvitessecpu);
         nSDL_DrawString(ecran,(select == 1 ? (fontselect) : (font)),160-lfps/2,75,textfps);
         nSDL_DrawString(ecran,(select == 2 ? (fontselect) : (font)),160-lvitessecpu/2,105,textvitessecpu);

        nSDL_DrawString(ecran,(select == 5 ? (fontselect) : (font)),160-(lquitter/2),195,"5- QUITTER L'EMULATEUR");
		nSDL_DrawString(ecran,(select == 4 ? (fontselect) : (font)),160-(lload/2),165,"4- CHARGER LE JEU AVEC LE DEBUGGER");
        nSDL_DrawString(ecran,(select == 3 ? (fontselect) : (font)),160-(lretour/2),135,"3- CHARGER LE JEU");
		//nSDL_DrawString(ecran,font,POSX(lreset),210,"DEL- SUPPRIMER LA SAUVEGARDE");

        SDL_Flip(ecran);
    }

    nSDL_FreeFont(font);
    nSDL_FreeFont(fontselect);

	SDL_FillRect(ecran,NULL, SDL_MapRGB(ecran->format, 0,0,0));
    return ret;
}


void dispDebug(Uint16 *history, int *adresse)
{
	int yopcode = 155, yregistre = 4;
	nSDL_Font *font = NULL;
	font = nSDL_LoadFont(NSDL_FONT_VGA, 255, 255, 255);
	SDL_Rect debugger = {0, 140, 320, 100};
	SDL_Rect registre = {256, 0, 64, 140};
	SDL_FillRect(ecran, &debugger, SDL_MapRGB(ecran->format, 255, 0, 0));
	SDL_FillRect(ecran, &registre, SDL_MapRGB(ecran->format, 255, 0, 0));
	nSDL_DrawString(ecran, font, 5, 145, "ADRESSE  OPCODE  ASSEMBLEUR");

	for(int i = 0; i < 16 ; i++)
	{
		if(i < 5)
		{
			if(history[i])
			{
				char bufOpcode[30] = {0};
				convertOpcodeAsm(adresse[i], bufOpcode, history[i]);
				nSDL_DrawString(ecran, font, 5, yopcode, bufOpcode);
				yopcode += (nSDL_GetStringHeight(font, bufOpcode) + 2);
			}
		}
		char bufReg[20] = {0};
		sprintf(bufReg, "r%X = %2.2X", i, (int)cpu.V[i]);
		nSDL_DrawString(ecran, font, 262, yregistre, bufReg);
		yregistre += (nSDL_GetStringHeight(font, bufReg) + 2);
	}
	nSDL_DrawString(ecran, font, 262, yregistre, "I = %2.2X", (int)cpu.I);
	SDL_Flip(ecran);
	nSDL_FreeFont(font);
}

void hideDebug()
{
	SDL_Rect debugger = {0, 140, 320, 100};
	SDL_Rect registre = {256, 0, 64, 140};
	SDL_FillRect(ecran, &debugger, SDL_MapRGB(ecran->format, 0, 0, 0));
	SDL_FillRect(ecran, &registre, SDL_MapRGB(ecran->format, 0, 0, 0));
	SDL_Flip(ecran);
}

void scroll(Uint16 *history, int *adresse, Uint16 opcode)
{
	int change = 0;
	for(int i = 0 ; i < 5 ; i++)
	{
		if((history[i] == 0) && (!change))
		{
            history[i] = opcode;
			adresse[i] = pclast;
			change = 1;
			if(i == 0)
				break;
		}
		else if(history[i] && (i == 4))
		{
			for(int j = 0; j < 4; j++)
			{
				history[j] = history[j+1];
				adresse[j] = adresse[j+1];
			}
			history[4] = opcode;
			adresse[4] = pclast;
			
		}

	}
}

int debugWaitKey()
{
	int ret = 0, loop = 1;
	while(SDL_WaitEvent(&event) && loop)
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:{
                switch(event.key.keysym.sym)
                {
					case SDLK_RETURN:{loop--;break;}
					case SDLK_ESCAPE:{ret++;loop--;break;}
					case SDLK_s:{ret--;loop--;break;}
					default:{break;}
				}
			}
			default:
				break;
		}
	}
	return ret;
}

char *convertOpcodeAsm(int adresse, char *buf, Uint16 opcode)
{
// ************** !!! AJOUTER TOUTES LES ADRESSES !!! ************** //
		int i = recupererAction(opcode);
		
		int b3 = (opcode&(0x0F00))>>8;  //on prend les 4 bits représentant X
		int b2 = (opcode&(0x00F0))>>4;  //idem pour Y
		int b1 = (opcode&(0x000F));     //idem

		int nombre3 = (b3<<8)+(b2<<4)+b1;
		int nombre2 = (b2<<4)+b1;
		int nombre1 = b1;

		int reg1 = b3;
		int reg2 = b2;

		switch(i) // avec des morceaux de https://github.com/craigthomas/Chip8Assembler
		{
			case 0:{
                sprintf(buf,"0x%4.4X    %4.4X   SYS $%3.3X", adresse, opcode, nombre3);
                break;
			}
			case 1:{
            //00E0 efface l'écran.
                sprintf(buf,"0x%4.4X    %4.4X   CLR", adresse, opcode);
                break;
			}

			case 2:{
            //00EE revient du saut.
                sprintf(buf,"0x%4.4X    %4.4X   RTS", adresse, opcode);
                break;
            }
			case 3:{
            //1NNN effectue un saut à l'adresse NNN.
                sprintf(buf,"0x%4.4X    %4.4X   JUMP $%3.3X", adresse, opcode, nombre3); // ajouter l'adresse du saut
                break;
            }
			case 4:{
            //2NNN appelle le sous-programme en NNN, mais on revient ensuite.
                sprintf(buf,"0x%4.4X    %4.4X   CALL $%3.3X", adresse, opcode, nombre3); // ajouter l'adresse du sous programme
                break;
            }
			case 5:{
            //3XNN saute l'instruction suivante si VX est égal à NN.
                sprintf(buf,"0x%4.4X    %4.4X   SKE r%1.1X,$%2.2X", adresse, opcode, reg1, nombre2);
                break;
            }
			case 6:{
            //4XNN saute l'instruction suivante si VX et NN ne sont pas égaux.
                sprintf(buf,"0x%4.4X    %4.4X   SKNE r%1.1X,$%2.2X", adresse, opcode, reg1, nombre2);
                break;
            }
			case 7:{
           //5XY0 saute l'instruction suivante si VX et VY sont égaux.
				sprintf(buf,"0x%4.4X    %4.4X   SKRE r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
				break;
            }
			case 8:{
            //6XNN définit VX à NN.
				sprintf(buf,"0x%4.4X    %4.4X   LOAD r%1.1X,$%2.2X", adresse, opcode, reg1, nombre2);
                break;
            }
			case 9:{
				//7XNN ajoute NN à VX.
                sprintf(buf,"0x%4.4X    %4.4X   ADD r%1.1X,$%2.2X", adresse, opcode, reg1, nombre2);
                break;
            }
			case 10:{
                //8XY0 définit VX à la valeur de VY.
                sprintf(buf,"0x%4.4X    %4.4X   MOVE r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 11:{
                //8XY1 définit VX à VX OR VY.
                sprintf(buf,"0x%4.4X    %4.4X   OR r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 12:{
                //8XY2 définit VX à VX AND VY.
                sprintf(buf,"0x%4.4X    %4.4X   AND r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 13:{
                //8XY3 définit VX à VX XOR VY.
                sprintf(buf,"0x%4.4X    %4.4X   XOR r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 14:{
                //8XY4 ajoute VY à VX. VF est mis à 1 quand il y a un dépassement de mémoire (carry), et à 0 quand il n'y en pas.
                sprintf(buf,"0x%4.4X    %4.4X   ADDR r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 15:{
                //8XY5 VY est soustraite de VX. VF est mis à 0 quand il y a un emprunt, et à 1 quand il n'y a en pas.
                sprintf(buf,"0x%4.4X    %4.4X   SUB r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 16:{
                //8X06 décale (shift) VX à droite de 1 bit. VF est fixé à la valeur du bit de poids faible de VX avant le décalage.
                sprintf(buf,"0x%4.4X    %4.4X   SHR r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 17:{
                //8XY7 VX = VY - VX. VF est mis à 0 quand il y a un emprunt et à 1 quand il n'y en a pas.
				sprintf(buf,"0x%4.4X    %4.4X   SUBB r%1.1X, r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 18:{
                //8X0E décale (shift) VX à gauche de 1 bit. VF est fixé à la valeur du bit de poids fort de VX avant le décalage.
				sprintf(buf,"0x%4.4X    %4.4X   SHL r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 19:{
                //9XY0 saute l'instruction suivante si VX et VY ne sont pas égaux.
                sprintf(buf,"0x%4.4X    %4.4X   SKRNE r%1.1X,r%1.1X", adresse, opcode, reg1, reg2);
                break;
            }
			case 20:{
				//ANNN affecte NNN à I.
                sprintf(buf,"0x%4.4X    %4.4X   LOADI $%3.3X", adresse, opcode, nombre3);
                break;
            }
			case 21:{
				//BNNN passe à l'adresse NNN + V0.
				sprintf(buf,"0x%4.4X    %4.4X   JUMPI $%3.3X", adresse, opcode, nombre3 + cpu.V[0]);
				break;
            }
			case 22:{
				//CXNN définit VX à un nombre aléatoire inférieur à NN.
				sprintf(buf,"0x%4.4X    %4.4X   RAND r%1.1X,$%2.2X", adresse, opcode, reg1, nombre2);
				break;
            }
			case 23:{
				//DXYN dessine un sprite aux coordonnées (VX, VY).
				sprintf(buf,"0x%4.4X    %4.4X   DRAW r%1.1X,r%1.1X,$%1.1X", adresse, opcode, reg1, reg2, nombre1);
				break;
            }
			case 24:{
				//EX9E saute l'instruction suivante si la clé stockée dans VX est pressée.
                sprintf(buf,"0x%4.4X    %4.4X   KEYP r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 25:{
				//EXA1 saute l'instruction suivante si la clé stockée dans VX n'est pas pressée.
                sprintf(buf,"0x%4.4X    %4.4X   KEYNP r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 26:{
				//FX07 définit VX à la valeur de la temporisation.
				sprintf(buf,"0x%4.4X    %4.4X   MOVED r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 27:{
                //FX0A attend l'appui sur une touche et stocke ensuite la donnée dans VX.
                sprintf(buf,"0x%4.4X    %4.4X   KEYD r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 28:{
				//FX15 définit la temporisation à VX.
				sprintf(buf,"0x%4.4X    %4.4X   LOADD r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 29:{
				//FX18 définit la minuterie sonore à VX.
				sprintf(buf,"0x%4.4X    %4.4X   LOADS r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 30:{
				//FX1E ajoute VX à I. VF est mis à 1 quand il y a overflow (I+VX>0xFFF), et à 0 si tel n'est pas le cas.
				sprintf(buf,"0x%4.4X    %4.4X   ADDI r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 31:{
				//FX29 définit I à l'emplacement du caractère stocké dans VX. Les caractères 0-F (en hexadécimal) sont représentés par une police 4x5.
				sprintf(buf,"0x%4.4X    %4.4X   LDSPR r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 32:{
                //FX33 stocke dans la mémoire le code décimal représentant VX (dans I, I+1, I+2).
				sprintf(buf,"0x%4.4X    %4.4X   BCD r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 33:{
                //FX55 stocke V0 à VX en mémoire à partir de l'adresse I.
				sprintf(buf,"0x%4.4X    %4.4X   STOR r%1.1X", adresse, opcode, reg1);
                break;
            }
			case 34:{
				//FX65 remplit V0 à VX avec les valeurs de la mémoire à partir de l'adresse I.
				sprintf(buf,"0x%4.4X    %4.4X   READ r%1.1X", adresse, opcode, reg1);
                break;
            }
			default: { //si ça arrive, il y un truc qui cloche
				sprintf(buf,"0x%4.4X    %4.4X   UNKNOWN", adresse, opcode);
				break;
			}
		}
	return buf;
}
