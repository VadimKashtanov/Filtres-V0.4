#pragma once

#include "marchee.h"

//	Type de couches:
//		<Neuronalle>
//		<Filtrique>
//	Chaque couche utilise son type de connection
//		neu_depuis
//		flt_depuis
//
//	Les couches :
//		0 - fltr prixs
//		1 - fltr
//		2 - neu<n>
//		3 - cond

//	Cond si n=2
//def f(a,b, p=[1,2,3,4]):
//  return sum(((1-(a+1)/2 if i==0 else (a+1)/2) * (1-(b+1)/2 if j==0 else (b+1)/2))*p[i*2+j] for i in (0,1) for j in (0,1))
//
// si n=3 il faut faire toutes les combinaisons de [i,j,k] and {0,1}^3
// et chaque condition a son poid, donc 2^n poids
//
//def f(a,b,c, p=[1,2,3,4,5,6,7,8]):
//   return sum(((1-(a+1)/2 if i==0 else (a+1)/2) * (1-(b+1)/2 if j==0 else (b+1)/2) * (1-(c+1)/2 if k==0 else (c+1)/2))*p[i*2*2+j*2+k] for i in (0,1) for j in (0,1) for k in (0,1))
//
//f(-1,-1,-1) == 1
//f(1,1,1) == 8
//f(0,0,0) == 4.5

#define C_max 10
#define Y_max 50

#define N0_max 8
#define N1_max 8
#define N2_max 5
#define N3_max 3

#define N_MAX MAX4(N0_max, N1_max, N2_max, N3_max)

#define POIDS_NEU(n) n         //
#define POIDS_COND(n) (1<<n)//FACT(n)  //  Si il y a n valeurs, il y a donc n*(n-1)*(n-2)*(n-3)...*1 combinaisons posibles
#define CONSTES_FLTR(n) n       //

#define P0_max 0
#define P1_max 0
#define P2_max POIDS_NEU(N2_max)
#define P3_max POIDS_COND(N3_max)

#define C0_max CONSTES_FLTR(N1_max)
#define C1_max CONSTES_FLTR(N1_max)
#define C2_max 0
#define C3_max 0

#define POIDS_max MAX4(P0_max, P1_max, P2_max, P3_max)
#define CONSTE_max MAX4(C0_max, C1_max, C2_max, C3_max)

typedef struct {
	//	Config dimentions & entree de la pile
	uint C;
	uint y[C_max], n[C_max], type[C_max];
	uint ema[Y_max], intervalles[Y_max];

	//  `Depuis` de chaque couche : Quelle valeur va chercher chaque element dans la couche d'avant
	uint neu_depuis[C_max][Y_max][MAX2(N2_max, N3_max)];	//[couche][neurone  ][connection] pris dans {0..y[-1]}
	uint flt_depuis[C_max][Y_max];	                        //[couche][filtre   ] 			  pris dans {0..(y[-1] - n)}

	//	Espace de Parametres & Calcules
	uint constes, poids, vars;
	float conste[Y_max*CONSTE_max], poid[Y_max*POIDS_max], var[Y_max*C_max];

	//	Depart de chaque couche dans les espaces
	uint conste_depart[C_max], poid_depart[C_max], y_depart[C_max];
} Mdl_t;

//  sizeof(Mdl_t)=5656=5ko avec C=5, Y=30      (N ~ 3..8)
//  sizeof(Mdl_t)=30656=30ko avec C=10, Y=100  (N ~ 3..8)
//  sizeof(Mdl_t)=97176=0.1Mo avec C=15, Y=200 (N ~ 3..8)

//	Allocation Memoire
Mdl_t * cree_mdl(uint C, uint * y, uint * n, uint * type);
void liberer_mdl(Mdl_t * mdl);
Mdl_t * copier_mdl(Mdl_t * mdl);

//  Controle
uint meme_mdl(Mdl_t * A, Mdl_t * B);
uint hash_mdl(Mdl_t * mdl);

//	Memoire dure
void ecrire_mdl(Mdl_t * mdl, char * file);
Mdl_t * lire_mdl(char * file);

//	Plume
void plume_mdl(Mdl_t * mdl);
void comportement(Mdl_t * mdl);

//	Qlqs fonctions directes
float poid_neu_rnd();   //{-2,-1,-.5,0,+.5,+1,+2}
float poid_cond_rnd();

//	Calcule f(x)
float f(Mdl_t * mdl, uint depart);










