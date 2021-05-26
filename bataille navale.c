#include "bataille navale.h"

int menuNouvellePartie()
{
    system("cls"); /* Nettoie la console de commande*/
    int choix2 = 0;

    printf("\n");
    printf("1: Joueur contre Joueur\n");
    printf("2: Joueur contre Ordinateur\n");
    printf("3: Retourner au menu principale\n");
    printf("\nEntrer votre choix : ");

    scanf("%d", &choix2);

    switch(choix2)
    {
        case 1:
            JvsJ();
            break;
        case 2 :
            JvsIA();
            break;
        case 3 :
            system("cls");
            break;
    }
    return main();
}

void revisualiserPartie()
{
    /*Toute cette partie permet d'afficher toutes les sauvegardes enregistré par le joueur, ce qui permet d'entrer le nom des sauvegardes plus facilement*/
    struct dirent * lecture;
    DIR *rep;
    rep = opendir(".");
    while ((lecture = readdir(rep)))
    {
        if (strstr(lecture->d_name, ".save"))
        {
            printf("\n%s", lecture->d_name);
        }
    }
    closedir(rep);

    /*Variable qui stock le nom des fichier sauvegarde*/
    char CsaveTemp[100];

    printf("\n\nEntrer le nom de la sauvegarde : ");
    scanf("%99s", CsaveTemp);
    getchar();


    system("cls");

    /*Initilisation du fichier sauvegarde*/
    FILE * saveTemp;

    /*ouverture du fichiers sauvergarde en mode lecture*/
    saveTemp = fopen(CsaveTemp, "rb");
    if (saveTemp==NULL)
    {
        printf("\nErreur : Le fichier sauvegarde joueur 1 est errone");
        revisualiserPartie();
    }


    /*Place le curseur au debut des fichiers*/
    rewind(saveTemp);

    /*Permet d'afficher les bateaux, les grilles et la victoire des joueurs tour par tour*/
    while(!feof(saveTemp))
    {
            /*Structure joueur temporaire qui contient la structure joueur du coup joué*/
            Joueur * test3 = malloc(sizeof(Joueur));
            fread(test3 , sizeof(Joueur), 1, saveTemp);

            printf("\n%s :",test3->nom);
            printf("\nVotre flotte :");
            afficherCoordonneeBateaux(test3, 0);
            afficherCoordonneeBateaux(test3, 1);
            afficherCoordonneeBateaux(test3, 2);
            afficherCoordonneeBateaux(test3, 3);
            afficherCoordonneeBateaux(test3, 4);

            printf("\nGrille de votre flotte & des tirs sur votre flotte :");
            afficherGrille(test3->grilles, 0);

            printf("\nGrille de vos tirs :");
            afficherGrille(test3->grilles, 1);
            sleep(2);

            /*Verifie si le joueur a gagné, si oui, on sort de la boucle*/
            if(test3->gagne ==1)
            {
                printf("\n%s a gagne !\n", test3->nom);
                sleep(2);
                main();
            }
    }

    fclose(saveTemp);
    main();
}

void reprisePartie()
{
    /*Initialisation des structures joueur*/
    Joueur * joueur1 = malloc(sizeof(Joueur));
    Joueur * joueur2 = malloc(sizeof(Joueur));

    /*Toute cette partie permet d'afficher toutes les sauvegardes enregistré par le joueur, ce qui permet d'entrer le nom des sauvegardes plus facilement*/
    struct dirent * lecture;
    DIR *rep;
    rep = opendir(".");
    while ((lecture = readdir(rep)))
    {
        if (strstr(lecture->d_name, ".save"))
        {
            printf("\n%s", lecture->d_name);
        }
    }
    closedir(rep);

    /*Variable qui stock le nom des fichier sauvegarde*/
    char CsaveTemp[100];

    printf("\n\nEntrer le nom de la sauvegarde : ");
    scanf("%99s", CsaveTemp);
    getchar();
    system("cls");

    /*Initilisation du fichier sauvegarde*/
    FILE * saveTemp;

    /*ouverture du fichier sauvergarde en mode lecture et ecriture*/
    saveTemp = fopen(CsaveTemp, "rb+");
    if (saveTemp==NULL)
    {
        printf("\nErreur : Le fichier sauvegarde joueur est errone");
        reprisePartie();
    }


    /*Place le curseur au debut du fichier*/
    rewind(saveTemp);

    /*Place le curseur à la fin du fichier*/
    fseek(saveTemp,0,SEEK_END);
    /*Place le curseur au début de la derniere structure joueur*/
    fseek(saveTemp, (-1)*sizeof(Joueur), SEEK_CUR);
    /*Initialisation de la structure joueur1 a partir du fichier sauvegarde*/
    fread(joueur1 , sizeof(Joueur), 1, saveTemp);
    /*Place le curseur au début de l'avant-derniere structure joueur*/
    fseek(saveTemp, (-2)*sizeof(Joueur), SEEK_CUR);
    /*Initialisation de la structure joueur2 a partir du fichier sauvegarde*/
    fread(joueur2 , sizeof(Joueur), 1, saveTemp);


    fclose(saveTemp);

    /*On renome le fichier en saveTemp.save pour avoir le même nom de fichier à sauvegarder que dans les fonctions JvsJreprise(), JvsIAreprise() et phaseJeu()*/
    rename(CsaveTemp,"saveTemp.save");

    /*reouverture du fichier sauvergarde mais cette fois en lecture et ecriture à partir de la fin du fichier*/
    saveTemp = fopen("saveTemp.save", "ab+");
    if (saveTemp==NULL)
    {
        printf("\nErreur : Le fichier sauvegarde joueur est errone");
        reprisePartie();
    }

    /*Cette partie permet a proprement parlé de relancer la partie en faisant attention à ce que le joueur ne prenent pas les informations de l'IA*/
    if(strcmp(joueur2->nom,"IA") == 0)
    {
        JvsIAreprise(joueur1, joueur2, saveTemp);
    }
    else if (strcmp(joueur1->nom,"IA") == 0)
    {
        JvsIAreprise(joueur2, joueur1, saveTemp);
    }
    else
    {
        JvsJreprise(joueur1, joueur2, saveTemp);
    }

    /*En cas d'erreur de renommage dans les fonctions phaseJeu, JvJreprise ou JvIAreprise, on force le fichier saveTemp.save a reprendre son nom originale pour ne pas perdre la sauvegarde pas erreur*/
    rep = opendir(".");
    while ((lecture = readdir(rep)))
    {
        if (strstr(lecture->d_name, ".save"))
        {
            if(strcmp(lecture->d_name,"saveTemp.save"))
            {
                rename("saveTemp.save", CsaveTemp);
            }
        }
    }
    closedir(rep);


    main();
}

void color(int t,int f)
{
    /*Permet de changer la couleur du texte affichee par la fonction printf avec "t" et le fond avec "f"*/
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}

int randIntervalle(int a,int b)
{
    /*Permet de generer un nombre aléatoire en a(inclus) et b(exclus)*/
    int random = rand()%(b-a)+a;
    return random;
}

void initialiserStructBateau(Joueur * joueur)
{
    /*Permet d'initiliaser les valeurs des cases bateaux à 0*/
    int i;
    for (i =0; i<5; i++)
    {
        joueur->porteavions[i][0] = 0;
        joueur->porteavions[i][1] = 0;
    }
        for (i =0; i<4; i++)
    {
        joueur->croiseur[i][0] = 0;
        joueur->croiseur[i][1] = 0;

    }
        for (i =0; i<3; i++)
    {
        joueur->contretorpilleur[i][0] = 0;
        joueur->contretorpilleur[i][1] = 0;
    }
        for (i =0; i<3; i++)
    {
        joueur->sousmarin[i][0] = 0;
        joueur->sousmarin[i][1] = 0;
    }
        for (i =0; i<2; i++)
    {

        joueur->torpilleur[i][0] = 0;
        joueur->torpilleur[i][1] = 0;
    }

}

void initialiserGrille(int grille[10][10][2], int choixGrille)
{
    int x = 0;
    int y = 0;

    /*Initialise tous les cases de la grille choisies*/
    for(x = 0 ; x < 10 ; x++)
    {
        for(y = 0 ; y < 10 ; y++)
        {
            grille[x][y][choixGrille] = 0;
        }
    }
}

int convertCharEnInt(char * charac)
{
    /*Utilisation des codes ASCII pour convertir les lettres de "A à I" ou "a à i" entrée par le joueur en numéro de colonne*/

    if (*charac > 64 &&  *charac < 91)
    {
        return(*charac-65);
    }
    else if (*charac > 96 &&  *charac < 123)
    {
        return(*charac-97);
    }
    else if (*charac > 47 &&  *charac < 58)
    {
        return(*charac-48);
    }
    else
    {
        printf("Ce n'est pas un caractere valide");
    }
    return 0;
}

char convertIntEnChar(int colonne)
{


    /*Converti les entiers en Char pour les afficher avec la fonction afficherCoordonneeBateaux */
    return((char)colonne+65);
}

void afficherGrille(int grille[10][10][2], int choixGrille)
{
    /*Sert à afficher les différentes grilles des joueurs*/

    int x = 0;
    int y = 0;

    printf("\n\n   | A B C D E F G H I J |");
    printf("\n---|---------------------|\n");
    for(x = 0 ; x < 10 ; x++)
    {
        printf(" %d | ",x);
        for(y = 0 ; y < 10 ; y++)
        {
            int cases = grille[x][y][choixGrille];
            if (cases == 0)
            {
                printf("0 ");
            }
            else if (cases == 1)
            {
                color(5,0);
                printf("B ");/*affiche les 1 comme des B pour bateau avec la couleur violette sur fond noir*/
                color(15,0);
            }
            else if (cases == 2)
            {
                color(2,0);
                printf("T ");/*affiche les 2 comme des T pour touché avec la couleur verte sur fond noir*/
                color(15,0);
            }
            else if (cases == 3)
            {
                color(9,0);
                printf("C ");/*affiche les 3 comme des C pour coulé avec la couleur bleu sur fond noir*/
                color(15,0);
            }
            else if (cases == 9)
            {
                color(12,0);
                printf("R "); /*affiche les 9 comme des R pour raté avec la couleur rouge sur fond noir*/
                color(15,0);
            }
        }
        printf("|");
        printf("\n");
    }
    printf("---|---------------------|\n");
}

void afficherCoordonneeBateaux(Joueur * joueur, int numerobat)
{
    /*Permet d'afficher les coordonnees des bateaux d'un joueur sous une forme compréhensible, c'est à dire, colonne de A à I et ligne de 0 à 9*/
    switch(numerobat)
    {
        /*En fonction des coordonnées du bateau à afficher, on va convertir les données stockées sous forme d'entier dans la structure JOUEUR*/
        case 0:
            printf("\n > Porte-Avions : %c%d, %c%d, %c%d, %c%d, %c%d", convertIntEnChar(joueur->porteavions[0][0]),joueur->porteavions[0][1],convertIntEnChar(joueur->porteavions[1][0]),joueur->porteavions[1][1], convertIntEnChar(joueur->porteavions[2][0]), joueur->porteavions[2][1], convertIntEnChar(joueur->porteavions[3][0]), joueur->porteavions[3][1],convertIntEnChar(joueur->porteavions[4][0]),joueur->porteavions[4][1]);
            break;
        case 1:
            printf("\n > Croiseur  : %c%d, %c%d, %c%d, %c%d", convertIntEnChar(joueur->croiseur[0][0]),joueur->croiseur[0][1],convertIntEnChar(joueur->croiseur[1][0]),joueur->croiseur[1][1], convertIntEnChar(joueur->croiseur[2][0]), joueur->croiseur[2][1], convertIntEnChar(joueur->croiseur[3][0]), joueur->croiseur[3][1]);
            break;
        case 2:
            printf("\n > Contre-Torpilleur : %c%d, %c%d, %c%d", convertIntEnChar(joueur->contretorpilleur[0][0]),joueur->contretorpilleur[0][1],convertIntEnChar(joueur->contretorpilleur[1][0]),joueur->contretorpilleur[1][1], convertIntEnChar(joueur->contretorpilleur[2][0]), joueur->contretorpilleur[2][1]);
            break;
        case 3:
            printf("\n > Sous-Marin : %c%d, %c%d, %c%d", convertIntEnChar(joueur->sousmarin[0][0]),joueur->sousmarin[0][1],convertIntEnChar(joueur->sousmarin[1][0]),joueur->sousmarin[1][1], convertIntEnChar(joueur->sousmarin[2][0]), joueur->sousmarin[2][1]);
            break;
        case 4:
            printf("\n > Torpilleur : %c%d, %c%d", convertIntEnChar(joueur->torpilleur[0][0]),joueur->torpilleur[0][1],convertIntEnChar(joueur->torpilleur[1][0]),joueur->torpilleur[1][1]);
            break;

    }
}

int placementBateauxIA(Joueur * IA, int numbat)
{
    /*Initialisation des coordonnées des cases avant et arriere*/
    int random_avant_int[2];
    int random_arriere_int[2];

    int randomLigneArriere;
    int randomColonneArriere;
    int randomCaseArriere;


    /*Determine aléatoirement une case dans la limite de la grille*/
    int randomLigneAvant = randIntervalle(0,10);
    int randomColonneAvant = randIntervalle(0,10);
    int randomCaseAvant = IA->grilles[randomLigneAvant][randomColonneAvant][0];

    /*Permet plus tard de determiner le sens du bateau, c'est a dire la position de case Arriere par rapport a la case Avant, droite ou gauche, haut ou bas*/
    int tirageSens = randIntervalle(0,2);

    /*Verifie que la case Avant n'est pas deja occupé, si oui, on relance la fonction*/
    if (randomCaseAvant == 1)
    {
        return placementBateauxIA(IA, numbat);
    }

    else
    {
        /*Permet de savoir si le bateau sera verticale ou horizontale, aléatoirement*/
        int tirageLigneOuColonne = randIntervalle(0,2);
        switch(tirageLigneOuColonne)
        {
            /*Cas ou le bateau est a l'horizatale*/
            case 0:
                switch(numbat)
                {
                    case 0:
                        randomLigneArriere = randomLigneAvant;
                        tirageSens =  randIntervalle(0,2);

                        switch(tirageSens)
                        {
                            case 0:
                                /*Attribut la valeur de la colonne qui est deja prédeterminé par la taille du bateau*/
                                randomColonneArriere = randomColonneAvant + 4;
                                if(randomColonneArriere > 9)
                                {
                                    /*Si le tirageSens determiner aleatoire induit un depassement de grille car trop proche du bord, on inverse juste le sens*/
                                    randomColonneArriere = randomColonneAvant - 4;
                                }
                                break;
                            case 1:
                                /*Attribut la valeur de la colonne qui est deja prédeterminé par la taille du bateau*/
                                randomColonneArriere = randomColonneAvant - 4;
                                /*Si le tirageSens determiner aleatoire induit un depassement de grille car trop proche du bord, on inverse juste le sens*/
                                if(randomColonneArriere < 0)
                                {
                                    randomColonneArriere = randomColonneAvant + 4;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }

                        /*Verifie que la case Arriere generée aleatoirement n'est pas deja occupé, si oui on relance la fonction au debut*/
                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        /*Si la case n'est pas occupé on essaie alors d'ajouter le bateau*/
                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;
                            /*Avant d'ajouter le bateau on verifie si le bateau ne chevauche pas un autre bateau et qu'il n'est pas a coté d'un autre bateau*/
                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 0);
                                }
                            /*Si l'une des conditions n'est pas respecté on relance la fonction*/
                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;
                    case 1:
                        randomLigneArriere = randomLigneAvant;
                        tirageSens =  randIntervalle(0,2);

                        switch(tirageSens)
                        {
                            case 0:
                                randomColonneArriere = randomColonneAvant + 3;
                                if(randomColonneArriere > 9)
                                {
                                    randomColonneArriere = randomColonneAvant - 3;
                                }
                                break;
                            case 1:
                                randomColonneArriere = randomColonneAvant - 3;
                                if(randomColonneArriere < 0)
                                {
                                    randomColonneArriere = randomColonneAvant + 3;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }

                        randomCaseArriere= IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 1);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }

                        break;
                    case 2:
                        randomLigneArriere = randomLigneAvant;
                        tirageSens =  randIntervalle(0,2);

                        switch(tirageSens)
                        {
                            case 0:
                                randomColonneArriere = randomColonneAvant + 2;
                                if(randomColonneArriere > 9)
                                {
                                    randomColonneArriere = randomColonneAvant - 2;
                                }
                                break;
                            case 1:
                                randomColonneArriere = randomColonneAvant - 2;
                                if(randomColonneArriere < 0)
                                {
                                    randomColonneArriere = randomColonneAvant + 2;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 2);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;

                    case 3:
                        randomLigneArriere = randomLigneAvant;
                        tirageSens =  randIntervalle(0,2);

                        switch(tirageSens)
                        {
                            case 0:
                                randomColonneArriere = randomColonneAvant + 2;
                                if(randomColonneArriere > 9)
                                {
                                    randomColonneArriere = randomColonneAvant - 2;
                                }
                                break;
                            case 1:
                                randomColonneArriere = randomColonneAvant - 2;
                                if(randomColonneArriere < 0)
                                {
                                    randomColonneArriere = randomColonneAvant + 2;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 3);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;


                    case 4:
                        randomLigneArriere = randomLigneAvant;
                        tirageSens = randIntervalle(0,2);

                        switch(tirageSens)
                        {
                            case 0:
                                randomColonneArriere = randomColonneAvant + 1;
                                if(randomColonneArriere > 9)
                                {
                                    randomColonneArriere = randomColonneAvant - 1;
                                }
                                break;
                            case 1:
                                randomColonneArriere = randomColonneAvant - 1;
                                if(randomColonneArriere < 0)
                                {
                                    randomColonneArriere = randomColonneAvant + 1;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                            {
                                ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 4);
                            }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;
                }
                break;
            /*Idem que le cas precedent sauf que cette fois le bateau est a la verticale*/
            case 1:
                switch(numbat)
                {
                    case 0:

                        switch(tirageSens)
                        {
                            case 0:
                                randomLigneArriere = randomLigneAvant + 4;
                                if(randomLigneArriere > 9)
                                {
                                    randomLigneArriere = randomLigneAvant - 4;
                                }
                                break;
                            case 1:
                                randomLigneArriere = randomLigneAvant - 4;
                                if(randomLigneArriere < 0)
                                {
                                    randomLigneArriere = randomLigneAvant +4;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }
                        randomColonneArriere =  randomColonneAvant;

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 0);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;

                    case 1:

                        switch(tirageSens)
                        {
                            case 0:
                                randomLigneArriere = randomLigneAvant + 3;
                                if(randomLigneArriere > 9)
                                {
                                    randomLigneArriere = randomLigneAvant - 3;
                                }
                                break;
                            case 1:
                                randomLigneArriere = randomLigneAvant - 3;
                                if(randomLigneArriere < 0)
                                {
                                    randomLigneArriere = randomLigneAvant + 3;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }
                        randomColonneArriere =  randomColonneAvant;

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 1);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;

                    case 2:

                        switch(tirageSens)
                        {
                            case 0:
                                randomLigneArriere = randomLigneAvant + 2;
                                if(randomLigneArriere > 9)
                                {
                                    randomLigneArriere = randomLigneAvant - 2;
                                }
                                break;
                            case 1:
                                randomLigneArriere = randomLigneAvant - 2;
                                if(randomLigneArriere < 0)
                                {
                                    randomLigneArriere = randomLigneAvant + 2;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }
                        randomColonneArriere =  randomColonneAvant;

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 2);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;

                    case 3:

                        switch(tirageSens)
                        {
                            case 0:
                                randomLigneArriere = randomLigneAvant + 2;
                                if(randomLigneArriere > 9)
                                {
                                    randomLigneArriere = randomLigneAvant -2;
                                }
                                break;
                            case 1:
                                randomLigneArriere = randomLigneAvant - 2;
                                if(randomLigneArriere < 0)
                                {
                                    randomLigneArriere = randomLigneAvant + 2;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }
                        randomColonneArriere =  randomColonneAvant;

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 3);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;

                    case 4:

                        switch(tirageSens)
                        {
                            case 0:
                                randomLigneArriere = randomLigneAvant + 1;
                                if(randomLigneArriere > 9)
                                {
                                    randomLigneArriere = randomLigneAvant -1;
                                }
                                break;
                            case 1:
                                randomLigneArriere = randomLigneAvant - 1;
                                if(randomLigneArriere < 0)
                                {
                                    randomLigneArriere = randomLigneAvant + 1;
                                }
                                break;
                                /*Cas particulier pour eviter un warning*/
                            default:
                                return placementBateauxIA(IA, numbat);
                        }
                        randomColonneArriere =  randomColonneAvant;

                        randomCaseArriere = IA->grilles[randomLigneArriere][randomColonneArriere][0];
                        if (randomCaseArriere == 1)
                        {
                            return placementBateauxIA(IA, numbat);
                        }

                        else
                        {
                            random_avant_int[0] = randomColonneAvant;
                            random_avant_int[1] = randomLigneAvant;
                            random_arriere_int[0] = randomColonneArriere;
                            random_arriere_int[1] = randomLigneArriere;

                            if((verifierPresenceBateau(IA, random_avant_int, random_arriere_int) == 0) && (verifierCaseAdjacente(IA, random_avant_int, random_arriere_int) == 0))
                                {
                                    ajoutBateauGrilleIA(IA, random_avant_int, random_arriere_int, 4);
                                }

                            else
                            {
                                return placementBateauxIA(IA, numbat);
                            }
                        }
                        break;
                }
                break;
        }
    }

    return 0;
}

int entrerCordonneesBateaux(Joueur * joueur, int numerobat)
{
    /*Permet au joueur de positionner le bateau selectionné avec la fonction ChoixBateauxPositioner*/

    char avant[2][2]; /*Stock la position AVANT du bateau entré par le joueur sous forme de CHAR*/
    char arriere[2][2]; /*Stock la position ARRIERE du bateau entré par le joueur sous forme de CHAR*/
    int avant_int[2];
    int arriere_int[2];



    switch(numerobat)
    {
        /*Affiche selon le bateau à positionner les instructions nécessaires*/
        case 0:

                    afficherGrille(joueur->grilles, 0);
                    printf("\nPositionement des bateaux de %s (attention pas de bateau en diagonale ni colle)\n", joueur->nom);
                    printf("\n%s : Entrer la position arriere du porte-avions (5 cases)", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", avant[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", avant[1]);
                    getchar();
                    printf("\n%s : Entrer la position avant du porte-avions", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", arriere[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", arriere[1]);
                    getchar();



                    /*Converti les positions avant et arriere du bateau en ENTIER avant de les stocker dans des nouveaux tableaux d'entier*/
                    avant_int[0] = convertCharEnInt(avant[0]);
                    avant_int[1] = convertCharEnInt(avant[1]);
                    arriere_int[0] = convertCharEnInt(arriere[0]);
                    arriere_int[1] = convertCharEnInt(arriere[1]);



                    /*Si l'emplacement est libre, lancement de la fonction ajoutBateauGrille*/
                    if((verifierPresenceBateau(joueur, avant_int, arriere_int) == 0) && (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 0))
                    {
                        ajoutBateauGrille(joueur, avant_int, arriere_int, 0);
                        afficherCoordonneeBateaux(joueur, numerobat);
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    /*Si un bateau est déjà présent sur l'emplacement où le joueur placer le bateau, affichage de message d'erreur et renvoie au menu de selection des bateaux à placer*/
                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a un bateau sur cet emplacement\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);

                    }

                    /*Verifie si le bateaux n'est pas de dimension 1*/
                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 2)
                    {
                        printf("\nLes cordonnees avant et arriere du bateau ne peuvent etre egales\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
                    else if (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a deja un bateau a proximite\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
                break;

        case 1:

                    afficherGrille(joueur->grilles, 0);
                    printf("\nPositionement des bateaux de %s (attention pas de bateau en diagonale ni colle)\n", joueur->nom);
                    printf("\n%s : Entrer la position arriere du croiseur (4 cases)", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", avant[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", avant[1]);
                    getchar();
                    printf("\n%s : Entrer la position avant du croiseur", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", arriere[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", arriere[1]);
                    getchar();

                    avant_int[0] = convertCharEnInt(avant[0]);
                    avant_int[1] = convertCharEnInt(avant[1]);
                    arriere_int[0] = convertCharEnInt(arriere[0]);
                    arriere_int[1] = convertCharEnInt(arriere[1]);


                    if((verifierPresenceBateau(joueur, avant_int, arriere_int) == 0) && (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 0))
                    {
                        ajoutBateauGrille(joueur, avant_int, arriere_int, 1);
                        afficherCoordonneeBateaux(joueur, numerobat);
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a un bateau sur cet emplacement\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 2)
                    {
                        printf("\nLes cordonnees avant et arriere du bateau ne peuvent etre egales\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
                    else if (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a deja un bateau a proximite\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
            break;

        case 2:

                    afficherGrille(joueur->grilles, 0);
                    printf("\nPositionement des bateaux de %s (attention pas de bateau en diagonale ni colle)\n", joueur->nom);
                    printf("\n%s : Entrer la position arriere du contre-torpilleur (3 cases)", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", avant[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", avant[1]);
                    getchar();
                    printf("\n%s : Entrer la position avant du contre-torpilleur", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", arriere[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", arriere[1]);
                    getchar();

                    avant_int[0] = convertCharEnInt(avant[0]);
                    avant_int[1] = convertCharEnInt(avant[1]);
                    arriere_int[0] = convertCharEnInt(arriere[0]);
                    arriere_int[1] = convertCharEnInt(arriere[1]);

                    printf("%d",verifierPresenceBateau(joueur, avant_int, arriere_int));

                    if((verifierPresenceBateau(joueur, avant_int, arriere_int) == 0) && (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 0))
                    {
                        ajoutBateauGrille(joueur, avant_int, arriere_int, 2);
                        afficherCoordonneeBateaux(joueur, numerobat);
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a un bateau sur cet emplacement\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 2)
                    {
                        printf("\nLes cordonnees avant et arriere du bateau ne peuvent etre egales\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
                    else if (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a deja un bateau a proximite\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
            break;

        case 3:

                    afficherGrille(joueur->grilles, 0);
                    printf("\nPositionement des bateaux de %s (attention pas de bateau en diagonale ni colle)\n", joueur->nom);
                    printf("\n%s : Entrer la position arriere du sous-marin (3 cases)", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", avant[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", avant[1]);
                    getchar();
                    printf("\n%s : Entrer la position avant du sous-marin", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", arriere[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", arriere[1]);
                    getchar();

                    avant_int[0] = convertCharEnInt(avant[0]);
                    avant_int[1] = convertCharEnInt(avant[1]);
                    arriere_int[0] = convertCharEnInt(arriere[0]);
                    arriere_int[1] = convertCharEnInt(arriere[1]);

                    printf("%d",verifierPresenceBateau(joueur, avant_int, arriere_int));

                    if((verifierPresenceBateau(joueur, avant_int, arriere_int) == 0) && (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 0))
                    {
                        ajoutBateauGrille(joueur, avant_int, arriere_int, 3);
                        afficherCoordonneeBateaux(joueur, numerobat);
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a un bateau sur cet emplacement\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 2)
                    {
                        printf("\nLes cordonnees avant et arriere du bateau ne peuvent etre egales\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
                    else if (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a deja un bateau a proximite\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
            break;

        case 4:

                    afficherGrille(joueur->grilles, 0);
                    printf("\nPositionement des bateaux de %s (attention pas de bateau en diagonale ni colle)\n", joueur->nom);
                    printf("%s : Entrer la position arriere du torpilleur (2 cases)", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", avant[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", avant[1]);
                    getchar();
                    printf("%s : Entrer la position avant du torpilleur", joueur->nom);
                    printf("\n > Colonne : ");
                    scanf("%s", arriere[0]);
                    getchar();
                    printf("\n > Ligne : ");
                    scanf("%s", arriere[1]);
                    getchar();

                    avant_int[0] = convertCharEnInt(avant[0]);
                    avant_int[1] = convertCharEnInt(avant[1]);
                    arriere_int[0] = convertCharEnInt(arriere[0]);
                    arriere_int[1] = convertCharEnInt(arriere[1]);

                    printf("%d",verifierPresenceBateau(joueur, avant_int, arriere_int));

                    if((verifierPresenceBateau(joueur, avant_int, arriere_int) == 0) && (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 0))
                    {
                        ajoutBateauGrille(joueur, avant_int, arriere_int, 4);
                        afficherCoordonneeBateaux(joueur, numerobat);
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a un bateau sur cet emplacement\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }

                    else if (verifierPresenceBateau(joueur, avant_int, arriere_int) == 2)
                    {
                        printf("\nLes cordonnees avant et arriere du bateau ne peuvent etre egales\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);

                    }
                    else if (verifierCaseAdjacente(joueur, avant_int, arriere_int) == 1)
                    {
                        printf("\nIl y a deja un bateau a proximite\n");
                        afficherGrille(joueur->grilles, 0);
                        choixBateauxPositioner(joueur);
                    }
            break;
    }
    return 0;
}

int verifierCaseAdjacente(Joueur * joueur, int avant[2], int arriere[2])
{
    int colonneAvant = avant[0];
    int ligneAvant = avant[1];
    int colonneArriere = arriere[0];
    int ligneArriere = arriere[1];

    int i;

    /*Calcul la taille du bateau pour ensuite parcourir chaque case et voir si l'une des futures cases est adjacente à un bateau*/
    int tailleBat = max(abs(ligneAvant - ligneArriere), abs(colonneAvant-colonneArriere))+1;


    for (i =0; i<tailleBat; i++)
    {
        /*Initiliasation des variable haut,bas,droite,gauche*/
       int haut = 0;
       int bas = 0;
       int droite = 0;
       int gauche = 0;

        /*Determine si le bateau est a la verticale*/
       if (colonneAvant == colonneArriere)
        {
            /*Initialise et determination de la ligne dont on va verifier les cases adjacentes*/
            int ligneActuel = min(ligneAvant,ligneArriere)+i;

            /*Attribution des valeurs gauche et droite*/
            gauche = colonneAvant-1;
            droite = colonneAvant+1;


            /*Attribution des valeurs haut et bas*/
            if (ligneActuel == 0)
                {haut = 0;}
            else if (ligneActuel == 9)
                {bas = 9;}
            else
                {
                haut = ligneActuel-1;
                bas = ligneActuel+1;
                }
            /*Attribution des  valeurs pour les  8 cases adjacentes à tester*/
            int testD = joueur->grilles[ligneActuel][droite][0];
            int testG = joueur->grilles[ligneActuel][gauche][0];
            int testH = joueur->grilles[haut][colonneAvant][0];
            int testB = joueur->grilles[bas][colonneAvant][0];
            int testHD = joueur->grilles[haut][droite][0];
            int testHG = joueur->grilles[haut][gauche][0];
            int testBD = joueur->grilles[bas][droite][0];
            int testBG = joueur->grilles[bas][gauche][0];

            /*Si l'une des cases adjacentes est celle d'un bateau, la fonction renvoie 1*/
            if(testD == 1 || testG == 1 || testB == 1 || testH == 1 || testHD == 1 || testHG == 1 || testBD == 1 || testBG == 1)
                {return 1;}

       }


        /*Determine si le bateau est a l'horizontale*/
        else if (ligneAvant == ligneArriere)
        {

                haut = ligneAvant-1;
                bas = ligneAvant+1;


            int colonneActuel = min(colonneAvant,colonneArriere)+i;

            if (colonneActuel == 0)
                {gauche = 0;}
            else if (colonneActuel == 9)
                {droite = 9;}
            else
                {
                gauche = colonneActuel-1;
                droite = colonneActuel+1;
                }

            int testD = joueur->grilles[ligneAvant][droite][0];
            int testG = joueur->grilles[ligneAvant][gauche][0];
            int testH = joueur->grilles[haut][colonneActuel][0];
            int testB = joueur->grilles[bas][colonneActuel][0];
            int testHD = joueur->grilles[haut][droite][0];
            int testHG = joueur->grilles[haut][gauche][0];
            int testBD = joueur->grilles[bas][droite][0];
            int testBG = joueur->grilles[bas][gauche][0];



            if(testD == 1 || testG == 1 || testH == 1 || testB == 1 || testHD == 1 || testHG == 1 || testBD == 1 || testBG == 1)
                {return 1;}

        }
    }
    return 0;
}

int verifierPresenceBateau(Joueur * joueur, int avant[2], int arriere[2])
{
    /*Sert à verifier si le bateau à placer n'est pas sur un emplacement déjà occupé à l'aide des postions arriere et avant du bateau*/

    int colonneAvant = avant[0];
    int ligneAvant = avant[1];
    int colonneArriere = arriere[0];
    int ligneArriere = arriere[1];

    /*Verifie si le bateau est de dim 1*/
    if (ligneAvant == ligneArriere && colonneAvant == colonneArriere)
    {
        return 2;
    }

    /*Verifie si la position du bateau est verticale*/
    else if (colonneAvant == colonneArriere)
    {
        /*Determine juste l'orientation du bateau pour parcourir les cases à la recherche d'autres bateaux dans le bon sens*/

        if (ligneAvant > ligneArriere)
        {
            int temp = ligneAvant - ligneArriere;
            int i;
            for (i =0; i<temp+1; i++)
            {
                /*Si il trouve un bateau sur l'une des case que le bateau a placer doit se trouver, il renvoie 1 soit true*/
                if (joueur->grilles[ligneArriere+i][colonneAvant][0] == 1)
                {
                    return 1;
                }
            }
        }

        else
        {
            int temp = ligneArriere - ligneAvant;
            int i;
            for (i =0; i<temp+1; i++)
            {
                if (joueur->grilles[ligneAvant+i][colonneAvant][0] == 1)
                {
                    return 1;
                }

            }
        }
    }

    /*Verifie si la position du bateau est horizontale*/

    else if (ligneAvant == ligneArriere)
    {

        if (colonneAvant > colonneArriere)
        {
            int temp = colonneAvant-colonneArriere;
            int j;
            for(j=0; j<temp+1; j++)
            {
                if(joueur->grilles[ligneAvant][colonneArriere+j][0] == 1)
                {
                    return 1;
                }
            }

        }

        else
        {
            int temp = colonneArriere - colonneAvant;
            int j;
            for(j=0; j<temp+1; j++)
            {

                if(joueur->grilles[ligneAvant][colonneAvant+j][0] == 1)
                {
                    return 1;
                }
            }
        }

    }

    return 0;
}

void verifierBateauDejaEntree(Joueur * joueur, int numerobat)
{
    /*Verifie si un bateau a deja ete entree et si oui, l'efface de la grille et de la structure du joueur*/
    switch(numerobat)
    {
        case 0 :
            /*Pour determiner si un bateau a deja été entree, la fonction regarde si la deuxieme case du bateau est egale à 0,0(A0), cas impossible sauf si le bateau n'a pas déjà été entrée*/
            if (joueur->porteavions[1][0] != 0 || joueur->porteavions[1][1] != 0)
            {
                    int i;
                    for (i =0; i<5; i++)
                    {
                        int l = joueur->porteavions[i][1];
                        int c = joueur->porteavions[i][0];

                        joueur->grilles[l][c][0] = 0;
                        joueur->porteavions[i][0] = 0;
                        joueur->porteavions[i][1] = 0;

                    }
            }
            break;
        case 1 :
            if (joueur->croiseur[1][1] != 0 || joueur->croiseur[1][0] != 0)
            {
                    int i;
                    for (i =0; i<4; i++)
                    {
                        int l = joueur->croiseur[i][1];
                        int c = joueur->croiseur[i][0];

                        joueur->grilles[l][c][0] = 0;
                        joueur->croiseur[i][0] = 0;
                        joueur->croiseur[i][1] = 0;
                    }
            }
            break;
        case 2 :
            if (joueur->contretorpilleur[1][1] != 0 || joueur->contretorpilleur[1][0] != 0)
            {
                    int i;
                    for (i =0; i<3; i++)
                    {
                        int l = joueur->contretorpilleur[i][1];
                        int c = joueur->contretorpilleur[i][0];
                        joueur->grilles[l][c][0] = 0;
                        joueur->contretorpilleur[i][0] = 0;
                        joueur->contretorpilleur[i][1] = 0;
                    }
            }
            break;
        case 3 :
            if (joueur->sousmarin[1][1] != 0 || joueur->sousmarin[1][0] != 0)
            {
                    int i;
                    for (i =0; i<3; i++)
                    {
                        int l = joueur->sousmarin[i][1];
                        int c = joueur->sousmarin[i][0];
                        joueur->grilles[l][c][0] = 0;
                        joueur->sousmarin[i][0] = 0;
                        joueur->sousmarin[i][1] = 0;
                    }
            }
            break;
        case 4 :
            if (joueur->torpilleur[1][1] != 0 || joueur->torpilleur[1][0] != 0)
            {
                    int i;
                    for (i =0; i<2; i++)
                    {
                        int l = joueur->torpilleur[i][1];
                        int c = joueur->torpilleur[i][0];
                        joueur->grilles[l][c][0] = 0;
                        joueur->torpilleur[i][0] = 0;
                        joueur->torpilleur[i][1] = 0;
                    }
            }
            break;
    }
}

void choixBateauxPositioner(Joueur * joueur)
{
        /*Permet au joueur de choisir le bateau à positionner*/
        int choix2  = 0;
        printf("\n\n%s : choisissez le bateau a placer :",joueur->nom);
        printf("\n[0] porte-avions (5 cases):");
        printf("\n[1] croiseur (4 cases):");
        printf("\n[2] contre-torpilleur (3 cases):");
        printf("\n[3] sous-marin (3 cases) :");
        printf("\n[4] torpilleur (2 cases) :");
        printf("\n[5] Quitter");
        printf("\n\nEntrer votre choix : ");
        scanf("%d", &choix2);
        getchar();

        /*Lance la fonction entrerCordonneesBateaux avec la valeur du bateau choisi par le joueur*/

        if(choix2 == 0)
        {
                afficherCoordonneeBateaux(joueur, 0);
                verifierBateauDejaEntree(joueur, 0);
                afficherCoordonneeBateaux(joueur, 0);
                entrerCordonneesBateaux(joueur, 0);
        }

        else if(choix2 == 1)
        {
                afficherCoordonneeBateaux(joueur, 1);
                verifierBateauDejaEntree(joueur, 1);
                afficherCoordonneeBateaux(joueur, 1);
                entrerCordonneesBateaux(joueur, 1);
        }
        else if(choix2 == 2)
        {
                afficherCoordonneeBateaux(joueur, 2);
                verifierBateauDejaEntree(joueur, 2);
                afficherCoordonneeBateaux(joueur, 2);
                entrerCordonneesBateaux(joueur, 2);
        }

        else if(choix2 == 3)
        {
                afficherCoordonneeBateaux(joueur, 3);
                verifierBateauDejaEntree(joueur, 3);
                afficherCoordonneeBateaux(joueur, 3);
                entrerCordonneesBateaux(joueur, 3);
        }

        else if(choix2 == 4)
        {
                afficherCoordonneeBateaux(joueur, 4);
                verifierBateauDejaEntree(joueur, 4);
                afficherCoordonneeBateaux(joueur, 4);
                entrerCordonneesBateaux(joueur, 4);
        }
        else
        {
        }
}

void ajoutBateauStructureJoueur1(Joueur * joueur, int ligneArriere, int colonneAvant, int numerobat, int i)
{
    /*Permet d'ajouter les coordonnées de chaque case du bateau à la structure bateau correspondante*/
    switch(numerobat)
    {
        case 0:
            joueur->porteavions[i][0] = colonneAvant;
            joueur->porteavions[i][1] = ligneArriere+i;
            break;
        case 1:
            joueur->croiseur[i][0] = colonneAvant;
            joueur->croiseur[i][1] = ligneArriere+i;
            break;
        case 2:
            joueur->contretorpilleur[i][0] = colonneAvant;
            joueur->contretorpilleur[i][1] = ligneArriere+i;
            break;
        case 3:
            joueur->sousmarin[i][0] = colonneAvant;
            joueur->sousmarin[i][1] = ligneArriere+i;
            break;
        case 4:
            joueur->torpilleur[i][0] = colonneAvant;
            joueur->torpilleur[i][1] = ligneArriere+i;
            break;
    }

}

void ajoutBateauStructureJoueur2(Joueur * joueur, int ligneAvant, int colonneAvant, int numerobat, int i)
{
    switch(numerobat)
    {
        case 0:
            joueur->porteavions[i][0] = colonneAvant;
            joueur->porteavions[i][1] = ligneAvant+i;
            break;
        case 1:
            joueur->croiseur[i][0] = colonneAvant;
            joueur->croiseur[i][1] = ligneAvant+i;
            break;
        case 2:
            joueur->contretorpilleur[i][0] = colonneAvant;
            joueur->contretorpilleur[i][1] = ligneAvant+i;
            break;
        case 3:
            joueur->sousmarin[i][0] = colonneAvant;
            joueur->sousmarin[i][1] = ligneAvant+i;
            break;
        case 4:
            joueur->torpilleur[i][0] = colonneAvant;
            joueur->torpilleur[i][1] = ligneAvant+i;
            break;
    }

}

void ajoutBateauStructureJoueur3(Joueur * joueur, int ligneAvant, int colonneArriere, int numerobat, int j)
{
    switch(numerobat)
    {
        case 0:
            joueur->porteavions[j][0] = colonneArriere+j;
            joueur->porteavions[j][1] = ligneAvant;
            break;
        case 1:
            joueur->croiseur[j][0] = colonneArriere+j;
            joueur->croiseur[j][1] = ligneAvant;
            break;
        case 2:
            joueur->contretorpilleur[j][0] = colonneArriere+j;
            joueur->contretorpilleur[j][1] = ligneAvant;
            break;
        case 3:
            joueur->sousmarin[j][0] = colonneArriere+j;
            joueur->sousmarin[j][1] = ligneAvant;
            break;
        case 4:
            joueur->torpilleur[j][0] = colonneArriere+j;
            joueur->torpilleur[j][1] = ligneAvant;
            break;
    }

}

void ajoutBateauStructureJoueur4(Joueur * joueur, int ligneAvant, int colonneAvant, int numerobat, int j)
{
    switch(numerobat)
    {
        case 0:
            joueur->porteavions[j][0] = colonneAvant+j;
            joueur->porteavions[j][1] = ligneAvant;
            break;
        case 1:
            joueur->croiseur[j][0] = colonneAvant+j;
            joueur->croiseur[j][1] = ligneAvant;
            break;
        case 2:
            joueur->contretorpilleur[j][0] = colonneAvant+j;
            joueur->contretorpilleur[j][1] = ligneAvant;
            break;
        case 3:
            joueur->sousmarin[j][0] = colonneAvant+j;
            joueur->sousmarin[j][1] = ligneAvant;
            break;
        case 4:
            joueur->torpilleur[j][0] = colonneAvant+j;
            joueur->torpilleur[j][1] = ligneAvant;
            break;
    }

}

void ajoutBateauGrille(Joueur * joueur, int avant[2], int arriere[2], int numerobat)
{
    /*Permet d'ajouter le bateau à la grille du joueur*/
    int colonneAvant = avant[0];
    int ligneAvant = avant[1];

    int colonneArriere = arriere[0];
    int ligneArriere = arriere[1];


    int verifTailleBat = 0;

    int c = abs(colonneArriere-colonneAvant);
    int l = abs(ligneArriere-ligneAvant);



    switch(numerobat)
    {


        /*En fonction du bateau, détermine la taille qu'il devrait avoir et la stock dans une variable verifTailleBat*/
        case 0:
            if((c == 4) || (l == 4))
            {verifTailleBat = 1;}
            break;
        case 1:
            if((c == 3) || (l == 3))
            {verifTailleBat = 1;}
            break;
        case 2:
            if((c == 2) || (l == 2))
            {verifTailleBat = 1;}
            break;
        case 3:
            if((c == 2) || (l == 2))
            {verifTailleBat = 1;}
            break;
        case 4:
            if((c == 1) || (l == 1))
            {verifTailleBat = 1;}
            break;
    }

    /*Verifie si le bateau est verticale, c'est à dire qu'il à la même colonne et que le bateau entré par le joueur à la bonne taille*/
    if (colonneAvant == colonneArriere && verifTailleBat == 1)
    {
        /*Determine juste l'orientation du bateau*/
        if (ligneAvant > ligneArriere)
        {
            int temp = ligneAvant - ligneArriere;
            int i;
            /*Parcours chaque case occupé par le bateau*/
            for (i =0; i<=temp; i++)
            {
                /*Met la case à 1 et lance de la fonction ajoutBateauStructureJoueur1*/
                joueur->grilles[ligneArriere+i][colonneAvant][0] = 1;
                ajoutBateauStructureJoueur1(joueur, ligneArriere, colonneAvant, numerobat, i);
                afficherCoordonneeBateaux(joueur, numerobat);
            }
        }

        else
        {
            int temp = ligneArriere - ligneAvant;
            int i;
            for (i =0; i<=temp; i++)
            {
                joueur->grilles[ligneAvant+i][colonneAvant][0] = 1;
                ajoutBateauStructureJoueur2(joueur, ligneAvant, colonneAvant, numerobat, i);
                afficherCoordonneeBateaux(joueur, numerobat);
            }
        }
    }

    /*Verifie si la position du bateau est horizontale etc..*/
    else if (ligneAvant == ligneArriere && verifTailleBat == 1)
    {
        if (colonneAvant > colonneArriere)
        {
            int temp = colonneAvant-colonneArriere;
            int j;
            for(j=0; j<=temp; j++)
            {
                joueur->grilles[ligneAvant][colonneArriere+j][0] = 1;
                ajoutBateauStructureJoueur3(joueur, ligneAvant, colonneArriere, numerobat, j);
                afficherCoordonneeBateaux(joueur, numerobat);
            }
        }

        else
        {
            int temp = colonneArriere - colonneAvant;
            int j;
            for(j=0; j<=temp; j++)
            {
                joueur->grilles[ligneAvant][colonneAvant+j][0] = 1;
                ajoutBateauStructureJoueur4(joueur, ligneAvant, colonneAvant, numerobat, j);
                afficherCoordonneeBateaux(joueur, numerobat);
            }
        }

    }

    else if (verifTailleBat == 0)
    {
        printf("Vous avez entree un bateau qui n'a pas la bonne taille");
        afficherCoordonneeBateaux(joueur, numerobat);
        choixBateauxPositioner(joueur);
    }

    else
    {
        printf("Vous avez entree un bateau qui suit une diagonale");
        afficherCoordonneeBateaux(joueur, numerobat);
        choixBateauxPositioner(joueur);
    }
}

void ajoutBateauGrilleIA(Joueur * joueur, int avant[2], int arriere[2], int numerobat)
{
    /*Permet d'ajouter le bateau à la grille du joueur*/
    int colonneAvant = avant[0];
    int ligneAvant = avant[1];

    int colonneArriere = arriere[0];
    int ligneArriere = arriere[1];

    int verifTailleBat = 0;

    int c = abs(colonneArriere-colonneAvant);
    int l = abs(ligneArriere-ligneAvant);

    switch(numerobat)
    {


        /*En fonction du bateau, détermine la taille qu'il devrait avoir et la stock dans une variable verifTailleBat*/
        case 0:
            if((c == 4) || (l == 4))
            {verifTailleBat = 1;}
            break;
        case 1:
            if((c == 3) || (l == 3))
            {verifTailleBat = 1;}
            break;
        case 2:
            if((c == 2) || (l == 2))
            {verifTailleBat = 1;}
            break;
        case 3:
            if((c == 2) || (l == 2))
            {verifTailleBat = 1;}
            break;
        case 4:
            if((c == 1) || (l == 1))
            {verifTailleBat = 1;}
            break;
    }


    /*Verifie si le bateau est verticale, c'est à dire qu'il à la même colonne et que le bateau entré par le joueur à la bonne taille*/
    if (colonneAvant == colonneArriere && verifTailleBat == 1)
    {
        /*Determine juste l'orientation du bateau*/
        if (ligneAvant > ligneArriere)
        {
            int temp = ligneAvant - ligneArriere;
            int i;
            /*Parcours chaque case occupé par le bateau*/
            for (i =0; i<=temp; i++)
            {
                /*Met la case à 1 et lance de la fonction ajoutBateauStructureJoueur1*/
                joueur->grilles[ligneArriere+i][colonneAvant][0] = 1;
                ajoutBateauStructureJoueur1(joueur, ligneArriere, colonneAvant, numerobat, i);
                afficherCoordonneeBateaux(joueur, numerobat);
                afficherGrille(joueur->grilles, 0);
            }
        }

        else
        {
            int temp = ligneArriere - ligneAvant;
            int i;
            for (i =0; i<=temp; i++)
            {
                joueur->grilles[ligneAvant+i][colonneAvant][0] = 1;
                ajoutBateauStructureJoueur2(joueur, ligneAvant, colonneAvant, numerobat, i);
                afficherCoordonneeBateaux(joueur, numerobat);
                afficherGrille(joueur->grilles, 0);
            }
        }
    }

    /*Verifie si la position du bateau est horizontale etc..*/
    else if (ligneAvant == ligneArriere && verifTailleBat == 1 )
    {
        if (colonneAvant > colonneArriere)
        {
            int temp = colonneAvant-colonneArriere;
            int j;
            for(j=0; j<=temp; j++)
            {
                joueur->grilles[ligneAvant][colonneArriere+j][0] = 1;
                ajoutBateauStructureJoueur3(joueur, ligneAvant, colonneArriere, numerobat, j);
                afficherCoordonneeBateaux(joueur, numerobat);
                afficherGrille(joueur->grilles, 0);
            }
        }

        else
        {
            int temp = colonneArriere - colonneAvant;
            int j;
            for(j=0; j<=temp; j++)
            {
                joueur->grilles[ligneAvant][colonneAvant+j][0] = 1;
                ajoutBateauStructureJoueur4(joueur, ligneAvant, colonneAvant, numerobat, j);
                afficherCoordonneeBateaux(joueur, numerobat);
                afficherGrille(joueur->grilles, 0);
            }
        }

    }

    /*Seul variante avec sa fonction soeur, ici quand les cases avant et arriere entrees ne sont pas conforme, on relance la fonction placementBateauxIA*/
    else if (verifTailleBat == 0)
    {
        afficherCoordonneeBateaux(joueur, numerobat);
        placementBateauxIA(joueur, numerobat);
    }

    else
    {
        afficherCoordonneeBateaux(joueur, numerobat);
        placementBateauxIA(joueur, numerobat);
    }
}

int touche(Joueur * joueur, int l, int c)
{
    /*Verifie si la case est celle d'un bateau adverse, si oui elle renvoie 1*/
    if(joueur->grilles[l][c][0] == 1)
    {
        return 1;
    }

    else if(joueur->grilles[l][c][0] == 0)
    {
        return 0;
    }

    return 0;
}

int coule(Joueur * joueur)
{
    int i;
    int casetouchePA = 0;/*variable qui determine le nombre de fois que le porte-avion a été touché*/

    printf("\n\n");
    /*Parcours chaque case du porte-avions et verifie si l'un des case a déjà été touché*/
    for (i =0; i<5; i++)
    {
        int c = joueur->porteavions[i][0];
        int l = joueur->porteavions[i][1];

        int bat = joueur->grilles[l][c][0];
        /*Si la case du bateau a deja été touché, on incremente casetouchePA*/
        if (bat == 2)
        {
            casetouchePA = casetouchePA + 1;
            /*Si toutes les cases du bateau ont été touché, le bateau est coulé, on renvoie donc 1*/
            if(casetouchePA == 5)
            {
                return 1;
            }
        }
    }

    int casetoucheCr = 0;
    printf("\n\n");
    for (i =0; i<4; i++)
    {
        int c = joueur->croiseur[i][0];
        int l = joueur->croiseur[i][1];

        int bat = joueur->grilles[l][c][0];
        if (bat == 2)
        {
            casetoucheCr = casetoucheCr + 1;
            if(casetoucheCr == 4)
            {
                return 2;
            }
        }
    }

    int casetoucheCT = 0;
    printf("\n\n");
    for (i =0; i<3; i++)
    {
        int c = joueur->contretorpilleur[i][0];
        int l = joueur->contretorpilleur[i][1];

        int bat = joueur->grilles[l][c][0];

        if (bat == 2)
        {
            casetoucheCT = casetoucheCT + 1;
            if(casetoucheCT == 3)
            {
                return 3;
            }
        }
    }

    int casetoucheSM = 0;
    printf("\n\n");

    for (i =0; i<3; i++)
    {
        int c = joueur->sousmarin[i][0];
        int l = joueur->sousmarin[i][1];

        int bat = joueur->grilles[l][c][0];
        if (bat == 2)
        {
            casetoucheSM = casetoucheSM + 1;
            if(casetoucheSM == 3)
            {
                return 4;
            }
        }
    }

    int casetoucheTr = 0;
    for (i =0; i<2; i++)
    {
        int c = joueur->torpilleur[i][0];
        int l = joueur->torpilleur[i][1];

        int bat = joueur->grilles[l][c][0];

        if (bat == 2)
        {
            casetoucheTr = casetoucheTr + 1;
            if(casetoucheTr == 2)
            {
                return 5;
            }
        }
    }

    return 0;

}

void miseajourCoule(Joueur * joueur1, Joueur * joueur2, int numbat)
{
    int i;
    int c;
    int l;
    /*Permet de mettre a jour les cases de la grille du bateau qui a ete coule*/
    switch(numbat)
    {
        case 0:
            for (i =0; i<5; i++)
            {
                /*Parcours toutes les cases du porte-avions, grace la structure porte-avion et les met a jour unes à unes*/
                 c = joueur2->porteavions[i][0];
                 l = joueur2->porteavions[i][1];
                joueur1->grilles[l][c][1] = 3;
                joueur2->grilles[l][c][0] = 3;
            }
            break;

        case 1:
            for (i =0; i<4; i++)
            {
                 c = joueur2->croiseur[i][0];
                 l = joueur2->croiseur[i][1];
                joueur1->grilles[l][c][1] = 3;
                joueur2->grilles[l][c][0] = 3;

            }
            break;

        case 2:
            for (i =0; i<3; i++)
            {
                 c = joueur2->contretorpilleur[i][0];
                 l = joueur2->contretorpilleur[i][1];
                joueur1->grilles[l][c][1] = 3;
                joueur2->grilles[l][c][0] = 3;
            }
            break;

        case 3:
            for (i =0; i<3; i++)
            {
                 c = joueur2->sousmarin[i][0];
                 l = joueur2->sousmarin[i][1];
                joueur1->grilles[l][c][1] = 3;
                joueur2->grilles[l][c][0] = 3;
            }
            break;

        case 4:
            for (i =0; i<2; i++)
            {
                 c = joueur2->torpilleur[i][0];
                 l = joueur2->torpilleur[i][1];
                joueur1->grilles[l][c][1] = 3;
                joueur2->grilles[l][c][0] = 3;
            }
            break;
    }
}

int perdu(Joueur * joueur)
{
    /*Determine si le joueur à perdu en verifiant si l'une des cases de chaque bateau est egale a coulé, d'où l'utilité de la fonction miseajourCoule*/

    int cPA = joueur->porteavions[0][0];
    int lPA = joueur->porteavions[0][1];
    int casePA = joueur->grilles[lPA][cPA][0];

    int cCr = joueur->croiseur[0][0];
    int lCr = joueur->croiseur[0][1];
    int caseCr = joueur->grilles[lCr][cCr][0];

    int cCT = joueur->contretorpilleur[0][0];
    int lCT = joueur->contretorpilleur[0][1];
    int caseCT = joueur->grilles[lCT][cCT][0];

    int cSM = joueur->sousmarin[0][0];
    int lSM = joueur->sousmarin[0][1];
    int caseSM = joueur->grilles[lSM][cSM][0];

    int cTo = joueur->torpilleur[0][0];
    int lTo = joueur->torpilleur[0][1];
    int caseTo = joueur->grilles[lTo][cTo][0];

    if(casePA == 3 && caseCr == 3 && caseCT == 3 && caseSM == 3 && caseTo == 3)
    {
        return 1;
    }

    return 0;

}

int phaseJeu(Joueur * joueur1, Joueur * joueur2, FILE * saveTemp)
{
    system("cls");
    int choix = 0;
    printf("\nnom du joueur jouant %s", joueur1->nom);
    printf("\n1 : Continuer");
    printf("\n2 : Quitter et sauvegarder");
    printf("\n3 : Quitter");
    printf("\nEntrer votre choix : ");

    scanf("%d", &choix);

    char Csave[100];
    char coordooneesTir_char[2][2];
    int c, l;
    int bateaucoule;
    int perduJ2;

    switch(choix)
    {
        case 1:
                /*Initiliasation des variables coordonnees de tir entrées par le joueur*/

                system("cls");
                /*affichage du nom et des cases des bateaux du joueur*/
                printf("%s", joueur1->nom);
                printf("\nVotre flotte :");
                afficherCoordonneeBateaux(joueur1, 0);
                afficherCoordonneeBateaux(joueur1, 1);
                afficherCoordonneeBateaux(joueur1, 2);
                afficherCoordonneeBateaux(joueur1, 3);
                afficherCoordonneeBateaux(joueur1, 4);

                printf("\nGrille de votre flotte & des tirs sur votre flotte :");
                afficherGrille(joueur1->grilles, 0);

                printf("\nGrille de vos tirs :");
                afficherGrille(joueur1->grilles, 1);

                printf("\nEntrer les coordonees de votre tir : ");
                printf("\n > Colonne : ");
                scanf("%s", coordooneesTir_char[0]);
                getchar();
                printf("\n > Ligne : ");
                scanf("%s", coordooneesTir_char[1]);

                /*convertion des données entrée en int exploitable*/
                c = convertCharEnInt(coordooneesTir_char[0]);
                l = convertCharEnInt(coordooneesTir_char[1]);

                if(c >= 0 && c <=9 && l >= 0 && l <=9)
                {
                    /*Verifie si le coup a deja été joué*/
                    if (joueur2->grilles[l][c][0] == 2 || joueur2->grilles[l][c][0] == 3 || joueur2->grilles[l][c][0] == 9)
                    {
                        printf("\nVous avez deja joue ce coup");
                        return 0;
                    }

                    /*Verifie si le coup a touché une cible*/
                    else if (joueur2->grilles[l][c][0] == 1)
                    {
                        joueur1->grilles[l][c][1] = 2;
                        joueur2->grilles[l][c][0] = 2;
                        printf("\nTouche !");
                        sleep(2);

                        /*Si le joueur a touché, on verifie s'il n'a pas aussi coulé le bateau*/
                        bateaucoule = coule(joueur2);
                        if(bateaucoule == 1 || bateaucoule == 2 || bateaucoule == 3 || bateaucoule == 4 || bateaucoule == 5)
                        {
                            /*Permet d'afficher le bateau coulé et de mettre a jour les cases de la grille du bateau coulé*/
                            switch(bateaucoule)
                            {
                                case 1:
                                    printf("\nPorte-avions coule !");
                                    miseajourCoule(joueur1, joueur2, 0);
                                    break;
                                case 2:
                                    printf("\nCroiseur coule !");
                                    miseajourCoule(joueur1, joueur2, 1);
                                    break;
                                case 3:
                                    printf("\nContre-torpilleur coule !");
                                    miseajourCoule(joueur1, joueur2, 2);
                                    break;
                                case 4:
                                    printf("\nSous-marin coule !");
                                    miseajourCoule(joueur1, joueur2, 3);
                                    break;
                                case 5:
                                    printf("\nTorpilleur coule !");
                                    miseajourCoule(joueur1, joueur2, 4);
                                    break;
                            }
                            sleep(2);

                            /*Verifie si le joueur1 a gagné en determine si le joueur2 a perdu*/
                            perduJ2 = perdu(joueur2);

                            if(perduJ2 == 1)
                            {
                                joueur1->gagne = 1;
                            }

                            return 0;
                        }
                        return 0;
                    }

                    /*Verifie si le joueur a raté son coup*/
                    else if (joueur2->grilles[l][c][0] == 0)
                    {
                        joueur1->grilles[l][c][1] = 9;
                        joueur2->grilles[l][c][0] = 9;
                        printf("\nRate !");
                        sleep(5);
                        return 0;
                    }

                }

                /*Affiche que le joueur s'est trompé en entrant ses coordonnées*/
                else
                {
                    printf("Vous avez entree de mauvaises coordonnees");
                    sleep(5);
                    phaseJeu(joueur1, joueur2, saveTemp);
                    return 0;
                }
                return 0;
            break;

        case 2:
            fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
            fwrite(joueur2, sizeof(Joueur), 1, saveTemp);
            /*Ferme le fichier*/
            fclose(saveTemp);
            /*Permet au joueur d'enregistrer ses sauvegardes sous un nom donnée*/
            printf("\n\nEntrer le nom de la sauvegarde voulu finissant par .save: ");
            scanf("%99s", Csave);
            getchar();

            rename("saveTemp.save", Csave);
            return main();
            break;
        case 3:
            return main();
            break;

    }

    return 0;
}

int phaseJeuIA(Joueur * joueur1, Joueur * joueur2)
{
    /*Determine au harsard une case dans la limite de la grille*/
    int l = randIntervalle(0,10);
    int c = randIntervalle(0,10);

    int ramdomCase = joueur2->grilles[l][c][0];

    /*Verifie si la case n'a pas deja été joué, si oui on relance la fonction*/
    if(ramdomCase == 2 || ramdomCase == 3 || ramdomCase == 9)
    {
        return phaseJeuIA(joueur1,joueur2);
    }

    /*verifie si l'IA a touché*/
    if (joueur2->grilles[l][c][0] == 1)
    {
        joueur1->grilles[l][c][1] = 2;
        joueur2->grilles[l][c][0] = 2;
        int bateaucoule = coule(joueur2);

        if(bateaucoule == 1 || bateaucoule == 2 || bateaucoule == 3 || bateaucoule == 4 || bateaucoule == 5)
        {
            /*Idem que pour sa fonction soeur, mais n'affiche rien*/
            switch(bateaucoule)
            {
                case 1:
                    miseajourCoule(joueur1, joueur2, 0);
                    break;
                case 2:
                    miseajourCoule(joueur1, joueur2, 1);
                    break;
                case 3:
                    miseajourCoule(joueur1, joueur2, 2);
                    break;
                case 4:
                    miseajourCoule(joueur1, joueur2, 3);
                    break;
                case 5:
                    miseajourCoule(joueur1, joueur2, 4);
                    break;
            }

            /*Idem que pour sa fonction soeur, mais n'affiche rien*/
            int perduJ2 = perdu(joueur2);
            if(perduJ2 == 1)
            {
                joueur1->gagne = 1;
            }

            return 0;
        }
        return 0;
    }

    /*Idem que pour sa fonction soeur, mais n'affiche rien*/
    else if (joueur2->grilles[l][c][0] == 0)
    {
        joueur1->grilles[l][c][1] = 9;
        joueur2->grilles[l][c][0] = 9;
        return 0;
    }

    return 0;
}

int JvsJ()
{
    /*On initialise et ouvre des fichiers sauvegarde temporaire pour chaque joueur*/
    FILE * saveTemp;
    saveTemp = fopen("saveTemp.save" , "wb+");
    if (saveTemp==NULL) {printf("Erreur fichier");}


    system("cls");

    /*Initialisation des structures joueur*/
    Joueur * joueur1 = malloc(sizeof(Joueur));
    Joueur * joueur2 = malloc(sizeof(Joueur));

    /*Initialisation de la fonction rand avec time(NULL)*/
    srand(time(NULL));

    printf("Entrer le nom du joueur 1 : ");
    scanf("%s", joueur1->nom);
    getchar();
    printf("\nEntrer le nom du joueur 2 : ");
    scanf("%s", joueur2->nom);
    getchar();

    /*Initialisation et placement des bateaux du joueur1*/
    system("cls");
    initialiserGrille(joueur1->grilles, 0);
    initialiserStructBateau(joueur1);
    choixBateauxPositioner(joueur1);

    /*Initialisation et placement des bateaux du joueur2*/
    system("cls");
    initialiserGrille(joueur2->grilles, 0);
    initialiserStructBateau(joueur2);
    choixBateauxPositioner(joueur2);





    /*Permet de determiner aleatoirement le tour du joueur qui commencera en premier*/
    int tirageSort = randIntervalle(0,2);
    /*Initiliasation des grilles tir, et des variable gagné*/
    initialiserGrille(joueur1->grilles, 1);
    initialiserGrille(joueur2->grilles, 1);
    joueur1->gagne = 0;
    joueur2->gagne = 0;

    rewind(saveTemp);

    /*ecriture des 1eres structures dans les fichiers sauvegarder pour tester le bon fonctionnement  */
    fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
    fwrite(joueur2, sizeof(Joueur), 1, saveTemp);


    int nbIteJ1 = 1;
    int nbIteJ2 = 1;
    while(joueur1->gagne == 0 && joueur2->gagne == 0)
    {
        if(tirageSort == 0 && joueur1->gagne == 0)
        {
                tirageSort = 1;
                phaseJeu(joueur1, joueur2, saveTemp);

                /*copie de la structure joueur du joueur1 au tour i*/
                fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
                nbIteJ1 = nbIteJ1+1;
        }

        else if (tirageSort == 1 && joueur2->gagne == 0)
        {
                tirageSort = 0;
                phaseJeu(joueur2, joueur1, saveTemp);

                /*Idem pour joueur2*/
                fwrite(joueur2, sizeof(Joueur), 1, saveTemp);
                nbIteJ2 = nbIteJ2+1;
        }
    }


    /*Ferme le fichier*/
    fclose(saveTemp);


    system("cls");
    /*Affiche le gagnant*/
    if (joueur1->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur1->nom);
    }

    else if (joueur2->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur2->nom);
    }

    /*Propose au joueur de sauvegarder sa partie*/
    int oN = 0;
    printf("\nVoulez-vous sauvergarder la partie ? (Non = 0/Oui = 1) : ");
    scanf("%d", &oN);
    getchar();

    char Csave[100];
    switch(oN)
    {
        case 0:
            /*Si le joueur ne veut pas sauvegarder la partie, on efface les fichiers temporaire*/
            remove("saveTemp.save");
            return main();
            break;
        case 1:
            /*Permet au joueur d'enregistrer ses sauvegardes sous un nom donnée*/
            printf("\n\nEntrer le nom de la sauvegarde voulu finissant par .save: ");
            scanf("%99s", Csave);
            getchar();

            rename("saveTemp.save", Csave);
            return main();
            break;
    }

    return main();
}

int JvsIA()
{
    FILE * saveTemp;
    saveTemp = fopen("saveTemp.save" , "wb+");
    if (saveTemp==NULL) {printf("Erreur fichier");}

    system("cls");

    /*Initialisation des structures joueur*/
    Joueur * joueur1 = malloc(sizeof(Joueur));
    Joueur * IA = malloc(sizeof(Joueur));

    /*Initialisation de la fonction rand avec time(NULL)*/
    srand(time(NULL));

    printf("Entrer le nom du joueur 1 : ");
    scanf("%s", joueur1->nom);
    getchar();

    /*On initialise le nom de l'IA manuellement*/
    strcpy(IA->nom, "IA");



    /*Initialisation automatique des bateaux de l'IA*/
    initialiserGrille(IA->grilles, 0);
    initialiserStructBateau(IA);
    placementBateauxIA(IA, 0);
    placementBateauxIA(IA, 1);
    placementBateauxIA(IA, 2);
    placementBateauxIA(IA, 3);
    placementBateauxIA(IA, 4);


    system("cls");
    initialiserGrille(joueur1->grilles, 0);
    initialiserStructBateau(joueur1);
    choixBateauxPositioner(joueur1);


    int tirageSort = randIntervalle(0,2);
    initialiserGrille(joueur1->grilles, 1);
    initialiserGrille(IA->grilles, 1);
    joueur1->gagne = 0;
    IA->gagne = 0;



    rewind(saveTemp);


    fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
    fwrite(IA, sizeof(Joueur), 1, saveTemp);


    int nbIteJ1 = 1;
    int nbIteJ2 = 1;
    while(joueur1->gagne == 0 && IA->gagne == 0)
    {
        if(tirageSort == 0 && joueur1->gagne == 0)
        {
                tirageSort = 1;
                phaseJeu(joueur1, IA, saveTemp);

                fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
                nbIteJ1 = nbIteJ1+1;
        }

        else if (tirageSort == 1 && IA->gagne == 0)
        {
                tirageSort = 0;
                /*Ici on utilise la fonction phaseJeuIA pour le tour de l'ordinateur*/
                phaseJeuIA(IA, joueur1);

                fwrite(IA, sizeof(Joueur), 1, saveTemp);
                nbIteJ2 = nbIteJ2+1;
        }

    }


    /*Place le curseur au debut des fichiers*/
    rewind(saveTemp);



    fclose(saveTemp);


    system("cls");

    if (joueur1->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur1->nom);
    }

    else if (IA->gagne == 1)
    {
        printf("\n L'%s a gagne la partie !\n", IA->nom);
    }

    int oN = 0;
    printf("Voulez-vous sauvergarder la partie ? (Non = 0/Oui = 1) : ");
    scanf("%d", &oN);
    getchar();

    char Csave[100];
    switch(oN)
    {
        case 0:
            remove("saveTemp.save");
            return main();
            break;
        case 1:
            printf("\n\nEntrer le nom de la sauvegarde voulu finissant par .save: ");
            scanf("%99s", Csave);
            getchar();

            rename("saveTemp.save", Csave);
            return main();
            break;
    }


    return main();
}

int JvsJreprise(Joueur * joueur1, Joueur * joueur2, FILE * saveTemp)
{
    /*On initialise et ouvre des fichiers sauvegarde temporaire pour chaque joueur*/
    system("cls");

    /*Initialisation de la fonction rand avec time(NULL)*/
    srand(time(NULL));


    /*Permet de determiner aleatoirement le tour du joueur qui commencera en premier*/
    int tirageSort = randIntervalle(0,2);

    int nbIteJ1 = 1;
    int nbIteJ2 = 1;
    while(joueur1->gagne == 0 && joueur2->gagne == 0)
    {
        if(tirageSort == 0 && joueur1->gagne == 0)
        {
                tirageSort = 1;
                phaseJeu(joueur1, joueur2, saveTemp);

                /*copie de la structure joueur du joueur1 au tour i*/
                fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
                nbIteJ1 = nbIteJ1+1;
        }

        else if (tirageSort == 1 && joueur2->gagne == 0)
        {
                tirageSort = 0;
                phaseJeu(joueur2, joueur1, saveTemp);

                /*Idem pour joueur2*/
                fwrite(joueur2, sizeof(Joueur), 1, saveTemp);
                nbIteJ2 = nbIteJ2+1;
        }
    }


    /*Ferme le fichier*/
    fclose(saveTemp);

    system("cls");

    /*Affiche le gagnant*/
    if (joueur1->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur1->nom);
    }

    else if (joueur2->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur2->nom);
    }

    /*Propose au joueur de sauvegarder sa partie*/
    int oN = 0;
    printf("\nVoulez-vous sauvergarder la partie ? (Non = 0/Oui = 1) : ");
    scanf("%d", &oN);
    getchar();

    char Csave[100];
    switch(oN)
    {
        case 0:
            /*Si le joueur ne veut pas sauvegarder la partie, on efface les fichiers temporaire*/
            remove("saveTemp.save");
            return main();
            break;
        case 1:
            /*Permet au joueur d'enregistrer ses sauvegardes sous un nom donnée*/
            printf("\n\nEntrer le nom de la sauvegarde voulu finissant par .save: ");
            scanf("%99s", Csave);
            getchar();

            rename("saveTemp.save", Csave);
            return main();
            break;
    }

    return main();
}


int JvsIAreprise(Joueur * joueur1, Joueur * IA, FILE * saveTemp)
{
    system("cls");

    /*Initialisation de la fonction rand avec time(NULL)*/
    srand(time(NULL));
    int tirageSort = randIntervalle(0,2);


    int nbIteJ1 = 1;
    int nbIteJ2 = 1;
    while(joueur1->gagne == 0 && IA->gagne == 0)
    {
        if(tirageSort == 0 && joueur1->gagne == 0)
        {
                tirageSort = 1;
                phaseJeu(joueur1, IA, saveTemp);

                fwrite(joueur1, sizeof(Joueur), 1, saveTemp);
                nbIteJ1 = nbIteJ1+1;
        }

        else if (tirageSort == 1 && IA->gagne == 0)
        {
                tirageSort = 0;
                /*Ici on utilise la fonction phaseJeuIA pour le tour de l'ordinateur*/
                phaseJeuIA(IA, joueur1);

                fwrite(IA, sizeof(Joueur), 1, saveTemp);
                nbIteJ2 = nbIteJ2+1;
        }

    }

    fclose(saveTemp);


    system("cls");

    if (joueur1->gagne == 1)
    {
        printf("\n %s a gagne la partie !\n", joueur1->nom);
    }

    else if (IA->gagne == 1)
    {
        printf("\n L'%s a gagne la partie !\n", IA->nom);
    }

    int oN = 0;
    printf("Voulez-vous sauvergarder la partie ? (Non = 0/Oui = 1) : ");
    scanf("%d", &oN);
    getchar();

    char Csave[100];
    switch(oN)
    {
        case 0:
            remove("saveTemp.save");
            return main();
            break;
        case 1:
            printf("\n\nEntrer le nom de la sauvegarde voulu finissant par .save: ");
            scanf("%99s", Csave);
            getchar();

            rename("saveTemp.save", Csave);
            return main();
            break;
    }


    return main();
}


