#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "entreeSortieH.h"
#include "biblioH.h"

void menu(){

  puts("1.affichage biblio");

  puts("2.ajouter un ouvrage");

  puts("3.recherche d’un ouvrage par son numero");

  puts("4.recherche d’un ouvrage par son titre");

  puts("5.recherche de tous les livres d’un meme auteur");

  puts("6.suppression d’un ouvrage");

  puts("7.recherche de tous les ouvrages avec plusieurs exemplaires");

  puts("press 0 to exit...");
}

int main(int argc, char** argv){

    if (argc < 3){

        printf("usage: %s file number_of_line\n",argv[0]);
        
        return EXIT_FAILURE;
    }

    BiblioH *b = charger_n_entrees(argv[1],atoi(argv[2]));

    BiblioH *bi;

    LivreH *livre;

	int num;

    char titre[256];

    char auteur[256];

    char ligne[256];

	int rep;

    do{

		bi = NULL, livre = NULL;

    	printf("\nQue voulez-vous faire ? ...\n");

    	menu();

    	fgets(ligne,256,stdin);

		sscanf(ligne,"%d",&rep);

    	switch(rep){

    		case 1:

    			printf("\nAffichage\n");

    			afficher_biblioH(b);

    			break;

    		case 2:

    			printf("\nVeuillez entrer le numero, le titre et l'auteur de l'ouvrage.\n");

    			/* On suppose que le titre et l’auteur ne contiennent pas d’espace*/
				
				fgets(ligne,256,stdin);

    			if (sscanf(ligne,"%d %s %s",&num,titre,auteur)==3) {

    				inserer(b,num,titre,auteur);

    				printf("\nAjout fait.\n");

    			} else {

    				printf("\nErreur format\n");

    			}

    			break;

			case 3:

    			printf("\nVeuillez entrer le numero de l'ouvrage.\n");
				
				fgets(ligne,256,stdin);

    			if (sscanf(ligne,"%d",&num)==1){

    				if ((livre = recherche_ouvrage_num(b,num))==NULL) printf("\nOuvrage non trouvé\n");
                    else afficher_livreH(livre);

    			} else {

                    printf("\nErreur format\n");
                }

    			break;

			case 4:

    			printf("\nVeuillez enntrer le titre de l'ouvrage.\n");
				
				fgets(ligne,256,stdin);

    			if (sscanf(ligne,"%s",titre)==1){

    				if ((livre = recherche_ouvrage_titre(b,titre))==NULL) printf("\nOuvrage non trouvé\n");
                    else afficher_livreH(livre);

                } else {

                    printf("\nErreur format\n");
                }

    			break;

			case 5:

    			printf("\nVeuillez entrer le nom de l'auteur.\n");
				
				fgets(ligne,256,stdin);

    			if (sscanf(ligne,"%s",auteur)==1){

    				if ((bi = recherche_meme_auteur(b,auteur))== NULL) printf("\nAucun ouvrage trouvé de cet auteur\n");
                    else afficher_biblioH(bi);

    			} else {

                    printf("\nErreur format\n");

                }

    			liberer_biblioH(bi);

    			break;

			case 6:

    			printf("\nVeuillez entrer le numero, le titre et l'auteur de l'ouvrage.\n");

    			/* On suppose que le titre et l’auteur ne contiennent pas d’espace*/
				
				fgets(ligne,256,stdin);

    			if (sscanf(ligne,"%d %s %s",&num,titre,auteur)==3) {

    				if (supprimer_livreH(b, num, titre, auteur)) {
						
						printf("\nsuppression fait.\n");
						
					} else {
						
						printf("\nAucun livre trouvé à supprimer\n");
						
					}

    			} else {

    				printf("\nErreur format\n");

    			}

    			break;

			case 7:

    			if ((bi = recherche_exemplaires(b))==NULL) printf("\nAucun exemplaires trouvés\n");
    			else afficher_biblioH(bi);

    			liberer_biblioH(bi);

    			break;

    	}

    } while (rep != 0);

    liberer_biblioH(b);

    printf("\nAu revoir !\n");

    return 0;
}

