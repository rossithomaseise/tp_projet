#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"

int main(){
	/*serverName*/
	char* serverName = "li1417-56.members.linode.com";
	/*port number used for the connection*/
	unsigned int port = 1234;
	/*Name of the bot*/
	char* name = "Thomas";
	
	/* ## For the function waitForT2RGame ## */
	/*Type of the game we want to play*/
	char* gameType = "TRAINING DO_NOTHING";
	/*Name of the game*/
	char gameName[50];
	/*the number of cities*/
	int nbCities ;
	/*the number of tracks between the cities*/
	int nbTracks ;

	/* ## For the function getMap ## */
	int* tracks;
	/*array of 5 t_color giving the 5 face up cards*/
	t_color faceUp[5];
	/*array of 4 t_colors with the initial cards in your hand*/
	t_color cards[4];

	/* #### Connection to the server #### */
	connectToServer(serverName,port,name);
	/* #### Initialisation for cards, map and the beginner #### */
	waitForT2RGame(gameType,gameName,&nbCities,&nbTracks);
	tracks = malloc(nbTracks*5*sizeof(int));
	getMap(tracks,faceUp,cards);
	printMap();
	/* #### Close the connection to the server #### */
	closeConnection();
	free(tracks);

//gcc -c main.c TicketToRideAPI.c clientAPI.c && gcc -o main main.o TicketToRideAPI.o clientAPI.o && ./main


	return 1;
}