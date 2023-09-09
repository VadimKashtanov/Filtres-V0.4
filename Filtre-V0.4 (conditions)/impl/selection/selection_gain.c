#include "selection.h"

void gain(Mdl_t * mdl, float * _gain, float * _prediction) {
	_gain[0] = USDT;
	_prediction[0] = 0.0;

	float _f, p0, p1;
	for (uint i=DEPART; i < PRIXS-1-3; i++) {
		p1 = prixs[i+2];
		p0 = prixs[i];
		//
		_f = signe( f(mdl, i) );
		
		if (_gain[0] > 0) _gain[0] += _gain[0] * _f * 0.20 * LEVIER * (p1/p0 - 1.0);	//20%
		//
		if (signe(p1/p0-1.0) == _f)
			_prediction[0] += 1.0;

		//
#define div_par 5
		i += div_par;
	};
	if (_gain[0] < 0) _gain[0] = 0;

	_gain[0] -= USDT;
	_prediction[0] /= (float)(PRIXS-1-DEPART-3);
	_prediction[0] *= (1+div_par);

	//uint h = hash_mdl(mdl);
	//if (h == 542 || h==444) plume_mdl(mdl);
	//printf("%p [%i] : %f\n", mdl, h, _prediction[0]);
};
