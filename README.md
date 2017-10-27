# Chip8Emu

## Installation:

Envoyez le fichier chip8_emu.tns dans n'importe quel dossier sur votre calculatrice.
Ouvrez le une premiere fois, afin qu'il enregistre les extensions ch8.
Il vous suffira ensuite d'ouvrir un fichier .ch8 pour que l'emulateur lance le jeu automatiquement.

## Touches:

Voici un shema du mappage des touches:
| Nspire | CHIP8 |
|:------:|:-----:|
| ABCD   | 123C  |
| HIJK   | 456D  |
| OPQR   | 789E  |
| VWXY   | A0BF  |

Utilisez TAB pour mettre en pause.
Pour acceder au menu, appuyez sur ESC
Pour afficher/enlever le debugger, appuyez sur 's'
(Voir le fichier keyboard.png pour plus de precision)

## Debugger:

Le debugger est en mode pas a pas. Vous devez donc appuyer sur ENTER ou la touche 'Revenir a la ligne' du clavier alphabetique pour passer
a l'instructions suivante. En bas, ce sont les opcodes qui s'affichent sous la formes:
ADRESSE OPCODE ASSEMBLEUR
A gauche, vous avez les registres.
La modification des opcodes est a venir.

## Les ROM chip8:

La plupart des rom chip8 sont libres de droit et peuvent donc etre telechargees legalement et gratuitement.
Pour transferer une rom chip8 sur votre calculatrice renommez la en \*.ch8.tns
Ensuite, transferez la. Pour la lancer sur votre calculatrice, il vous suffira d'appuyer sur entrer comme si c'etait un fichier normal.
Pour que cela marche, vous devrez avoir lance au prealable le fichier chip8_emu, au moins une fois, pour la gestion des extensions.

## Compiler:

Ce projet est sous license creative commons (CC) non commercial. Il est donc libre et opensource.
Vous pouvez donc faire les modifications que vous voulez, et le compiler vous meme, vous pouvez. N'oubliez pas de nous mentionner si vous publier votre
propre version.
Pour le compiler, les sources et le makefile sont dans le dossier src. Il vous suffit d'executer make dans une console bash.

## Remerciement:

Je tiens a remercier particulierement:
  -bestcoder pour le code source original
  -Alien_ pour les differentes ameliorations ajoutes

Ce fut un projet realiser en cooperation par Alien_ et moi-meme Ti64CLi++.
Ce projet utilise la nSDL de Hoffa, un portage de la SDL pour Nspire
