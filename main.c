#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#define POSITION_DEPART 0 /*0: I begin ; =1 the opponent begin*/
//gcc -c main.c TicketToRideAPI.c clientAPI.c && gcc -o main main.o TicketToRideAPI.o clientAPI.o && ./main
int main(){
	/* #### For the function connectToServer #### */
	/*serverName*/
	char* serverName = "li1417-56.members.linode.com";
	/*port number used for the connection*/
	unsigned int port = 1234;
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
	int* tracks;
	/*array of 5 t_color giving the 5 face up cards*/
	t_color faceUp[5];
	/*array of 4 t_colors with the initial cards in your hand*/
	t_color cards[4];

	/* #### Variable pour les fonctions de notre joueur #### */
	t_color* card = (t_color*)malloc(sizeof(int));

	/* #### Variables pour la fonction get the opponent move #### */
	t_move* move = (t_move*)malloc(sizeof(t_typeMove));
	int* replay = (int*)malloc(sizeof(int));

	/*Variable qui contient la valeur de la fonction joué*/
	t_return_code code;
	/*Variable de notre position de départ*/
	int start = POSITION_DEPART;

	/* ######## Connection to the server ######## */
	connectToServer(serverName,port,name);
	/* ######## Initialisation for cards, map and the beginner ######## */
	waitForT2RGame("TRAINING DO_NOTHING timeout=100 start=1 map=USA",gameName,&nbCities,&nbTracks);
	/*On alloue la taille nécessaire */
	tracks = malloc(nbTracks*5*sizeof(int));
	/*######## Initialisation for the map ########
	Get the map, the decks and initial cards and tell who starts*/
	getMap(tracks,faceUp,cards);
	/* ######## Display of the map ######## */
	printMap();

	while(1){
		/*Position de départ*/
		if(start==1){
			start = 2;
			/*Si c'est à l'adversaire de commencer*/
			goto depart;
		}
		/*Notre tour*/
		for(int i=0;i<2;i++){
				/*on joue, ici on pioche*/
				code = drawBlindCard(card);
				/*Conditions si le coup est gagnant, perdant ou si on continue*/
				if(code==NORMAL_MOVE){
					printf("Vous avez pioché une carte");
				}else if(code==WINNING_MOVE){
					printf("Vous avez gagné");
					return code;
				}else{
					printf("Vous avez perdu");
					return code;
				}
				/*Affichage des coups du joueurs pas à pas*/
				printMap();
		}
		depart: /*Etiquette lorsque l'adversaire commence le jeu*/
		for(int i=0;i<2;i++){
			code = getMove(move,replay);
			if(!replay){
				i++;
			}
			if(code==NORMAL_MOVE){
				printf("l'adversaire a pioché une carte");
			}else if(code==WINNING_MOVE){
				printf("Vous avez perdu");
				return code;
			}else{
				printf("Vous avez gagné");
				return code;
			}
			/*Affichage des coups de l'adversaire pas à pas*/
			printMap();
		}
	}
	/* #### Close the connection to the server #### */
	closeConnection();
	free(tracks);
	return 1;
}
