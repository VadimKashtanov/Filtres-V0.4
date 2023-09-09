#include "mdl.h"

static float filtre_n(float * x, float * filtre, uint n) {  //[0;1]
	float _min=x[0], _max=x[0];
	float _x;
	for (uint i=1; i < n; i++) {
		_x = x[i];
		if (_x > _max) _max = _x;
		if (_x < _min) _min = _x;
	}
	if (_min == _max) {
		_min=0;_max=1;
		for (uint i=0; i < n; i++) x[i]=0.5;
		//ERR("Impossible de normaliser un vecteur uniforme");
	}

	//
	float x_norm[n];

	//
	float _s = 0;
	const float d = _max-_min;
	float tmp, signe;
	for (uint i=0; i < n; i++) {
		x_norm[i] = (x[i]-_min)/d;
		_s += sqrtf(1.0f+fabs(x_norm[i] - filtre[i]));
	}
	_s = _s/n - 1;

	//
	float _d = 0.0;
	for (int i=0; i < n-1; i++) {
		_d += powf(1.0f + fabs((x_norm[i+1]-x_norm[i]) - (filtre[i+1]-filtre[i])),2);
	}
	_d = _d/(n-1) - 1;

	//
	return ___exp(-_s*_s - _d*_d);
};

//========================================= Neurone Moyenne Biaisee =======================================

static float neurone_n(float * x, float * poid, uint n) {       //[-1;1]
	float _somme = 0.0;
	float coef = 0;
	for (uint i=0; i < n; i++) {
		_somme += x[i]*poid[i];
		coef += fabs(poid[i]);
	}
	return _somme/coef;	//si que des 1, alors coef==n
};

//========================================  Neurone Condition  ===========================================

uint update(uint * indexs, uint * max, uint * min, uint n) {
	if (n==0)
		return 0;
	//
	if (min == 0) {
		for (uint i=0; i < n; i++) {	//size_t is faster than int. Using updata don't instead classic while loop isn't realy afecting time
			indexs[i]++;					//Update i cursor
			if (indexs[i] != max[i]) break;	//If undex max : go to next update call
			else if (i == n-1) return 0;	//Else : Overflow of current cursor. If last => end of loop
			else indexs[i] = 0;				//Last case : set current cursor to 0 and ++ next cursor
		}
		return 1;	//Loop isn't ended, can next update()
	} else {
		for (uint i=0; i < n; i++) {	//size_t is faster than int. Using updata don't instead classic while loop isn't realy afecting time
			indexs[i]++;					//Update i cursor
			if (indexs[i] != max[i]) break;	//If undex max : go to next update call
			else if (i == n-1) return 0;	//Else : Overflow of current cursor. If last => end of loop
			else indexs[i] = min[i];				//Last case : set current cursor to 0 and ++ next cursor
		}
		return 1;	//Loop isn't ended, can next update()
	}
};	//To use update(), creat indexs and fill by 0, set max, then : do {} while (updata(i,max,n));

static float neurone_condition_n(float * x, float * poid, uint n) { //[-1;+1]
    if (n < 2) {
        ERR("Comparer 0 ou 1 valeurs entre elles ?");
    } else if (n == 2) {
        float _00 = (  (1-(x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) )*poid[0];
        float _01 = (  ((x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) )*poid[1];
        float _10 = (  (1-(x[0]+1)/2)*x[0] + ((x[1]+1)/2) )*poid[2];
        float _11 = (  ((x[0]+1)/2)*x[0] + ((x[1]+1)/2) )*poid[3];
        return (_00+_01+_10+_11);
    } else if (n == 3) {
        float _000 = (  (1-(x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) + (1-(x[2]+1)/2) )*poid[0];
        float _001 = (  (1-(x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) + ((x[2]+1)/2) )*poid[1];
        float _010 = (  (1-(x[0]+1)/2)*x[0] + ((x[1]+1)/2) + (1-(x[2]+1)/2) )*poid[2];
        float _011 = (  (1-(x[0]+1)/2)*x[0] + ((x[1]+1)/2) + ((x[2]+1)/2) )*poid[3];
        float _100 = (  ((x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) + (1-(x[2]+1)/2) )*poid[4];
        float _101 = (  ((x[0]+1)/2)*x[0] + (1-(x[1]+1)/2) + ((x[2]+1)/2) )*poid[5];
        float _110 = (  ((x[0]+1)/2)*x[0] + ((x[1]+1)/2) + (1-(x[2]+1)/2) )*poid[6];
        float _111 = (  ((x[0]+1)/2)*x[0] + ((x[1]+1)/2) + ((x[2]+1)/2) )*poid[7];
        return (_000+_001+_010+_011+_100+_101+_110+_111);
    } else {
        //update...  
        uint __max[n];
        uint __min[n];
        uint __id[n];
        FOR(0, i, n) {__max[i]=1+1; __min[i]=0; __id[i]=0;};
        uint max_iter = (1 << (n+1));   //+1 pour la securitee
        uint i=0;
        float _somme = 0;
        float mul;
        do {
            FOR(0, j, n) {
                mul = 1;
                FOR(0, k, n) mul *= (__id[k] == 0 ? (1.0-(x[k]+1.0)/2.0) : (x[k]+1.0)/2.0);
                //(1-(a+1)/2 if i==0 else (a+1)/2)   *   (1-(b+1)/2 if j==0 else (b+1)/2)
                _somme += mul * poid[i];
            };
            assert(i++ < max_iter);
        } while (update(__id, __max, __min, n) != 0);
        return _somme;
    }
};

//===========================================================================================

float f(Mdl_t * mdl, uint depart) {
    uint C = mdl->C;
	uint * type = mdl->type;
	uint * y = mdl->y;
	uint * n = mdl->n;

	uint d;

	//	Filtres N
	float _x[N_MAX];

    //  Couche[0], fltr prixs
	FOR(0, i, y[0]) {
		uint _ema = mdl->ema[i];
		uint interv = intervalles[mdl->intervalles[i]];
		//
		FOR(0, j, n[0]) {
			_x[j] = ema[_ema][depart - j*interv];
		}

		mdl->var[i] = filtre_n(
			_x, mdl->conste + i*CONSTES_FLTR(n[0]), n[0]);
	};

    //  Autres insts principales

	FOR(1, i, C) {
		FOR(0, j, y[i]) {
			if (type[i] == 1) {
                //  Filtre 
				mdl->var[mdl->y_depart[i] + j] = filtre_n(
					mdl->var + mdl->y_depart[i-1] + mdl->flt_depuis[i][j],
					mdl->conste + mdl->conste_depart[i] + mdl->n[i]*j,
					mdl->n[i]
				);
			} else if (mdl->type[i] == 2) {
                //   ====   Neurone ==== 
				d = (C-i);
				//
				FOR(0, k, n[i]) {
					_x[k] = mdl->var[mdl->y_depart[i-1] + mdl->neu_depuis[i][j][k]];
				}
				//
				mdl->var[mdl->y_depart[i] + j] = neurone_n(
					_x, mdl->poid + mdl->poid_depart[i] + j*POIDS_NEU(mdl->n[i]),
					mdl->n[i]
				);
            } else if (mdl->type[i] == 3) {
                //  ======= Neurone Condition ======
                d = (C-i);
				//
				FOR(0, k, n[i]) {
					_x[k] = mdl->var[mdl->y_depart[i-1] + mdl->neu_depuis[i][j][k]];
				}
				//
				mdl->var[mdl->y_depart[i] + j] = neurone_condition_n(
					_x, mdl->poid + mdl->poid_depart[i] + j*POIDS_COND(mdl->n[i]),
					mdl->n[i]
				);
                
    		} else {
				ERR("Pas de couche %i", mdl->type[i]);
			}
		}
	}
	//
	return mdl->var[mdl->vars-1];
};


















