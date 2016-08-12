#include "mark.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
using namespace std;
using namespace cv;

mark::mark(void)
{
	settype("null");
}

mark::mark(string name){

	settype(name);

	if (name == "cross")
	{

		setHSVmin(Scalar(25, 91, 65));
		setHSVmax(Scalar(67, 221, 254));

	}
	if (name == "naught")
	{


		setHSVmin(Scalar(91, 123, 93));
		setHSVmax(Scalar(146, 256, 256));
	}

}

mark::~mark(void)
{
}

int mark::getxpos() {

	return mark::xpos;
}

void mark::setxpos(int x) {
	mark::xpos = x;
}

int mark::getypos() {

	return mark::ypos;

}
void mark::setypos(int y) {
	mark::ypos = y;
}

int mark::getboardx() {
	return mark::boardx;
}
void mark::setboardx(int b) {
	mark::boardx = b;
}

int mark::getboardy() {
	return mark::boardy;
}
void mark::setboardy(int c) {
	mark::boardy = c;
}



Scalar mark::getHSVmin() {
	return mark::HSVmin;

}

Scalar mark::getHSVmax() {
	return mark::HSVmax;

}
void mark::setHSVmin(Scalar min){
	mark::HSVmin = min;
}

void mark::setHSVmax(Scalar max) {
	mark::HSVmax = max;
}
