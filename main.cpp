/****************************************************************************
*Filename:    Source.c
*Author:      Shehab Hosny
*Date:	      10/31/2019
*Version:     1.0
*Description: D2S Internship Project
****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <time.h>

using namespace std;

//Create a Class for the Rectangle Shape
class Rectangle {
public:

	//Declare a Vector to store the IDs of the Rectangles intersecting this rectangle
	vector<long> Pair;

	//Constructor to Store the Coordinates of the Rectangle
	Rectangle(long long xl, long long yb, long long xr, long long yt) {
		this->id = Count++;
		this->xl = xl;
		this->xr = xr;
		this->yb = yb;
		this->yt = yt;
		this->ignore = 0;
	}

	//Set an ID for this Rectangle
	void Set_ID(long id) {
		this->id = id;
	}

	//Set the Ignore Flag of this Rectangle (To drop it out the Rectangles list)
	void SetIgnore(bool b) {
		ignore = b;
	}

	//Get the status of the Ignore Flag of this Rectangle
	bool GetIgnore() {
		return ignore;
	}

	//Get the Rectangle ID
	long GetID() {
		return id;
	}

	//Get the Rectangle Left x-coordinate
	long long Get_xl() {
		return xl;
	}

	//Get the Rectangle Right x-coordinate
	long long Get_xr() {
		return xr;
	}

	//Get the Rectangle Top y-coordinate
	long long Get_yt() {
		return yt;
	}

	//Get the Rectangle Bottom y-coordinate
	long long Get_yb() {
		return yb;
	}

	//Operator Overloading to sort a list of Rectangles in ascending order of Bottom y-coordinates
	bool operator < (const Rectangle &Obj) const {
		return (this->yb < Obj.yb);
	}

private:
	//Declare Private Properties
	long id;
	static long Count;
	long long xl, xr, yt, yb;
	bool ignore;
};

//Create a Class for the Vertical Edges in the Rectangle
class Vertical_Edge {
public:

	//Constructor to Store the Rectangle ID, the x-coordinate and the side (Left or Right) of this Vertical Edge
	Vertical_Edge(Rectangle R, long long x) {
		this->id = R.GetID();

		if (x == R.Get_xl()) {
			this->x = R.Get_xl();
			this->side = 'L';
		}

		else if (x == R.Get_xr()) {
			this->x = R.Get_xr();
			this->side = 'R';
		}
	}

	//Get the x-coordinate of the Vertical Edge
	long long Get_x() {
		return x;
	}

	//Get the Rectangle ID corresponding to this Vertical Edge
	long Get_ID() {
		return id;
	}

	//Get the Side (Left or Right) of the Vertical Edge
	char Get_Side() {
		return side;
	}

	//Operator Overloading to sort a list of Vertical Edges in ascending order of x-coordinates
	bool operator < (const Vertical_Edge &Obj) const {
		return (this->x < Obj.x);
	}

private:
	//Declare Private Properties
	char side;
	long id;
	long long x;
};

//Global variables and vectors
long Rectangle::Count = 1;
vector<long> Pair;
vector<vector<long>> Intersect_Pairs;
vector<vector<long>> Intersect_Pairs_Sorted;

//Functions Prototypes
void Record(vector<Rectangle> &Rectangles_Set, vector<Vertical_Edge> &Vertical_Edges_Set);
void Clear_Vectors(vector<Rectangle> &Rectangles_Set, vector<Vertical_Edge> &Vertical_Edges_Set, vector<Rectangle> &Next_Rectangles_Set);
void Divide_Conquer(vector<Vertical_Edge> &Vertical_Edges_Set, vector<Rectangle> &Rectangles_Set, long start, long end);
void Stab(vector<Rectangle> &A, vector<Rectangle> &B);
void Get_Ready(vector<vector<long>> &A);
void Non_Overlapping_Set(vector<Rectangle> &Rectangles_Set, vector<Rectangle> &Next_Rectangles_Set);
int No_Of_Intersections(Rectangle R, vector<Rectangle> &A);

int main() {

	long long a, b, c, d;								//Variables to temporarily store the Rectangles' coordinates
	int No_Of_Files = 1;								//Variable to store the number of output files
	int No_Of_Rect = 0;									//Variable to store the Number of Rectangles for later use
	clock_t time_req;									//Variable to store the time taken
	stringstream ss;									//Variable to ad integer to a string
	vector<Rectangle> Rectangles_Set;					//Vector to store the set of rectangles in the readed file 
	vector<Vertical_Edge> Vertical_Edges_Set;			//Vector to store the set of vertical edges of all rectangles
	vector<Rectangle> Next_Rectangles_Set;				//Vector to store the Rectangles to be moved to the next file

	char File_Name[300];								//String of the input file path
	ifstream Dataset;
	cout << "Enter the File Path: [Note: Use this Format => C:/Users/Desktop/data_set_1.txt]" << endl;
	cin.getline(File_Name, 300);
	Dataset.open(File_Name);

	if (!Dataset.is_open())								//Abort the program in case the input file path is invalid
		exit(EXIT_FAILURE);

	time_req = clock();									//Start the Timing interval to calculate the Runtime of the Program Algorithm

	while (Dataset.good()) {						//Input the Coordinates of each ractange and push the rectangle to the vector
		Dataset >> a;
		Dataset >> b;
		Dataset >> c;
		Dataset >> d;
		Rectangles_Set.push_back(Rectangle(a, b, c, d));
	}

	if (Rectangles_Set.size() == 0) {
		cout << "The input file is empty" << endl;
		return 0;
	}

	Rectangles_Set.pop_back();						//To Remove the Last Blank Line

	cout << "Number of Input Rectangles = " << Rectangles_Set.size() << endl;

	do {											////Start exploring the First Non-Overlapping Group

		Clear_Vectors(Rectangles_Set, Vertical_Edges_Set, Next_Rectangles_Set);		//Clear all the related vectors for later use

		Record(Rectangles_Set, Vertical_Edges_Set);									//Call the record function to fill the vector of Intersected Pairs
		Get_Ready(Intersect_Pairs);													//Remove Redundant Pairs from the Intersect_Pairs vector								

		Non_Overlapping_Set(Rectangles_Set, Next_Rectangles_Set);					//Get the Maximum Set of Independent Rectangles

		ofstream MyFile;
		ss << "group_" << No_Of_Files << ".txt";
		No_Of_Files++;
		MyFile.open(ss.str());
		ss.str("");

		for (int i = 0; i < Rectangles_Set.size(); i++) {
			if (Rectangles_Set[i].GetIgnore() == 0)
				MyFile << Rectangles_Set[i].Get_xl() << " " << Rectangles_Set[i].Get_yb() << " " << Rectangles_Set[i].Get_xr() << " " << Rectangles_Set[i].Get_yt() << "\n";
		}

		MyFile.close();

		//Clear the Rectangles_Set vector to fill it with rectangles of the next group
		Rectangles_Set.clear();

		//Fill the Rectangles_Set with rectangles of the next group
		int No_Of_Rect = 0;
		for (int i = 0; i < Next_Rectangles_Set.size(); i++) {
			Rectangles_Set.push_back(Next_Rectangles_Set[i]);
			Rectangles_Set[No_Of_Rect].Set_ID(No_Of_Rect + 1);
			No_Of_Rect++;
		}

	} while (Rectangles_Set.size() != 0);

	//End the Timing interval to calculate the Runtime of the Program Algorithm
	time_req = clock() - time_req;
	cout << "Run time taken: " << (float)time_req / CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}

void Record(vector<Rectangle> &Rectangles_Set, vector<Vertical_Edge> &Vertical_Edges_Set) {

	//Loop on the Rectangles_Set to record all the Vertical Edges of the Rectangles
	for (long i = 0; i < Rectangles_Set.size(); i++) {
		Vertical_Edges_Set.push_back(Vertical_Edge(Rectangles_Set[i], Rectangles_Set[i].Get_xl()));
		Vertical_Edges_Set.push_back(Vertical_Edge(Rectangles_Set[i], Rectangles_Set[i].Get_xr()));
	}

	//Sort the Vertical_Edges_Set in ascending order of coordinates
	sort(Vertical_Edges_Set.begin(), Vertical_Edges_Set.end(), less<Vertical_Edge>());

	//Get the size of the Vertical_Edges_Set
	long n = Vertical_Edges_Set.size() - 1;

	//Call the Divide_Conquer function
	Divide_Conquer(Vertical_Edges_Set, Rectangles_Set, 0, n);
}

void Divide_Conquer(vector<Vertical_Edge> &Vertical_Edges_Set, vector<Rectangle> &Rectangles_Set, long start, long end) {

	//Vectors to store Rectangles in each set (Described briefly in the Report)
	vector<Rectangle> S11, S22, S12, S21;

	//Condition to stop recursion
	if (start >= end)
		return;

	//Get the median index
	long m = start + (end - start) / 2;

	//Loop to fill the list S11 and S12
	for (long i = start; i <= m; i++) {

		if (Vertical_Edges_Set[i].Get_Side() == 'R') {
			S11.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);
		}

		else if (Vertical_Edges_Set[i].Get_Side() == 'L') {
			if (Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].Get_xr() > (Vertical_Edges_Set[end].Get_x()))
				S12.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);

			else {
				if ((Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].Get_xr() == (Vertical_Edges_Set[end].Get_x())) &&
					(Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].GetID() > (Vertical_Edges_Set[start].Get_ID())))
					S12.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);
			}
		}
	}

	//Loop to fill the lists S22 and S21
	for (long i = m + 1; i <= end; i++) {

		if (Vertical_Edges_Set[i].Get_Side() == 'L') {
			S22.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);
		}

		else if (Vertical_Edges_Set[i].Get_Side() == 'R') {

			if (Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].Get_xl() < (Vertical_Edges_Set[start].Get_x()))
				S21.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);

			else {
				if ((Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].Get_xl() == (Vertical_Edges_Set[start].Get_x())) &&
					(Rectangles_Set[Vertical_Edges_Set[i].Get_ID() - 1].GetID() < (Vertical_Edges_Set[start].Get_ID()))) {
					S21.push_back(Rectangles_Set[(Vertical_Edges_Set[i].Get_ID()) - 1]);
				}
			}
		}

	}

	//Sort vectors S11, S12, S22 and S21 in ascending order of y-coordiate
	sort(S11.begin(), S11.end(), less<Rectangle>());
	sort(S22.begin(), S22.end(), less<Rectangle>());
	sort(S12.begin(), S12.end(), less<Rectangle>());
	sort(S21.begin(), S21.end(), less<Rectangle>());

	/*Parameters of Sets passed in Stab consist of Rectangles which intersect by their x-coordinates only
	  Stab function is used to detect the rectangles which intersect in their y-coordinates too*/
	Stab(S12, S22);
	Stab(S21, S11);
	Stab(S12, S21);

	//Recursive call of Divide_Conquer function
	Divide_Conquer(Vertical_Edges_Set, Rectangles_Set, start, m);
	Divide_Conquer(Vertical_Edges_Set, Rectangles_Set, m + 1, end);
}

//This function is used to find the rectangles intersecting in the y-coordinate and find the Intersecting Pairs of rectangles
void Stab(vector<Rectangle> &A, vector<Rectangle> &B) {

	int i = 0, j = 0, k;

	while ((i < (A.size())) && (j < B.size())) {

		if (A[i].Get_yb() < B[j].Get_yb()) {
			k = j;

			while ((k < B.size())) {

				if (B[k].Get_yb() < A[i].Get_yt()) {

					Pair.clear();

					if (!((A[i].Get_xl() == B[k].Get_xr()) || (A[i].Get_xr() == B[k].Get_xl()))) {

						if (A[i].GetID() < B[k].GetID()) {
							Pair.push_back(A[i].GetID());
							Pair.push_back(B[k].GetID());
						}

						else {
							Pair.push_back(B[k].GetID());
							Pair.push_back(A[i].GetID());
						}

						Intersect_Pairs.push_back(Pair);
					}
				}
				k++;
			}
			i++;
		}

		else {
			k = i;

			while ((k < A.size())) {

				if (A[k].Get_yb() < B[j].Get_yt()) {

					Pair.clear();

					if (!((A[k].Get_xl() == B[j].Get_xr()) || (A[k].Get_xr() == B[j].Get_xl()))) {

						if (A[k].GetID() < B[j].GetID()) {
							Pair.push_back(A[k].GetID());
							Pair.push_back(B[j].GetID());
						}

						else {
							Pair.push_back(B[j].GetID());
							Pair.push_back(A[k].GetID());
						}

						Intersect_Pairs.push_back(Pair);
					}
				}
				k++;
			}
			j++;
		}
	}
}

void Get_Ready(vector<vector<long>> &A) {

	//Sort the Intersect_Pairs list by their first column
	sort(A.begin(), A.end(), [](const std::vector<long>& a, const std::vector<long>& b)
	{
		return (a[0] < b[0]);
	});

	//Sort the Intersect_Pairs list by their second column
	sort(A.begin(), A.end(), [](const std::vector<long>& a, const std::vector<long>& b)
	{
		if (a[0] == b[0])
			return (a[1] < b[1]);
		return (a[0] < b[0]);
	});

	//Remove redundant intersections
	for (long i = 0; i < A.size(); i++) {

		if ((i + 1) < A.size()) {
			if (!((A[i][0] == A[i + 1][0] && A[i][1] == A[i + 1][1]) || (A[i][0] == A[i][1]))) {
				Intersect_Pairs_Sorted.push_back(Intersect_Pairs[i]);
			}
		}

		else
			Intersect_Pairs_Sorted.push_back(Intersect_Pairs[A.size() - 1]);
	}
}

//This function is used to clear some vectors for later use
void Clear_Vectors(vector<Rectangle> &Rectangles_Set, vector<Vertical_Edge> &Vertical_Edges_Set, vector<Rectangle> &Next_Rectangles_Set) {

	Intersect_Pairs.clear();
	Intersect_Pairs_Sorted.clear();
	Vertical_Edges_Set.clear();
	Next_Rectangles_Set.clear();

	for (int i = 0; i < Rectangles_Set.size(); i++) {
		Rectangles_Set[i].Pair.clear();
		Rectangles_Set[i].SetIgnore(0);
	}
}

void Non_Overlapping_Set(vector<Rectangle> &Rectangles_Set, vector<Rectangle> &Next_Rectangles_Set) {

	long index;

	//Fill the Pairs vector for each intersecting rectangle
	for (int i = 0; i < Intersect_Pairs_Sorted.size(); i++) {
		Rectangles_Set[Intersect_Pairs_Sorted[i][0] - 1].Pair.push_back(Intersect_Pairs_Sorted[i][1]);
	}

	//Loop on the Rectangles_Set to decide which rectangles will be moved to the next group
	for (int i = 0; i < Rectangles_Set.size(); i++) {

		if (i == 0) {
			for (int j = 0; j < Rectangles_Set[i].Pair.size(); j++) {
				index = Rectangles_Set[i].Pair.back() - 1;
				Rectangles_Set[i].Pair.pop_back();
				j--;
				Rectangles_Set[index].SetIgnore(1);
				Next_Rectangles_Set.push_back(Rectangles_Set[index]);
			}
		}

		else if (Rectangles_Set[i].GetIgnore() == 0) {

			for (int j = 0; j < Rectangles_Set[i].Pair.size(); j++) {

				index = Rectangles_Set[i].Pair.back() - 1;
				Rectangles_Set[i].Pair.pop_back();
				j--;

				if (Rectangles_Set[index].GetIgnore() == 0) {

					if (No_Of_Intersections(Rectangles_Set[i], Next_Rectangles_Set) < No_Of_Intersections(Rectangles_Set[index], Next_Rectangles_Set)) {
						Rectangles_Set[i].SetIgnore(1);
						Next_Rectangles_Set.push_back(Rectangles_Set[i]);
						break;
					}
					else {
						Rectangles_Set[index].SetIgnore(1);
						Next_Rectangles_Set.push_back(Rectangles_Set[index]);
					}
				}
			}
		}
	}
}

//This function is used to find the number of Rectangles in Vector A intersecting the Rectangle R
int No_Of_Intersections(Rectangle R, vector<Rectangle> &A) {

	int Intersections = 0;

	for (int i = 0; i < A.size(); i++) {

		if (!((R.Get_xl() >= A[i].Get_xr() || R.Get_xr() <= A[i].Get_xl()) ||
			(R.Get_yb() >= A[i].Get_yt() || R.Get_yt() <= A[i].Get_yb())))
		{
			Intersections++;
		}
	}

	return Intersections;
}