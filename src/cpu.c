#include "cpu.h"


void initialiserCpu()
{
  //On initialise le tout
    Uint16 i=0;

    for(i=0;i<TAILLEMEMOIRE;i++)
    {
        cpu.memoire[i]=0;
    }

    for(i=0;i<16;i++)
    {
        cpu.V[i]=0;
        cpu.saut[i]=0;
        cpu.touche[i]=0;
    }

    cpu.pc=ADRESSEDEBUT;
    cpu.nbrsaut=0;
    cpu.compteurJeu=0;
    //cpu.compteurSon=0;
    cpu.I=0;

    initialiserJump();
}

void reset()
{
    Uint8 i=0;
    for(i=0;i<16;i++)
    {
        cpu.V[i]=0;
        cpu.saut[i]=0;
        cpu.touche[i]=0;
    }

    cpu.pc=ADRESSEDEBUT;
    cpu.nbrsaut=0;
    cpu.compteurJeu=0;
    //cpu.compteurSon=0;
    cpu.I=0;
    initialiserPixel();
    updateEcran();

}


void initialiserJump()
{

  jp.masque[0]= 0x0000; jp.id[0]=0x0FFF;          /* 0NNN */
  jp.masque[1]= 0xFFFF; jp.id[1]=0x00E0;          /* 00E0 */
  jp.masque[2]= 0xFFFF; jp.id[2]=0x00EE;          /* 00EE */
  jp.masque[3]= 0xF000; jp.id[3]=0x1000;          /* 1NNN */
  jp.masque[4]= 0xF000; jp.id[4]=0x2000;          /* 2NNN */
  jp.masque[5]= 0xF000; jp.id[5]=0x3000;          /* 3XNN */
  jp.masque[6]= 0xF000; jp.id[6]=0x4000;          /* 4XNN */
  jp.masque[7]= 0xF00F; jp.id[7]=0x5000;          /* 5XY0 */
  jp.masque[8]= 0xF000; jp.id[8]=0x6000;          /* 6XNN */
  jp.masque[9]= 0xF000; jp.id[9]=0x7000;          /* 7XNN */
  jp.masque[10]= 0xF00F; jp.id[10]=0x8000;          /* 8XY0 */
  jp.masque[11]= 0xF00F; jp.id[11]=0x8001;          /* 8XY1 */
  jp.masque[12]= 0xF00F; jp.id[12]=0x8002;          /* 8XY2 */
  jp.masque[13]= 0xF00F; jp.id[13]=0x8003;          /* BXY3 */
  jp.masque[14]= 0xF00F; jp.id[14]=0x8004;          /* 8XY4 */
  jp.masque[15]= 0xF00F; jp.id[15]=0x8005;          /* 8XY5 */
  jp.masque[16]= 0xF00F; jp.id[16]=0x8006;          /* 8XY6 */
  jp.masque[17]= 0xF00F; jp.id[17]=0x8007;          /* 8XY7 */
  jp.masque[18]= 0xF00F; jp.id[18]=0x800E;          /* 8XYE */
  jp.masque[19]= 0xF00F; jp.id[19]=0x9000;          /* 9XY0 */
  jp.masque[20]= 0xF000; jp.id[20]=0xA000;          /* ANNN */
  jp.masque[21]= 0xF000; jp.id[21]=0xB000;          /* BNNN */
  jp.masque[22]= 0xF000; jp.id[22]=0xC000;          /* CXNN */
  jp.masque[23]= 0xF000; jp.id[23]=0xD000;          /* DXYN */
  jp.masque[24]= 0xF0FF; jp.id[24]=0xE09E;          /* EX9E */
  jp.masque[25]= 0xF0FF; jp.id[25]=0xE0A1;          /* EXA1 */
  jp.masque[26]= 0xF0FF; jp.id[26]=0xF007;          /* FX07 */
  jp.masque[27]= 0xF0FF; jp.id[27]=0xF00A;          /* FX0A */
  jp.masque[28]= 0xF0FF; jp.id[28]=0xF015;          /* FX15 */
  jp.masque[29]= 0xF0FF; jp.id[29]=0xF018;          /* FX18 */
  jp.masque[30]= 0xF0FF; jp.id[30]=0xF01E;          /* FX1E */
  jp.masque[31]= 0xF0FF; jp.id[31]=0xF029;          /* FX29 */
  jp.masque[32]= 0xF0FF; jp.id[32]=0xF033;          /* FX33 */
  jp.masque[33]= 0xF0FF; jp.id[33]=0xF055;          /* FX55 */
  jp.masque[34]= 0xF0FF; jp.id[34]=0xF065;          /* FX65 */

}

Uint8 recupererAction(Uint16 opcode)
{
    Uint8 action;
    Uint16 resultat;

    for(action=0; action<NBROPCODE;action++)
    {
        resultat= (jp.masque[action]&opcode);  /* On récupère les bits concernés par le test */

        if(resultat == jp.id[action]) /* On a trouvé l'action à effectuer */
           break; /* Plus la peine de continuer la boucle */
    }

    return action;
}

void decompter()
{
    if(cpu.compteurJeu>0)
    cpu.compteurJeu--;

    /*if(cpu.compteurSon>0)
    cpu.compteurSon--;*/
}

Uint16 recupererOpcode()
{
    return (cpu.memoire[cpu.pc]<<8)+cpu.memoire[cpu.pc+1];
}


Uint8 interpreterOpcode(Uint16 opcode)
{
    Uint8 continuer=1;
    Uint8 b4,b3,b2,b1;

    b3=(opcode&(0x0F00))>>8;  //on prend les 4 bits représentant X
    b2=(opcode&(0x00F0))>>4;  //idem pour Y
    b1=(opcode&(0x000F));     //idem

    b4= recupererAction(opcode);

    switch(b4)
    {
     case 0:{
               //Cet opcode n'est pas implémenté.
                break;
              }
     case 1:{
            //00E0 efface l'écran.
                effacerEcran();
                break;
               }

     case 2:{
            //00EE revient du saut.

                if(cpu.nbrsaut>0)
                {
                    cpu.nbrsaut--;
                    cpu.pc=cpu.saut[cpu.nbrsaut];
                }
                break;
            }
    case 3:{
            //1NNN effectue un saut à l'adresse 1NNN.

                cpu.pc=(b3<<8)+(b2<<4)+b1; //on prend le nombre NNN (pour le saut)
                cpu.pc-=2; //on verra pourquoi à la fin

                break;
            }
    case 4:{
            //2NNN appelle le sous-programme en NNN, mais on revient ensuite.

                cpu.saut[cpu.nbrsaut]=cpu.pc; //on reste là où on était

                if(cpu.nbrsaut<15)
                {
                    cpu.nbrsaut++;
                }

                cpu.pc=(b3<<8)+(b2<<4)+b1; //on prend le nombre NNN (pour le saut)
                cpu.pc-=2; //on verra pourquoi à la fin

                break;
            }
    case 5:{
            //3XNN saute l'instruction suivante si VX est égal à NN.

                if(cpu.V[b3]==((b2<<4)+b1))
                {
                    cpu.pc+=2;
                }

                break;
            }
    case 6:{
            //4XNN saute l'instruction suivante si VX et NN ne sont pas égaux.
                if(cpu.V[b3]!=((b2<<4)+b1))
                {
                    cpu.pc+=2;
                }

                break;
            }
    case 7:{
           //5XY0 saute l'instruction suivante si VX et VY sont égaux.
                if(cpu.V[b3]==cpu.V[b2])
                {
                    cpu.pc+=2;
                }

                break;
            }

    case 8:{
            //6XNN définit VX à NN.
                cpu.V[b3]=(b2<<4)+b1;
                break;
            }
    case 9:{
                //7XNN ajoute NN à VX.
                cpu.V[b3]+=(b2<<4)+b1;

                break;
            }
    case 10:{
                //8XY0 définit VX à la valeur de VY.
                cpu.V[b3]=cpu.V[b2];

                break;
            }
    case 11:{
                //8XY1 définit VX à VX OR VY.
                cpu.V[b3]=cpu.V[b3]|cpu.V[b2];

                break;
            }
    case 12:{
                //8XY2 définit VX à VX AND VY.
                cpu.V[b3]=cpu.V[b3]&cpu.V[b2];

                break;
            }
    case 13:{
                //8XY3 définit VX à VX XOR VY.
                cpu.V[b3]=cpu.V[b3]^cpu.V[b2];

                break;
            }
    case 14:{
                //8XY4 ajoute VY à VX. VF est mis à 1 quand il y a un dépassement de mémoire (carry), et à 0 quand il n'y en pas.
                if((cpu.V[b3]+cpu.V[b2])>255)
                {
                    cpu.V[0xF]=1; //cpu.V[15]
                }
                else
                {
                    cpu.V[0xF]=0; //cpu.V[15]
                }
                cpu.V[b3]+=cpu.V[b2];

                break;
            }
    case 15:{
                //8XY5 VY est soustraite de VX. VF est mis à 0 quand il y a un emprunt, et à 1 quand il n'y a en pas.

                if((cpu.V[b3]<cpu.V[b2]))
                {
                    cpu.V[0xF]=0; //cpu.V[15]
                }
                else
                {
                    cpu.V[0xF]=1; //cpu.V[15]
                }
                cpu.V[b3]-=cpu.V[b2];

                break;
            }
    case 16:{
                //8XY6 décale (shift) VX à droite de 1 bit. VF est fixé à la valeur du bit de poids faible de VX avant le décalage.
                cpu.V[0xF]=(cpu.V[b3]&(0x01));
                cpu.V[b3]=(cpu.V[b3]>>1);

                break;
            }
    case 17:{
                //8XY7 VX = VY - VX. VF est mis à 0 quand il y a un emprunt et à 1 quand il n'y en a pas.
                if((cpu.V[b2]<cpu.V[b3]))
                {
                    cpu.V[0xF]=0; //cpu.V[15]
                }
                else
                {
                    cpu.V[0xF]=1; //cpu.V[15]
                }
                cpu.V[b3]=cpu.V[b2]-cpu.V[b3];

                break;
            }
    case 18:{
                //8XYE décale (shift) VX à gauche de 1 bit. VF est fixé à la valeur du bit de poids fort de VX avant le décalage.
                cpu.V[0xF]=(cpu.V[b3]>>7);
                cpu.V[b3]=(cpu.V[b3]<<1);

                break;
             }

    case 19:{
                //9XY0 saute l'instruction suivante si VX et VY ne sont pas égaux.
                if(cpu.V[b3]!=cpu.V[b2])
                    {
                        cpu.pc+=2;
                    }

                break;
            }
    case 20:{
            //ANNN affecte NNN à I.

                cpu.I=(b3<<8)+(b2<<4)+b1;

                break;
            }
    case 21:{
           //BNNN passe à l'adresse NNN + V0.

            cpu.pc=(b3<<8)+(b2<<4)+b1+cpu.V[0];
            cpu.pc-=2;

            break;

            }
    case 22:{
            //CXNN définit VX à un nombre aléatoire inférieur à NN.
            cpu.V[b3]=(rand())%((b2<<4)+b1+1);

            break;

            }

    case 23:{
           //DXYN dessine un sprite aux coordonnées (VX, VY).


            dessinerEcran(b1,b2,b3) ;

            break;

            }
    case 24:{
               //EX9E saute l'instruction suivante si la clé stockée dans VX est pressée.
                if(cpu.touche[cpu.V[b3]]==1)//1 pressé, 0 relaché
                {
                    cpu.pc+=2;
                }

                break;
            }
    case 25:{
            //EXA1 saute l'instruction suivante si la clé stockée dans VX n'est pas pressée.
                if(cpu.touche[cpu.V[b3]]==0)//1 pressé, 0 relaché
                {
                    cpu.pc+=2;
                }

                break;
            }

    case 26:{
                 //FX07 définit VX à la valeur de la temporisation.
                cpu.V[b3]=cpu.compteurJeu;

                break;
            }
    case 27:{
                //FX0A attend l'appui sur une touche et stocke ensuite la donnée dans VX.
                continuer=attendAppui(b3);

                break;
            }


    case 28:{
               //FX15 définit la temporisation à VX.
                cpu.compteurJeu=cpu.V[b3];

                break;
            }
    case 29:{
                //FX18 définit la minuterie sonore à VX.
                //cpu.compteurSon=cpu.V[b3];

                break;
            }
    case 30:{
             //FX1E ajoute VX à I. VF est mis à 1 quand il y a overflow (I+VX>0xFFF), et à 0 si tel n'est pas le cas.

                if((cpu.I+cpu.V[b3])>0xFFF)
                {
                    cpu.V[0xF]=1;
                }
                else
                {
                    cpu.V[0xF]=0;
                }
                cpu.I+=cpu.V[b3];


                break;
            }

    case 31:{
                 //FX29 définit I à l'emplacement du caractère stocké dans VX. Les caractères 0-F (en hexadécimal) sont représentés par une police 4x5.
                cpu.I=cpu.V[b3]*5;

                break;
            }

    case 32:{
                //FX33 stocke dans la mémoire le code décimal représentant VX (dans I, I+1, I+2).

                cpu.memoire[cpu.I]=(cpu.V[b3]-cpu.V[b3]%100)/100;
                cpu.memoire[cpu.I+1]=(((cpu.V[b3]-cpu.V[b3]%10)/10)%10);
                cpu.memoire[cpu.I+2]=cpu.V[b3]-cpu.memoire[cpu.I]*100-10*cpu.memoire[cpu.I+1];

                break;
            }
    case 33:{
                //FX55 stocke V0 à VX en mémoire à partir de l'adresse I.
                Uint8 i=0;
                for(i=0;i<=b3;i++)
                {
                    cpu.memoire[cpu.I+i]=cpu.V[i];
                }

                break;
            }
    case 34:{
                 //FX65 remplit V0 à VX avec les valeurs de la mémoire à partir de l'adresse I.

                Uint8 i=0;

                for(i=0;i<=b3;i++)
                {
                  cpu.V[i]=cpu.memoire[cpu.I+i];
                }

                break;
            }

    default: { //si ça arrive, il y un truc qui cloche

                    break;
             }

}
    cpu.pc+=2; //on passe au prochain opcode
    return continuer;
}



void dessinerEcran(Uint8 b1,Uint8 b2, Uint8 b3)
{
    Uint8 x=0,y=0,k=0,codage=0,j=0,decalage=0;
    cpu.V[0xF]=0;

     for(k=0;k<b1;k++)
        {
            codage=cpu.memoire[cpu.I+k]; //on récupère le codage de la ligne à dessiner

            y=(cpu.V[b2]+k)%L; //on calcule l'ordonnée de la ligne à dessiner, on ne doit pas dépasser L

            for(j=0,decalage=7;j<8;j++,decalage--)
             {
                x=(cpu.V[b3]+j)%l; //on calcule l'abscisse, on ne doit pas dépasser l

                        if(((codage)&(0x1<<decalage))!=0) //on récupère le bit correspondant
                        {   //si c'est blanc
                            if( pixel[x][y].couleur==BLANC) //le pixel était blanc
                            {
                                pixel[x][y].couleur=NOIR; //on l'éteint
                                cpu.V[0xF]=1; //il y a donc collusion
                            }
                            else //sinon
                            {
                                 pixel[x][y].couleur=BLANC; //on l'allume
                            }


                        }
            }
        }

}

void chargerFont()
{
     cpu.memoire[0]=0xF0;cpu.memoire[1]=0x90;cpu.memoire[2]=0x90;cpu.memoire[3]=0x90; cpu.memoire[4]=0xF0; //O

    cpu.memoire[5]=0x20;cpu.memoire[6]=0x60;cpu.memoire[7]=0x20;cpu.memoire[8]=0x20;cpu.memoire[9]=0x70; //1

    cpu.memoire[10]=0xF0;cpu.memoire[11]=0x10;cpu.memoire[12]=0xF0;cpu.memoire[13]=0x80; cpu.memoire[14]=0xF0; //2

    cpu.memoire[15]=0xF0;cpu.memoire[16]=0x10;cpu.memoire[17]=0xF0;cpu.memoire[18]=0x10;cpu.memoire[19]=0xF0; //3

    cpu.memoire[20]=0x90;cpu.memoire[21]=0x90;cpu.memoire[22]=0xF0;cpu.memoire[23]=0x10;cpu.memoire[24]=0x10; //4

    cpu.memoire[25]=0xF0;cpu.memoire[26]=0x80;cpu.memoire[27]=0xF0;cpu.memoire[28]=0x10;cpu.memoire[29]=0xF0; //5

    cpu.memoire[30]=0xF0;cpu.memoire[31]=0x80;cpu.memoire[32]=0xF0;cpu.memoire[33]=0x90;cpu.memoire[34]=0xF0; //6

    cpu.memoire[35]=0xF0;cpu.memoire[36]=0x10;cpu.memoire[37]=0x20;cpu.memoire[38]=0x40;cpu.memoire[39]=0x40; //7

    cpu.memoire[40]=0xF0;cpu.memoire[41]=0x90;cpu.memoire[42]=0xF0;cpu.memoire[43]=0x90;cpu.memoire[44]=0xF0; //8

    cpu.memoire[45]=0xF0;cpu.memoire[46]=0x90;cpu.memoire[47]=0xF0;cpu.memoire[48]=0x10;cpu.memoire[49]=0xF0; //9

    cpu.memoire[50]=0xF0;cpu.memoire[51]=0x90;cpu.memoire[52]=0xF0;cpu.memoire[53]=0x90;cpu.memoire[54]=0x90; //A

    cpu.memoire[55]=0xE0;cpu.memoire[56]=0x90;cpu.memoire[57]=0xE0;cpu.memoire[58]=0x90;cpu.memoire[59]=0xE0; //B

    cpu.memoire[60]=0xF0;cpu.memoire[61]=0x80;cpu.memoire[62]=0x80;cpu.memoire[63]=0x80;cpu.memoire[64]=0xF0; //C

    cpu.memoire[65]=0xE0;cpu.memoire[66]=0x90;cpu.memoire[67]=0x90;cpu.memoire[68]=0x90;cpu.memoire[69]=0xE0; //D

    cpu.memoire[70]=0xF0;cpu.memoire[71]=0x80;cpu.memoire[72]=0xF0;cpu.memoire[73]=0x80;cpu.memoire[74]=0xF0; //E

    cpu.memoire[75]=0xF0;cpu.memoire[76]=0x80;cpu.memoire[77]=0xF0;cpu.memoire[78]=0x80;cpu.memoire[79]=0x80; //F

    //OUF !

}


Uint8 attendAppui(Uint8 b3)
{
    Uint8 attend=1,continuer=1;

    while(attend)
    {
        SDL_WaitEvent(&event);

            switch(event.type)
            {
                case SDL_KEYDOWN:{

                    switch(event.key.keysym.sym)
                    {

                            case SDLK_w:{ cpu.V[b3]=0x0; cpu.touche[0x0]=1; attend=0;break;}
                            case SDLK_a:{ cpu.V[b3]=0x1; cpu.touche[0x1]=1; attend=0;break;}
                            case SDLK_b:{ cpu.V[b3]=0x2; cpu.touche[0x2]=1; attend=0;break;}
                            case SDLK_c:{ cpu.V[b3]=0x3; cpu.touche[0x3]=1; attend=0;break;}
                            case SDLK_h:{ cpu.V[b3]=0x4; cpu.touche[0x4]=1; attend=0;break;}
                            case SDLK_i:{ cpu.V[b3]=0x5; cpu.touche[0x5]=1; attend=0;break;}
                            case SDLK_j:{ cpu.V[b3]=0x6; cpu.touche[0x6]=1; attend=0;break;}
                            case SDLK_o:{ cpu.V[b3]=0x7; cpu.touche[0x7]=1; attend=0;break;}
                            case SDLK_p:{ cpu.V[b3]=0x8; cpu.touche[0x8]=1; attend=0;break;}
                            case SDLK_q:{ cpu.V[b3]=0x9; cpu.touche[0x9]=1; attend=0;break;}
                            case SDLK_v:{        cpu.V[b3]=0xA;       cpu.touche[0xA]=1;  attend=0;break;}
                            case SDLK_x:{    cpu.V[b3]=0xB;     cpu.touche[0xB]=1;  attend=0;break;}
                            case SDLK_d:{  cpu.V[b3]=0xC;     cpu.touche[0xC]=1;  attend=0;break;}
                            case SDLK_k:{     cpu.V[b3]=0xD;     cpu.touche[0xD]=1;  attend=0;break;}
                            case SDLK_r:{      cpu.V[b3]=0xE;     cpu.touche[0xE]=1;  attend=0;break;}
                            case SDLK_y:{     cpu.V[b3]=0xF;     cpu.touche[0xF]=1;  attend=0;break;}
							case SDLK_ESCAPE:{ continuer=0; attend=0;break;}
                            default:{ break;}
                    } break;}

          default:{ break;}
      }
    }


 return continuer;
}
