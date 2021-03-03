#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <C:\Users\Elucidater\source\repos\Infimum\Infimum\polyhedron.h>
# define _DEBUG

using namespace std;

int main() {
	
	double limits_XDown = 0.38, limits_XUp = 1.77, limits_YDown = 1.3,
	limits_YUp = 5.7, limits_ZDown = 0.8, limits_ZUp = 2.1, coefAccuracy = 0.0001;

	char ch;

	Polyhedron polyhedron = Polyhedron(coefAccuracy, limits_XDown, limits_XUp, limits_YDown, limits_YUp, limits_ZDown, limits_ZUp);

	srand((unsigned)time(NULL));

	int tri = 0;
	int ty = 50;
	while(true) {

		if (tri > ty)
		{
			polyhedron.switcher = 7;
		}
		else {
			tri++;
		}

		if (polyhedron.switcher == 4) {
			goto _4;
		}

		polyhedron.analize();

		switch (polyhedron.switcher)
		{
		case 0: goto _3;
		case 1: goto _3;
		case 2: goto _3;
		case 3: goto _3;
			_3:
			polyhedron.reflection();
			break;
		case 4:
			_4:
#ifdef _DEBUG
			for (size_t i = 0; i < 4; i++)
			{
				if (polyhedron.dWeights[i] == polyhedron.dSortedWeights[0])
				{
					polyhedron.switcher = i;
				}

			}

			std::cout << tri << "Got infimum :" << polyhedron.dSortedWeights[0] << endl << "Corresponding coords: " << endl << "A1: " << polyhedron.corners[polyhedron.switcher][0]	<< endl << "A2: " << polyhedron.corners[polyhedron.switcher][1]	<< endl << "A3: " << polyhedron.corners[polyhedron.switcher][2]	<< endl << "Try again? " << endl;
			tri = 0;
			

			ch = _getch();

			if (ch == 'x') {
				return 0;
			}
			else {
				polyhedron.makeCorners();
				polyhedron.switcher = 0;
				break;
			}
#endif
				polyhedron;
				polyhedron;
				return 0;
			break;
		case 5:

			std::cout << "Gone out of limits" << endl;
			std::cout << tri << "Last infimum :" << polyhedron.dSortedWeights[0] << endl << "Corresponding coords: " << endl << "A1: " << polyhedron.corners[polyhedron.switcher][0] << endl << "A2: " << polyhedron.corners[polyhedron.switcher][1] << endl << "A3: " << polyhedron.corners[polyhedron.switcher][2] << endl << "Try again? " << endl;
			
			polyhedron.makeCorners();
			polyhedron.switcher = 0;
			tri = 0;

			break;
		case 6:
			std::cout << "Got quilibrity of weights" << endl;

			return 0;
			break;
		case 7:
			std::cout << "Run out of tries: Continue?" << endl;

			tri = 0;

			ch = _getch();

			if (ch == 'x') {
				return 0;
			}
			else {
				ty += ty;
				polyhedron.switcher = 0;
				break;
			}

			polyhedron.switcher = 0;
			tri = 0;
			break;
		default:
			break;

		}

	}

	return 0;
}