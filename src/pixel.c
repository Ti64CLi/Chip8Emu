#include "pixel.h"

void initialiserPixel()
{

    Uint8 x=0,y=0;

    for(x=0;x<l;x++)
    {
        for(y=0;y<L;y++)
        {
            pixel[x][y].position.x=x*DIMPIXEL;
            pixel[x][y].position.y=y*DIMPIXEL;
            pixel[x][y].couleur=NOIR;
        }
    }

}


void initialiserEcran()
{
    ecran=NULL;
    carre[0]=NULL;
    carre[1]=NULL;

    ecran=SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE );
    //SDL_WM_SetCaption("BC-Chip8 By BestCoder",NULL);

    if(ecran==NULL)
    {
        printf("Erreur lors du chargement du mode vidéo %s",SDL_GetError());
        exit(EXIT_FAILURE);
    }


    carre[0]=SDL_CreateRGBSurface(SDL_HWSURFACE,DIMPIXEL,DIMPIXEL,32,0,0,0,0); //le pixel noir
	
    if(carre[0]==NULL)
    {
       printf("Erreur lors du chargement de la surface %s",SDL_GetError());
       exit(EXIT_FAILURE);
    }

    SDL_FillRect(carre[0],NULL,SDL_MapRGB(carre[0]->format,0x00,0x00,0x00)); //le pixel noir

    carre[1]=SDL_CreateRGBSurface(SDL_HWSURFACE,DIMPIXEL,DIMPIXEL,32,0,0,0,0); //le pixel blanc
	
     if(carre[1]==NULL)
     {
	printf("Erreur lors du chargement de la surface %s",SDL_GetError());
        exit(EXIT_FAILURE);
     }

    SDL_FillRect(carre[1],NULL,SDL_MapRGB(carre[1]->format,0xFF,0xFF,0xFF));  //le pixel blanc
}

void dessinerPixel(PIXEL pixel)
{
 /* pixel.couleur peut prendre deux valeurs : 0, auquel cas on dessine le pixel en noir, ou 1, on dessine alors le pixel en blanc */
    SDL_BlitSurface(carre[pixel.couleur],NULL,ecran,&pixel.position);
}

void effacerEcran()
{
    //Pour effacer l'écran, on remet tous les pixels en noir
    Uint8 x=0,y=0;
    for(x=0;x<l;x++)
    {
        for(y=0;y<L;y++)
        {
            pixel[x][y].couleur=NOIR;
        }
    }

  //on repeint l'écran en noir
    SDL_FillRect(ecran,NULL,NOIR);
}

void updateEcran()
{
  //On dessine tous les pixels à l'écran
   Uint8 x=0,y=0;

	for(x=0;x<l;x++)
	{
	   for(y=0;y<L;y++)
                {
                   dessinerPixel(pixel[x][y]);
		}
	}

  SDL_Flip(ecran); //on affiche les modifications
}
