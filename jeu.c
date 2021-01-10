#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

int distanceMini(int D[36],int Visite[36]){
	int min; /*valeur du minimum*/
	int indice_min; /*indice du minimum*/
	min = 100; /*= + infini*/
	/*recherche de la ville non visitée la plus proche*/
	for(int i=0;i<36;i++){
		if(Visite[i] == 0 && D[i] < min){
			min = D[i];
			indice_min = i;
		}
	}
	return indice_min;
}

void affiche_chemin_dest_src(int src, int dest,int Prec[36],s_joueur* joueur){
	int j = 1;
	/* on démarre à la fin*/
	int v = dest; /* on remonte le chemin jusqu'à arriver au départ*/
	/*tab pour récupérer l'indice des villes du chemin de l'obj*/
	int tab_chemin_obj_temp[10];
	/*Initialisation de tab_chemin_obj_temp*/
	for(int i=0;i<10;i++){
		tab_chemin_obj_temp[i] = 100;
	}
	//printf("La destination est %d\n",dest);
	tab_chemin_obj_temp[0] = dest; 
	while(v != src){
		//printf("Le chemin est : %d\n",Prec[v]);
		tab_chemin_obj_temp[j] = Prec[v];
		/*Afficher le chemin qui va de Prec[v] à v*/
		v = Prec[v];
		j++;
	}
	int f = 9;
	for(int i=0;i<10;i++){
		if(tab_chemin_obj_temp[f-i]!=100){
			joueur->tab_chemin_obj[i] = tab_chemin_obj_temp[f-i];
			printf(	"l'ordre du chemin est %d\n",joueur->tab_chemin_obj[i]);
		} 
		/////////////////////////////
	}
}

void algo_dijkstra(int src, int route_distance[36][36],int D[36],int Prec[36],int dest){
	/* tableau de booléens = 0 si n'a pas été visité (false) et 1 sinon(true)*/
	int Visite[36];
	/*indice qui indique la ville considérée dans la boucle*/
	int u = src;

	/*INITIALISATION*/
	for(int i=0;i<36;i++){
		D[i] = 100; /*infini*/
		Visite[i] = 0; /*false*/
	}
	/*On part de la source*/
	D[src] = 0;
	/*On cherche le plus court chemin pour aller de src à chaque ville*/
	for(int f=0;f<36;f++){
		//printf("ola");
		/*On trouve la ville non visité la plus proche*/
		u = distanceMini(D,Visite);
		/*On marque cette ville comme visitée*/
		Visite[u] = 1; /* true*/
		/* On met à jour chaque ville reliée à src*/
		for(int v=0;v<36;v++){
			/*on ne met à jour que les villes non visités, où il y a un chemin entre u et v et où il est plus court
			d'arriver à v en passant par u*/
			if(Visite[v] == 0  && route_distance[u][v] < 100 && D[u] + route_distance[u][v] < D[v]){
				D[v] = D[u] + route_distance[u][v];
				Prec[v] = u;
			}
		}
	}
}

t_return_code mon_bot(s_partie* partie,s_choixCoup* choixCoup,s_joueur* joueur,int tracks[390],t_color deck[5]){
	/*Variable pour enregistrer la valeur de retour de la fonction choisie*/
	t_return_code code;
	int f = 0;/*pour gérer le joueur->tab_chemin_obj[i]*/
	/*premier tour*/
	 if(partie->numero_tour == 0){
	 	drawObjectives(choixCoup->obj);
		/*On choisit les 2 objectifs les plus grands*/
		if(choixCoup->obj[0].score >= choixCoup->obj[1].score && choixCoup->obj[0].score >= choixCoup->obj[2].score){
			affiche_objectif(choixCoup->obj);
			choixCoup->objectiveCards[0] = 1;
			/*Il devient l'objectif le plus important*/
			joueur->tab_objectif_ordre[0] = 0;
			if(choixCoup->obj[1].score > choixCoup->obj[2].score){
				choixCoup->objectiveCards[1] = 1;
				choixCoup->objectiveCards[2] = 0;
				/*Il devient le 2ème objectif le plus important*/
				joueur->tab_objectif_ordre[1] = 1;

			}
			else{
				choixCoup->objectiveCards[2] = 1;
				choixCoup->objectiveCards[1] = 0;
				/*Il devient le 2ème objectif le plus important*/
				joueur->tab_objectif_ordre[1] = 2;	
			}
		}
		else{
			affiche_objectif(choixCoup->obj);
			choixCoup->objectiveCards[1] = 1;
			choixCoup->objectiveCards[2] = 1;
			choixCoup->objectiveCards[0] = 0;	
			/*On regarde l'objectif le plus grand*/
			if(choixCoup->obj[1].score > choixCoup->obj[2].score){
				joueur->tab_objectif_ordre[0] = 1;
				joueur->tab_objectif_ordre[1] = 2;
			}
			else{
				joueur->tab_objectif_ordre[0] = 2;
				joueur->tab_objectif_ordre[1] = 1;
			}
		}
		choix_coup(choixCoup);
		choixCoup->identifiant_coup = 4;
		code = action_coup(tracks,deck,choixCoup,joueur,partie);
		printf("\nVotre objectif principal est : %d\n",joueur->tab_objectif_ordre[0]);
		printf("Votre objectif secondaire est : %d\n",joueur->tab_objectif_ordre[1]);	
		return code;
	}
	/*tour suivant*/
	else{
		/*si aucun objectif est rempli, alors on en pioche pas d'autre et la priorité reste sur l'obj principal*/
		if(joueur->objectif_rempli == 0){
			/*On regarde si on peut poser une route*/
			/* Indique la plus ourte distance entre les villes src et i*/
			int D[36];
			/*Indique de quelle ville il faut venir pour aller de i à src*/
			int Prec[36]={0};
			int src = choixCoup->obj[joueur->tab_objectif_ordre[0]].city1;
			int dest = choixCoup->obj[joueur->tab_objectif_ordre[0]].city2;
			algo_dijkstra(src,partie->route_distance,D,Prec,dest);
			affiche_chemin_dest_src(src,dest,Prec,joueur);
			
			for(int i=0;i<10;i++){


					printf(	"l'ordre du chemin est %d\n",joueur->tab_chemin_obj[i]);

			}
			int city1,city2;
			/*On regarde si on a bien city1 < city2 */
			if(joueur->tab_chemin_obj[0] < joueur->tab_chemin_obj[0+1]){
				city1 = joueur->tab_chemin_obj[0];
				city2 = joueur->tab_chemin_obj[0+1];
			}
			else{
				city1 = joueur->tab_chemin_obj[0+1];
				city2 = joueur->tab_chemin_obj[0];	
			}

			
			printf("\n city 1 %d et city 2 %d\n",src,city2);	
			printf("\n city 1 %d et city 2 %d\n",joueur->tab_chemin_obj[0],joueur->tab_chemin_obj[1]);	
			printf("\non a route_d %d\n",partie->route_distance[city1][city2]);
			printf("\n somme %d\n",joueur->tab_carte_couleur[partie->route_couleur[city1][city2]]);
			if(partie->route_distance[city1][city2] <= joueur->tab_carte_couleur[partie->route_couleur[city1][city2]] + joueur->tab_carte_couleur[8]
				&& partie->route_dispo[city1][city2]!=1){
				choixCoup->identifiant_coup = 1;
				choixCoup->city1 = city1;
				choixCoup->city2 = city2;
				choixCoup->color = joueur->tab_carte_couleur[partie->route_couleur[city1][city2]];
				choixCoup->nbLocomotives = joueur->tab_carte_couleur[8];
				code = action_coup(tracks,deck,choixCoup,joueur,partie);
				return code;
			}
		}
		choixCoup->identifiant_coup = 2;
		code = action_coup(tracks,deck,choixCoup,joueur,partie);
		return code;
	}
}
