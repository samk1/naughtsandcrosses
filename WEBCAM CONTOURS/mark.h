#pragma once
#include <string>
#include <opencv\highgui.h>
#include <opencv\cv.h>
using namespace std;
using namespace cv;
class mark
{
public:
	mark(void);
	~mark(void);

	mark(string name);

	//setting up positional variables for each object
	int getxpos();
	void setxpos(int x);

	int getypos();
	void setypos(int y);

	//gameboard variables
	int getboardx();
	void setboardx(int b);

	int getboardy();
	void setboardy(int c);

	//marker type(x or y)
	int getmarker() {
		return marker;
	}

	void setmarker(int m) { marker = m; }

	Scalar getHSVmin();
	Scalar getHSVmax();

	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

	string gettype() {
		return type;
	}

	void settype(string t) { type = t; }

private:

	int xpos, ypos, boardx, boardy, marker;
	string type;

	Scalar HSVmin, HSVmax;
	Scalar Color;
};

