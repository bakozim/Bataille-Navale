#ifndef BATAILLE_NAVALE_H_
#define BATAILLE_NAVALE_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <dirent.h>
#define B 1 /*presence d'un bateau*/
#define T 2 /*Touché*/
#define C 3 /*Coulé*/
#define R 9 /*Raté*/



struct joueur
{
	char nom[20]; /*Nom du joueur*/
	int grilles[10][10][2]; /*Grilles quadrillées de A à J pour les colonnes de et de 1 à 10 pour les lignes, une pour les bateux du joueur et les coups de l'adversaire et une pour ceux du joueur lui même*/
	int porteavions[5][2];
	int croiseur[4][2];
    int contretorpilleur[3][2];
    int sousmarin[3][2];
    int torpilleur[2][2];
    int gagne;
};typedef struct joueur Joueur;

int main();
int sleep();

int menuNouvellePartie();
void revisualiserPartie();
void reprisePartie();
int JvsJreprise(Joueur * joueur1, Joueur * joueur2, FILE * saveTemp);
int JvsIAreprise(Joueur * joueur1, Joueur * IA, FILE * saveTemp);

void color(int t,int f);
int randIntervalle(int a,int b);

void initialiserStructBateau(Joueur * joueur);
void initialiserGrille(int grille[10][10][2], int choixGrille);
int convertCharEnInt(char * charac);
char convertIntEnChar(int colonne);

void afficherGrille(int grille[10][10][2], int choixGrille);
void afficherCoordonneeBateaux(Joueur * joueur, int numerobat);

int placementBateauxIA(Joueur * IA, int numbat);
int entrerCordonneesBateaux(Joueur * joueur, int numerobat);
void choixBateauxPositioner(Joueur * joueur);
int verifierCaseAdjacente(Joueur * joueur, int avant[2], int arriere[2]);
int verifierPresenceBateau(Joueur * joueur, int avant[2], int arriere[2]);
void verifierBateauDejaEntree(Joueur * joueur, int numerobat);
void ajoutBateauStructureJoueur1(Joueur * joueur, int ligneArriere, int colonneAvant, int numerobat, int i);
void ajoutBateauStructureJoueur2(Joueur * joueur, int ligneAvant, int colonneAvant, int numerobat, int i);
void ajoutBateauStructureJoueur3(Joueur * joueur, int ligneAvant, int colonneArriere, int numerobat, int j);
void ajoutBateauStructureJoueur4(Joueur * joueur, int ligneAvant, int colonneAvant, int numerobat, int j);
void ajoutBateauGrille(Joueur * joueur, int avant[2], int arriere[2], int numerobat);
void ajoutBateauGrilleIA(Joueur * joueur, int avant[2], int arriere[2], int numerobat);

int phaseJeu(Joueur * joueur1, Joueur * joueur2, FILE * saveTemp);
int phaseJeuIA(Joueur * joueur1, Joueur * joueur2);
int JvsJ();
int JvsIA();
int touche(Joueur * joueur, int l, int c);
int coule(Joueur * joueur);
void miseajourCoule(Joueur * joueur, Joueur * joueur2, int numbat);
int perdu(Joueur * joueur);



#endif
