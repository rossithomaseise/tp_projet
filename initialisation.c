#include <stdio.h>
#include <stdlib.h>

#include "initialisation.h"

void initialisation_jeu(int tracks[390],s_plateau_jeu* plateauJeu,s_joueur* joueur,s_joueur* adversaire,s_partie* partie){

	/* #### For the function connectToServer #### */
	/*serverName*/
	char* serverName = "li1417-56.members.linode.com";
	/*port number used for the connection*/
	unsigned int port = 5678;//5678 1234
	/*Name of the player*/
	char* name = "Thomas";

	/* #### For the function waitForT2RGame #### */
	/*Type of the game we want to play*/
	//char* gameType = BOT;
	/*Name of the game*/
	char *gameName = (char*) malloc(50*sizeof(char));
	/*the number of cities*/
	int nbCities ;
	/*the number of tracks between the cities*/
	int nbTracks ;

	/* #### For the function getMap #### */
	/*array of (5 x number of tracks) integers*/
	//int* tracks;
	/*array of 5 t_color giving the 5 face up cards*/
	t_color faceUp[5];
	/*array of 4 t_colors with the initial cards in your hand*/
	t_color cards[4];

	/* ######## Connection to the server ######## */
	connectToServer(serverName,port,name);
	/* ######## Initialisation for cards, map and the beginner ######## */
	waitForT2RGame("TRAINING DO_NOTHING timeout=100 start=0 map=USA",gameName,&nbCities,&nbTracks);

	/* ## Initialisation du plateau ## */
	plateauJeu->nbCities = nbCities;
	plateauJeu->nbTracks = nbTracks;

	//printf("Le nombre de tracks est : %d",nbTracks);

	/*On alloue la taille nécessaire */
	//tracks = malloc(nbTracks*5*sizeof(int));
	/*######## Initialisation for the map 
	Get the map, the decks and initial cards and tell who starts ######## */
	/*######## Initialisation des paramètres de la partie : 
	cartes face visible et n° joueur ########*/
	partie->numero_joueur_actuel = getMap(tracks,faceUp,cards);
	/*Premier tour*/
	partie->premier_tour = 1;
	/*numéro du tour*/
	partie->numero_tour = 0;
	/*numéro du joueur*/
	if(!partie->numero_joueur_actuel){/* =0 => on commence */
		partie->numero_joueur = 0;
	}else{ /* =1 => l'adversaire commence */
		partie->numero_joueur = 1;
	}
	/* cartes face visible*/
	for(int i=0;i<5;i++){
		partie->faceUp[i] = faceUp[i];
	}

	for(int i=0;i<78;i++){
		int indice_city1 = tracks[5*i];
		int indice_city2 = tracks[5*i+1];
		int distance = tracks[5*i+2];
		int couleur = tracks[5*i+3];
		partie->route_distance[indice_city1][indice_city2] = distance;
		partie->route_couleur[indice_city1][indice_city2] = couleur;
	}
	/*Initialisation à 0 => toutes les routes sont disponibles*/
	for(int i=0;i<36;i++){
		for(int j=0;j<36;j++){
			partie->route_dispo[i][j] = 0;
			/*longueur = infini si non comprise entre 0 et 6*/
			if( partie->route_distance[i][j] > 6 || partie->route_distance[i][j] <= 0){
				partie->route_distance[i][j] = 100;
			}
			/*couleur = infini si non comprise entre 0 et 8*/
			if( partie->route_couleur[i][j] > 9 || partie->route_couleur[i][j] <= 0){
				partie->route_couleur[i][j] = 100;
			}
		}
	}
	for(int i=0;i<36;i++){
		for(int j=0;j<36;j++){
			//printf("\nPour i = %d et j =%d, on a %d \n",i,j,partie->route_couleur[i][j]);
		}
	}
	/* ## Initialisation des paramètres du joueur */
	joueur->wagons_disponible = 45;
	joueur->nb_carte_main = 4;
	joueur->nb_objectif = 0;
	joueur->score = 0;
	joueur->objectif_rempli = 0;
	/*initialisation à zéro*/
	for(int i=0;i<9;i++){
		joueur->tab_carte_couleur[i] = 0;
	}
	/*initialisation à zéro*/
	for(int i=0;i<10;i++){
		joueur->tab_objectif_ordre[i] = 100;
	}
	
	/*on va incrémenter +1 par carte de couleur */
	for(int i=0;i<4;i++){
		joueur->cards[i] = cards[i];/*on récupère ici les 4 cartes avec leurs couleurs*/
		/*Fonction qui compte les cartes selon leur couleur*/
		compteur_carte(cards[i],joueur);
	}
	/* ## Initialisation des paramètres du joueur */
	adversaire->wagons_disponible = 45;
	adversaire->nb_carte_main = 4;
	adversaire->nb_objectif = 0;
}

// void fin_jeu(int* tracks){
// 	 #### Close the connection to the server #### 
// 	closeConnection();
// 	free(tracks);
// }

void affichage_info_plateau_jeu(s_plateau_jeu* plateauJeu){
	printf("\n\nIl y a %d villes\n",plateauJeu->nbCities);
}
void affichage_info_partie(s_partie* partie){
	printf("\n\n Le numéro du joueur actuel est %d\n",partie->numero_joueur_actuel);
	printf("Notre numéro de joueur est %d\n",partie->numero_joueur);
	printf("On a comme couleur de cartes face visible :   ");
	for(int i=0;i<5;i++){
		printf("%d   ",partie->faceUp[i]);
	}
}

/*Fonction qui demande à l'utilisateur de choisir un coup avec si nécessaires 
les données pour*/
void choix_coup(s_choixCoup* choixCoup){
	printf("\n\nVeuillez choisir un coup :claim_ROUTE = 1,DRAW_BLIND_CARD = 2,DRAW_CARD = 3,DRAW_OBJECTIVES = 4,CHOOSE_OBJECTIVES = 5 \n");
	/*L'utilisateur choisit le coup par son id*/
	//scanf("%d",&choixCoup->identifiant_coup);
	/*Si le joueur réclame une route, il faut les données nécessaires pour*/
	if(choixCoup->identifiant_coup == 1){
		printf("Entrez : id city1, id city2, color et nb locomotives");
		//scanf("%d %d %d %d",&choixCoup->city1,&choixCoup->city2,&choixCoup->color,&choixCoup->nbLocomotives);
	}
 }
 
t_return_code action_coup(int tracks[390],t_color deck[5],s_choixCoup* choixCoup,s_joueur* joueur,s_partie* partie){
	/*Variable pour enregistrer la valeur de retour de la fonction choisie*/
	t_return_code code;
	/*Variable pour la fonction drawCard*/
	t_color card;
	/*Variable pour la fonction drawBlindCard*/
	t_color* card_pioche = (t_color*)malloc(sizeof(int));

	/*Variable pour la fonction claimRoute*/
	int position_tracks = 0;

	if(choixCoup->identifiant_coup == 1){
		code = claimRoute(choixCoup->city1,choixCoup->city2,choixCoup->color,choixCoup->nbLocomotives);

		/* On regarde où se situe les informations de la route choisie*/
		for(int i =0; i<391;i++){
			if(tracks[5*i] == choixCoup->city1 ){
				position_tracks = i*5 + 2;
			}
		}
		/*On effectue alors le décompte des cartes selon leur couleurs*/
		if(choixCoup->color == 1){
			joueur->tab_carte_couleur[0]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color == 2){
			joueur->tab_carte_couleur[1]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 3){
			joueur->tab_carte_couleur[2]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 4){
			joueur->tab_carte_couleur[3]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 5){
			joueur->tab_carte_couleur[4]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 6){
			joueur->tab_carte_couleur[5]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 7){
			joueur->tab_carte_couleur[6]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		else if(choixCoup->color  == 8){
			joueur->tab_carte_couleur[7]-=partie->route_distance[choixCoup->city1][choixCoup->city2] -choixCoup->nbLocomotives;
		}
		/*On décompte le nombre de locomotives*/
		if(choixCoup->nbLocomotives>0){
			joueur->tab_carte_couleur[8]-=choixCoup->nbLocomotives;
		}
		/*On effectue le décompte des wagons disponibles*/
		joueur->wagons_disponible =joueur->wagons_disponible -partie->route_distance[choixCoup->city1][choixCoup->city2]- choixCoup->nbLocomotives;
		joueur->nb_carte_main = joueur->nb_carte_main -partie->route_distance[choixCoup->city1][choixCoup->city2] - choixCoup->nbLocomotives;
		/*on rend non disponible la route prise*/
		partie->route_dispo[choixCoup->city1][choixCoup->city2] = 1;
	}
	
	/*Coup = piocher une carte dans la pioche*/
	else if(choixCoup->identifiant_coup == 2){
		for(int i=0;i<2;i++){
			code = drawBlindCard(card_pioche);
			joueur->nb_carte_main ++;
			card = *card_pioche;
			/*Fonction qui compte les cartes selon leur couleur*/
			compteur_carte(card,joueur);
		}
	}
	/*Coup = piocher une carte face visible*/
	else if(choixCoup->identifiant_coup == 3){
		for(int i=0;i<2;i++){
			printf("Entrez la couleur de carte");
			scanf("%d",&card);
			code = drawCard(card,deck);
			joueur->nb_carte_main ++;
			/*Si la carte est multicolor, on ne re-pioche pas*/
			if(card == MULTICOLOR){
				i++;
			}
			/*Fonction qui compte les cartes selon leur couleur*/
			compteur_carte(card,joueur);
		}
	}
	/*Coup = piocher et choisir un objectif*/
	else if(choixCoup->identifiant_coup == 4){
		/*On pioche un objectif*/
		//drawObjectives(choixCoup->obj);
		/*Il faudrait printer les objectifs*/
		affiche_objectif(choixCoup->obj);
		printf("Entrez 1 pour prendre le ou les objectifs et 0 sinon");
		//scanf("%d %d %d",&choixCoup->objectiveCards[0],&choixCoup->objectiveCards[1],&choixCoup->objectiveCards[2]);
		/*Condition piocher au moins 2 obj au 1er tour*/
		if(partie->premier_tour){
			while((choixCoup->objectiveCards[0]+choixCoup->objectiveCards[1]+choixCoup->objectiveCards[2])<2){
				printf("Au premier tour, il faut piocher au moins 2 objectifs");
				//scanf("%d %d %d",&choixCoup->objectiveCards[0],&choixCoup->objectiveCards[1],&choixCoup->objectiveCards[2]);
			}
			partie->premier_tour = 0;
		}
		/*Condition piocher au moins 1 obj si pas premier tour*/
		if(!partie->premier_tour){
			while((choixCoup->objectiveCards[0]+choixCoup->objectiveCards[1]+choixCoup->objectiveCards[2])<1){
				printf("Il faut piocher au moins 1 objectif");
				//scanf("%d %d %d",&choixCoup->objectiveCards[0],&choixCoup->objectiveCards[1],&choixCoup->objectiveCards[2]);
			}
		}
		code = chooseObjectives(choixCoup->objectiveCards);
		joueur->nb_objectif += choixCoup->objectiveCards[0]+choixCoup->objectiveCards[1]+choixCoup->objectiveCards[2];
	}
	return code;
}


// void initialisation_map_wagon(int tracks[390],int tab[78]){
// 	for(int i=0;i<78;i++){
// 		tab[i] = *(p++);
// 		printf("le track est : %d",tab[i] );
// 	}

// }

void compteur_carte(t_color card,s_joueur* joueur){
	if(card == PURPLE){
		joueur->tab_carte_couleur[0]+=1;
	}
	else if(card== WHITE){
		joueur->tab_carte_couleur[1]+=1;
	}
	else if(card == BLUE){
		joueur->tab_carte_couleur[2]+=1;
	}
	else if(card == YELLOW){
		joueur->tab_carte_couleur[3]+=1;
	}
	else if(card == ORANGE){
		joueur->tab_carte_couleur[4]+=1;
	}
	else if(card == BLACK){
		joueur->tab_carte_couleur[5]+=1;
	}
	else if(card == RED){
		joueur->tab_carte_couleur[6]+=1;
	}
	else if(card == GREEN){
		joueur->tab_carte_couleur[7]+=1;
	}
	else if(card == MULTICOLOR){
		joueur->tab_carte_couleur[8]+=1;
	}
}

void affiche_objectif(t_objective obj[3]){
	printf("\nVoici la liste des objectifs : \n");
	for(int i=0;i<3;i++){
		printf("Obj n° %d, city1 = %d ; city2 = %d ; score = %d\n",i,obj[i].city1,obj[i].city2,obj[i].score);
	}
}

void affichage_info_joueur(s_joueur* joueur){
	printf("\n\nIl vous reste %d wagons disponibles\n",joueur->wagons_disponible);
	printf("Vous avez %d objectif\n",joueur->nb_objectif);
	printf("Vous avez %d cartes en main\n",joueur->nb_carte_main);
	printf("Vous avez : ");
	printf("%d purple;  ",joueur->tab_carte_couleur[0]);
	printf("%d white;  ",joueur->tab_carte_couleur[1]);
	printf("%d blue;  ",joueur->tab_carte_couleur[2]);
	printf("%d yellow;  ",joueur->tab_carte_couleur[3]);
	printf("%d orange;  ",joueur->tab_carte_couleur[4]);
	printf("%d black;  ",joueur->tab_carte_couleur[5]);
	printf("%d red;  ",joueur->tab_carte_couleur[6]);
	printf("%d green;  ",joueur->tab_carte_couleur[7]);
	printf("%d multicolor \n\n",joueur->tab_carte_couleur[8]);	
}