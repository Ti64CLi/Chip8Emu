#ifndef PIXEL_H
#define PIXEL_H
#include <SDL/SDL.h>

#define NOIR  0
#define BLANC 1
#define l 64
#define L 32
#define DIMPIXEL 4
#define WIDTH   l*DIMPIXEL
#define HEIGHT  L*DIMPIXEL

typedef struct
{
    SDL_Rect position; //regroupe l'abscisse et l'ordonnée
    Uint32 couleur;   //comme son nom l'indique, c'est la couleur
} PIXEL;

SDL_Surface *ecran,*carre[2];
PIXEL pixel[l][L];
SDL_Event event;

void initialiserEcran();
void initialiserPixel();
void dessinerPixel(PIXEL pixel);
void effacerEcran();
void updateEcran();


#endif
