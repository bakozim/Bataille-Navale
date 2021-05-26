#include "bataille navale.h"


int main()
{
    int choix1 = 0;
    printf(" ___________________________________________________________________  \n");
    printf("|  ____        _        _ _ _        _   _                  _       | \n");
    printf("| | __ )  __ _| |_ __ _(_) | | ___  | \\ | | __ ___   ____ _| | ___  | \n");
    printf("| |  _ \\ / _` | __/ _` | | | |/ _ \\ |  \\| |/ _` \\ \\ / / _` | |/ _ \\ | \n");
    printf("| | |_) | (_| | || (_| | | | |  __/ | |\\  | (_| |\\ V / (_| | |  __/ | \n");
    printf("| |____/ \\__,_|\\__\\__,_|_|_|_|\\___| |_| \\_|\\__,_| \\_/ \\__,_|_|\\___| | \n");
    printf("|___________________________________________________________________| \n");

    printf("\n\n");
    printf("1 : Reprise partie\n");
    printf("2 : Commencer une nouvelle partie\n");
    printf("3 : Revisualiser une partie\n");
    printf("4 : Aide en ligne\n");
    printf("5 : Quitter\n");
    printf("\nEntrer votre choix : ");
    scanf("%d", &choix1);

    switch (choix1)
    {
        case 1:
            reprisePartie();
            break;
        case 2:
            menuNouvellePartie();
            break;
        case 3:
            revisualiserPartie();
            break;
        case 4:
            system ("start https://fr.wikipedia.org/wiki/Bataille_navale_(jeu)");
            break;
        case 5:
            break;
    }

    return 0;
}
