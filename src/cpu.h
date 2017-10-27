#ifndef CPU_H
#define CPU_H
#include "pixel.h"

#define TAILLEMEMOIRE 4096
#define ADRESSEDEBUT 512
#define NBROPCODE 35

    typedef struct
    {
        Uint8 memoire[TAILLEMEMOIRE];
        Uint8 V[16]; //le registre
        Uint16 I; //stocke une adresse mémoire ou dessinateur
        Uint16 saut[16]; //pour gérer les sauts dans memoire, 16 au maximum
        Uint8 nbrsaut; //stocke le nombre de sauts effectués pour ne pas dépasser 16
        Uint8 compteurJeu; //compteur pour le graphisme (fréquence de rafraîchissement)
        //Uint8 compteurSon; //compteur pour le son
        Uint16 pc; //pour parcourir le tableau memoire
        Uint8 touche[16]; //pour stocker l'état des touches
    } CPU;

CPU cpu;

typedef struct
{
    Uint16 masque[NBROPCODE];   //la Chip 8 peut effectuer 35 opérations, chaque opération possédant son masque
    Uint16 id[NBROPCODE];   //idem, chaque opération possède son propre identifiant

}JUMP;

JUMP jp;

void initialiserJump();
void initialiserCpu();
void decompter();
void chargerFont();
void dessinerEcran(Uint8,Uint8,Uint8);
void reset();

Uint16 recupererOpcode();
Uint8 interpreterOpcode(Uint16);
Uint8 recupererAction(Uint16);
Uint8 attendAppui(Uint8);

#endif
