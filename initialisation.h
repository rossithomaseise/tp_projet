#include "TicketToRideAPI.h"
/*Structure pour le plateau de jeu : variables constantes 
nombre de villes, nombres de chemins, tableau route??*/
typedef struct{
	int nbCities;
	int nbTracks;
	int tableau_route; /* Tracks????? */
}s_plateau_jeu; /*--- A mettre en pointeur???? --- */

/*structure pour le joueur */
typedef struct{
	int wagons_disponible; 
	int nb_carte_main;
	int nb_objectif;
	t_color cards[60];
	int tab_carte_couleur[9]; 
	/*indique le nb de couleur de cartes en main (non dispo pour l'adversaire)
	Ordre qui suit celui du fichier h*/
	t_objective objectif[10]; /*un tableau qui regroupe les objectifs (non dispo pour l'adversaire)*/
	int score; /////////////////////pas encore utilisé
	int tab_objectif_ordre[10];
	int objectif_rempli; /* =0 si aucun rempli, = 1 si un objectif, = 2 si 2 et on redémarre (je ne prends que 2 obj)*/
	int tab_chemin_obj[10];
}s_joueur;

typedef struct{
	int identifiant_coup;
	/*Données nécessaires pour CLAIM_ROUTE*/
	int city1;
	int city2;
	int color;
	int nbLocomotives;
	/*Variable pour la fonction drawObjectives*/
	t_objective obj[3];
	/*Variable pour la fonction chooseObjectives*/
	int objectiveCards[3];
}s_choixCoup;


/*structure pour les informations de la partie*/
typedef struct{
	/* Variable pour indiquer le 1er tour pour choisir au moins 2 objectifs
	=1 -> premeir tour */
	int premier_tour;
	int numero_tour;
	int numero_joueur_actuel; /*numéro du joueur en train de jouer*/
	int numero_joueur; /*numéro constant du joueur réel*/
	t_color faceUp[5]; /* cartes face visible*/
	int route_dispo[36][36]; /* id city1, id city2, 0 si disponible, 1 si prise */
	int route_distance[36][36];
	int route_couleur[36][36];
}s_partie;

void initialisation_jeu(int tracks[390],s_plateau_jeu* plateauJeu,s_joueur* joueur,s_joueur* adversaire,s_partie* partie);
void fin_jeu(int* tracks);
//void affichage_info_joueur(s_joueur* joueur);
void affichage_info_plateau_jeu(s_plateau_jeu* plateauJeu);
void affichage_info_partie(s_partie* partie);
/*Fonction qui demande à l'utilisateur de choisir un coup avec si nécessaires 
les données pour*/
void choix_coup(s_choixCoup* choixCoup);
/*Fonction qui choisit l'action a joué selon l'id sélectionné par choix_coup*/
t_return_code action_coup(int tracks[390],t_color deck[5],s_choixCoup* choixCoup,s_joueur* joueur,s_partie* partie);
void initialisation_map_wagon(int tracks[390],int tab[70]);
/*Fonction qui compte les cartes selon leur couleur*/
void compteur_carte(t_color card,s_joueur* joueur);
void affiche_objectif(t_objective obj[3]);
void affichage_info_joueur(s_joueur* joueur);

