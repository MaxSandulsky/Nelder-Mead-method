#include "polyhedron.h"
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

using namespace std;

Polyhedron::Polyhedron(double coefAccuracy, double limits_XDown, double limits_XUp, double limits_YDown, double limits_YUp, double limits_ZDown, double limits_ZUp) {

	accuracy = coefAccuracy;

	switcher = 0;

	lims[0][0] = limits_XDown;
	lims[0][1] = limits_XUp;

	lims[1][0] = limits_YDown;
	lims[1][1] = limits_YUp;

	lims[2][0] = limits_ZDown;
	lims[2][1] = limits_ZUp;

	coefProxy = 0;
	erroCounter = 0;

	makeCorners();

}

bool Polyhedron::makeCorners() {

	srand((unsigned)time(NULL));

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{

			double dRand = rand();

			while (dRand < lims[j][0] || dRand > lims[j][1]) {

				dRand /= 3;

			}
			corners[i][j] = dRand;

		}
	}
	return true;
}
//Return deviant
int Polyhedron::analize() {

	//Check convergency

	for (size_t i = 0; i < 4; i++)
	{
		switcher = i;
		reflect(0);
		if (pow((pow((dWeightofTriangle[0] - corners[switcher][0]), 2) + pow((dWeightofTriangle[1] - corners[switcher][1]), 2) + pow((dWeightofTriangle[0] - corners[switcher][1]), 2)), (1 / 2)) < accuracy || switcher == 4) {
			switcher = 4;
			return switcher;
		}
	}

	//Find weights
	for (size_t i = 0; i < 4; i++)
	{
		dWeights[i] = targetFunction(corners[i][0], corners[i][1], corners[i][2]);
		dSortedWeights[i] = dWeights[i];
	}

	std::sort(dSortedWeights, dSortedWeights + 4);

	//Find deviant
	for (size_t i = 0; i < 4; i++)
	{
		if (dWeights[i] == dSortedWeights[3])
		{
			switcher = i;
		}

	}

		//Check for Error
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (corners[j][i] < lims[i][0]) {

				switcher = 5;
				return switcher;
			}

			if (corners[j][i] > lims[i][1]) {

				switcher = 5;
				return switcher;
			}
		}
	}

	if (dWeights[0] == dWeights[1] && dWeights[0] == dWeights[2] && dWeights[0] == dWeights[3])
	{
		switcher = 6;

		return switcher;
	}

	return switcher;
}

double Polyhedron::reflection() {

	double y_res = reflect(1);

	if (switcher >= 4) {
		return switcher;
	}

	if (y_res < dSortedWeights[0])
	{

		y_res = reflect(2);

		if (y_res < dSortedWeights[0])
		{
			
			corners[switcher][0] = dWeightofTriangle[0];
			corners[switcher][1] = dWeightofTriangle[1];
			corners[switcher][2] = dWeightofTriangle[2];

			return 0;
		}
		else {

			y_res = reflect(1);

			corners[switcher][0] = dWeightofTriangle[0];
			corners[switcher][1] = dWeightofTriangle[1];
			corners[switcher][2] = dWeightofTriangle[2];

			return 0;
		}
	}
	else if (y_res > dSortedWeights[0] && y_res < dSortedWeights[1]) {

		corners[switcher][0] = dWeightofTriangle[0];
		corners[switcher][1] = dWeightofTriangle[1];
		corners[switcher][2] = dWeightofTriangle[2];

		return 0;

	}
	else if (y_res < dSortedWeights[3] && y_res > dSortedWeights[2]) {

		for (size_t i = 0; i < 3; i++)
		{
			swap(dWeightofTriangle[i], corners[switcher][i]);

		}

	}

	y_res = shrinkage();

		if (y_res < dSortedWeights[3])
		{

			corners[switcher][0] = dWeightofTriangle[0];
			corners[switcher][1] = dWeightofTriangle[1];
			corners[switcher][2] = dWeightofTriangle[2];

			return 0;

		}
		else {

			for (size_t i = 0; i < 4; i++)
			{
				if (dWeights[i] == dSortedWeights[0])
				{
					switcher = i;
				}

			}

			compress();
			reflect(0);

			for (int i = 1; i < 4; i++) {
				dWeightofTriangle[i] = corners[switcher][i - 1] - dWeightofTriangle[i - 1];
			}

			y_res = dSortedWeights[3];
			double y_ref = dSortedWeights[3];
			double dVector_ref[3];
			
			bool fLag = true;

			while (y_res <= y_ref && fLag) {
				for (size_t i = 0; i < 3; i++)
				{
					dCoords[0][i] = corners[switcher][i] + dWeightofTriangle[i];

					dWeightofTriangle[i] += dWeightofTriangle[i];
					dVector_ref[i] = dWeightofTriangle[i]/10;
				}

				//Настроить функцию возврата в зону видимости

				for (size_t i = 0; i < 3; i++) {
					if (dCoords[0][i] > lims[i][1] || dCoords[0][i] < lims[i][0]) {
						while (dCoords[0][i] > lims[i][1] || dCoords[0][i] < lims[i][0])
						{
							if (dCoords[0][i] < 1 && dCoords[0][i] > -1) {

								switcher = 4;
								return targetFunction(corners[switcher][0], corners[switcher][i], corners[switcher][2]);
							}
							else if (dCoords[0][i] < -2000 || dCoords[0][i] >2000) {

								switcher = 5;
								return targetFunction(corners[switcher][0], corners[switcher][i], corners[switcher][2]);

							}

							for (size_t k = 0; k < 3; k++)
							{
								dCoords[0][i] -= dVector_ref[i];
							}
						}
					}
				}

				y_res = targetFunction(dCoords[switcher][0], dCoords[switcher][1], dCoords[switcher][2]);

				if (y_res < y_ref) {

					y_ref = y_res;
				}
				else {
					fLag = false;
				}
			}

			//Find deviant
			for (size_t i = 0; i < 4; i++)
			{
				if (dWeights[i] == dSortedWeights[3])
				{
					switcher = i;
				}
			}

			if (y_ref < dSortedWeights[3])
			{
				for (size_t i = 0; i < 3; i++)
				{
					corners[switcher][i] = dCoords[0][i];
				}
			}
		}
		return 0;
}

double Polyhedron::reflect(int iMultiplier) {

	double dTemporal[3];
	

	for (size_t i = 0; i < 3; i++)
	{
		dTemporal[i] = 0;

		for (size_t j = 0; j < 4; j++)
		{
			if (j != switcher)
			{
				dTemporal[i] += corners[j][i];
			}
		}

		dTemporal[i] /= 3;

		dWeightofTriangle[i] = dTemporal[i] + (iMultiplier * (dTemporal[i] - corners[switcher][i]));

		dCoords[0][i] = (corners[switcher][i] - dTemporal[i]) / 10;

		if (fabs(dCoords[0][i]) < accuracy) {
			if (coefProxy == 40) {
				switcher = 4;
				coefProxy = 0;
				return targetFunction(corners[switcher][0], corners[switcher][i], corners[switcher][2]);
			}
			else {
				coefProxy++;
			}
			dCoords[0][i] *= 10;
		}
	}

	for (size_t i = 0; i < 3; i++) {
		if (dWeightofTriangle[i] > lims[i][1]) {
			while (dWeightofTriangle[i] > lims[i][1])
			{		
				for (size_t k = 0; k < 3; k++)
				{
					dWeightofTriangle[k] += dCoords[0][k];
					if (erroCounter > 10000)
					{

						switcher = 4;
						coefProxy = 0;
						erroCounter = 0;
						return targetFunction(corners[switcher][0], corners[switcher][i], corners[switcher][2]);
					}
					else {
						erroCounter++;
					}
				}
			}
		}

		if (dWeightofTriangle[i] < lims[i][0]) {
			while (dWeightofTriangle[i] < lims[i][0])
			{
				for (size_t k = 0; k < 3; k++)
				{
					dWeightofTriangle[k] += dCoords[0][k];
					if (erroCounter > 10000)
					{

						switcher = 4;
						coefProxy = 0;
						erroCounter = 0;
						return targetFunction(corners[switcher][0], corners[switcher][i], corners[switcher][2]);
					}
					else {
						erroCounter++;
					}
				}
			}
		}
	}

	double y_res = targetFunction(dWeightofTriangle[0], dWeightofTriangle[1], dWeightofTriangle[2]);

	return y_res;
}

double Polyhedron::shrinkage() {

	double y_res = reflect(0);

	for (size_t i = 0; i < 3; i++)
	{
		dWeightofTriangle[i] = (dWeightofTriangle[i] + corners[switcher][i]) / 2;
	}

	y_res = targetFunction(dWeightofTriangle[0], dWeightofTriangle[1], dWeightofTriangle[2]);

	return y_res;
}

double Polyhedron::compress() {

	double dTemporal[4][3];
	double y_res;

	//count steps for each axes
	for (size_t j = 0; j < 4; j++)
	{
		if (j != switcher) {
			for (size_t i = 0; i < 3; i++)
			{

				dCoords[j][i] = (corners[switcher][i] - corners[j][i]) / 10;
			}
		}
	}

		for (size_t j = 0; j < 4; j++)
		{
			for (size_t f = 0; f < 10; f++) {
				if (j != switcher) {
					for (size_t i = 0; i < 3; i++)
					{

						dTemporal[j][i] = corners[j][i] + (dCoords[j][i] * f);

					}

					y_res = targetFunction(dTemporal[j][0], dTemporal[j][1], dTemporal[j][2]);

					if (y_res < dWeights[j])
					{
						dWeights[j] = y_res;
						f = 10;
						for (size_t i = 0; i < 3; i++)
						{

							corners[j][i] = dTemporal[j][i];

						}
					}
				}
				else {
					f = 10;
				}
			}
		}

	return y_res;
}

double Polyhedron::targetFunction(double x, double y, double z) {

	double y_res = pow(x, 2) - 2 *( x * y) + pow(y, 2) + z - z * pow(x, 2);

	return y_res;

}