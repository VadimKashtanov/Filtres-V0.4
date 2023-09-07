
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

typedef struct {
	//	Config dimentions & entree de la pile
	uint C;
	uint * y, * n, * type;
	uint * ema, * intervalles;

	uint max_n;

	//
	//	Une nouvelle couche Neuronique
	//

	uint *** neu_depuis;	//[couche][neurone  ][connection] pris dans {0..y[-1]}
	uint **  flt_depuis;	//[couche][filtre   ] 			  pris dans {0..(y[-1] - n)}

	//	Espace de Parametres & Calcules
	uint constes, poids, vars;
	float * conste, * poid, * var;

	//	Depart de chaque couche dans les espaces
	uint * conste_depart, * poid_depart, * y_depart;
} Mdl_t;