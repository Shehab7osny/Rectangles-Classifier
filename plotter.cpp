/****************************************************************************
*Filename:    Source.c
*Author:      Shehab Hosny
*Date:	      10/31/2019
*Version:     1.0
*Description: D2S Internship Project (Rectangles Plotter)
****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ccc_win.h"

using namespace std;

void DrawRectangle(Point LowerLeft, Point UpperRight) {
	float a = LowerLeft.get_x();
	float b = LowerLeft.get_y();
	float c = UpperRight.get_x();
	float d = UpperRight.get_y();
	Point P1(a, b);
	Point P2(c, b);
	Point P3(c, d);
	Point P4(a, d);
	cwin << Line(P1, P2);
	cwin << Line(P2, P3);
	cwin << Line(P3, P4);
	cwin << Line(P4, P1);
}

int ccc_win_main() {

	vector<long> xl;
	vector<long> xr;
	vector<long> yt;
	vector<long> yb;

	ifstream Dataset;
	long a, b, c, d;
	int n;

	//Please Provide the path of the file right here
	//Please make sure to use the same format as included and use the backslash '/' not '\'
	Dataset.open("C:/Users/sheha/OneDrive/Personal Files/Silicon Vision/Mentor Graphics/Project/Main Project/Main Project/group_2.txt");

	if (!Dataset.is_open())
		exit(EXIT_FAILURE);

	int id = 1;
	while (Dataset.good()) {
		Dataset >> a;
		xl.push_back(a);
		Dataset >> b;
		yb.push_back(b);
		Dataset >> c;
		xr.push_back(c);
		Dataset >> d;
		yt.push_back(d);
	}
	
	n = xl.size();

	a = xl[0];
	b = yb[0];
	c = xr[0];
	d = yt[0];

	for (int i = 0; i < n; i++) {
		if (xl[i] < a)
			a = xl[i];

		if (yb[i] < b)
			b = yb[i];
	}

	if (a < 0) {
		a = abs(a);

		for (int i = 0; i < n; i++) {
			xl[i] = xl[i] + a;
			xr[i] = xr[i] + a;
			if (xr[i] > c)
				c = xr[i];
		}
	}

	else {
		for (int i = 0; i < n; i++) {
			xl[i] = xl[i] - a;
			xr[i] = xr[i] - a;
			if (xr[i] > c)
				c = xr[i];
		}
	}

	if (b < 0) {
		b = abs(b);

		for (int i = 0; i < n; i++) {
			yb[i] = yb[i] + b;
			yt[i] = yt[i] + b;
			if (yt[i] > d)
				d = yt[i];
		}
	}

	else {
		for (int i = 0; i < n; i++) {
			yb[i] = yb[i] - b;
			yt[i] = yt[i] - b;
			if (yt[i] > d)
				d = yt[i];
		}
	}

	cwin.coord(0 - (0.5*a), (d + (0.1*d)) , (c + (0.1*c)) , 0 - (0.5*b));

	for (int i = 0; i < n - 1; i++) {
		Point LowerLeft(xl[i], yb[i]), UpperRight(xr[i], yt[i]);
		DrawRectangle(LowerLeft, UpperRight);
		Point Name(xr[i], yt[i]);
		Message m(Name, id);
		cwin << m;
		id++;
	}
	return 0;
}