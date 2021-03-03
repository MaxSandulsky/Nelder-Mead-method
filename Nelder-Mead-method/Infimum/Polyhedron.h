#pragma once
class Polyhedron
{

public: 

	double corners[4][3];
	double dWeights[4];
	double dWeightofTriangle[4];
	double dCoords[4][3];
	double dSortedWeights[4];

	double lims[3][2];

	double accuracy;

	int switcher;
	int coefProxy;
	int erroCounter;
	Polyhedron(double, double, double, double, double, double, double);

	bool makeCorners();

	int analize();

	double targetFunction(double, double, double);

	double reflection();

	double reflect(int);

	double compress();

	double shrinkage();

	~Polyhedron() {
		
	}
};

