
#include <Windows.h>

#include <sstream>
#include <string>
#include <iostream>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <vector>

#include "mark.h"



using namespace std;
using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//board positions (found from boardcalibration)
double BOARD_SIZE=450;
int boardxorigin = 100;
int boardyorigin = 100;


//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 40 * 40;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;

//board markers
const int MIN_BOARDMARK_SIZE = 8 * 8;
const int MAX_BOARDMARK_SIZE = 40*40;

//random variables
int numObjects;
int numcrosses;
int numnaughts;
//turn variable
bool turn = false;

//Computer and player marker types
string C_mark;
string P_mark;

//setup game board
int board[3][3] = { {0,0,0}, {0,0,0},{0,0,0} };
int boardtemp[3][3];

const string trackbarWindowName = "Trackbars";

//gamestart function
void gamestart()
{
	bool valid = false;
	while (!valid)
	{
		cout << "Would you like to be naughts or crosses (crosses goes first) (x,o)?: \n";
		cin >> P_mark;
		if (P_mark == "x" || P_mark == "o") valid = true;

		else {
			cout << "invalid marker please enter again";
			}
	}

	if (P_mark == "x")
	{
		cout << "Ok I am naughts\n";
		turn=false;
	}

	else
	{
		cout << "Ok I am crosses\n";
		turn = true;
	}

}




//function to check if its computers turn
void turncheck(vector<mark> themarks)
{
	if (themarks.size()%2==0 && (P_mark == "o") || themarks.size() % 2 != 0 && (P_mark == "x"))
	{
		turn = true;
		//cout << "my turn";
	}
	else turn = false;

}



void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed
}

string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}


void createTrackbars() {
	//create window for trackbars
	namedWindow(trackbarWindowName, 0);

	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf_s(TrackbarName, "H_MIN", H_MIN);
	sprintf_s(TrackbarName, "H_MAX", H_MAX);
	sprintf_s(TrackbarName, "S_MIN", S_MIN);
	sprintf_s(TrackbarName, "S_MAX", S_MAX);
	sprintf_s(TrackbarName, "V_MIN", V_MIN);
	sprintf_s(TrackbarName, "V_MAX", V_MAX);

	//create trackbars and insert them into window  
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);

}




//function for morphological operations to refine threshold images
void morphOps(Mat &thresh) {
	//erode away small noise
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(2, 2));
	//dilate remaining objects
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}


	//function to draw crosshairs on tracked images
void drawObject(vector<mark> themarks, Mat &frame) {

	for (int i = 0; i < themarks.size(); i++) {

		cv::circle(frame, cv::Point(themarks.at(i).getxpos(), themarks.at(i).getypos()), 10, cv::Scalar(0, 0, 255));
		cv::putText(frame, intToString(themarks.at(i).getxpos()) + " , " + intToString(themarks.at(i).getypos()), cv::Point(themarks.at(i).getxpos(), 
			themarks.at(i).getypos() + 20), 1, 1, Scalar(0, 255, 0));
		cv::putText(frame, themarks.at(i).gettype(), cv::Point(themarks.at(i).getxpos(), themarks.at(i).getypos() - 30), 1, 2, Scalar(0,255,255));
	}
	waitKey(30);
}




//Function to change Board Parameters/ check that board has changed
void boardchange(vector<mark> themarks) {
	//set temporary board status then wait x second
	for (int j = 0; j <= 2; j++) {
		for (int k = 0; k <= 2; k++) {
			boardtemp[j][k] = board[j][k];
		}
	}

//nested loops to check board positions of each mark
		for (int i = 0; i <themarks.size(); i++) {

			for (int j = 0; j <= 2; j++) {
				for (int k = 0; k <= 2; k++) {

					//if the marks position matches a board position change board state
					if (themarks.at(i).getboardx() == (k + 1) && themarks.at(i).getboardy() == (j + 1))
					{
			
							if (themarks.at(i).gettype() == "cross")
								{
									board[j][k] = 1;
								}
							if (themarks.at(i).gettype() == "naught")
								{
									board[j][k] = 2;
								}

							//change parts of the board that arent occupied to 0's (for a different scenario where all board positions have 
							//small marks on them
							/*if (themarks.at(i).gettype() == "board")
								{
									board[j][k] = 0;
								}*/
					}
				}		
			}	
			//Check board positions for marks, if none of the marks fall onto a board position make the board position 0.
			/*for (int j = 0; j <= 2; j++) {
				for (int k = 0; k <= 2; k++) {
					if (themarks.begin().getboardx(), themarks.end(), board[j][k]) != themarks.end()) {

						board[j][k] = 0;
					}
				}
			}*/
		}

		//check a new frame to see if board has changed, if the board state is the same after 2 loops the program will continue
		for (int j = 0; j <= 2; j++) {
			for (int k = 0; k <= 2; k++) {
				if (board[j][k] != boardtemp[j][k])
					//exit function and return to main loop
					goto stop;
			}
		}

		//Print board results
		for (int j = 0; j <= 2; j++) {
			for (int k = 0; k <= 2; k++) {
				cout << board[j][k];
			}
			cout << "\n";
		}

		cout << "\n";

		//goto marker
	stop:
		//wait 3 seconds before taking new sample
		waitKey(3000);
	}




//Calibration mode tracking function
void trackcalib(Mat threshold, Mat HSV, Mat &cameraFeed) {

	//create temporary image
	Mat temp;
	threshold.copyTo(temp);
	vector <mark> peices;
	
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//these two vectors needed for output of findContours

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find filtered object
	double refArea = 0;
	bool objectFound = false;

	//checking that contours have been found
	if (hierarchy.size() > 0) {
		numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS- noisy filter
		if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//check object is right size
				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA)
				{
					//store each object as a seperate peice
					mark peice;

					//set each peices position
					peice.setxpos(moment.m10 / area);
					peice.setypos(moment.m01 / area);

					peices.push_back(peice);

					objectFound = true;

					refArea = area;
				}

				else objectFound = false;
			}
		}
		//found an object
		if (objectFound == true) {
			putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
			//draw object location on screen
			drawObject(peices, cameraFeed);
		}

		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}

}





//Find Board (couldn't figure out how to do board state variables in a class so just left it for now)
void findboard(Mat threshold, Mat HSV, Mat &cameraFeed) {

	//create temporary image
	Mat temp;
	threshold.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//checking that contours have been found
	if (hierarchy.size() > 0) {

		//find the length of each board side
		BOARD_SIZE=(arcLength(contours[0],false))/4;
		//find the board origin
		Moments moment = moments(contours[0]);
		double area = moment.m00;

		boardxorigin=((moment.m10 / area) - (BOARD_SIZE / 2));
		boardyorigin=((moment.m01 / area) - (BOARD_SIZE / 2));
		
		//print board settings
		cout << "board x origin="<< boardxorigin<<"\n"<< "board y origin=" << boardyorigin << "\n" 
			<<"boardsize="<<BOARD_SIZE << "\n";
		
		waitKey(2000);
	}
}





//track naughts and crosses
void track(mark themarks, Mat threshold, Mat HSV, Mat &cameraFeed) {
	//create temporary image
	Mat temp;
	threshold.copyTo(temp);
	
	//peices and boardstate vectors
	vector <mark> peices;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//these two vectors needed for output of findContours

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find filtered object
	double refArea = 0;
	bool objectFound = false;

	//checking that contours have been found
	if (hierarchy.size() > 0) {
		numObjects = hierarchy.size();

			//if number of objects greater than MAX_NUM_OBJECTS- noisy filter
			if (numObjects < MAX_NUM_OBJECTS) {
				for (int index = 0; index >= 0; index = hierarchy[index][0]) {

					Moments moment = moments((cv::Mat)contours[index]);
					double area = moment.m00;

					//check object is right size
					if (area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA)
					{
						//store each object as a seperate peice
						mark peice;

						//set each peices position
						peice.setxpos(moment.m10 / area);
						peice.setypos(moment.m01 / area);


						//set peices gameboard positions (3x3 board) (using found board size details)
						for (int j = 1; j <= 3; j++) {
							if (peice.getxpos() >= (boardxorigin + ((BOARD_SIZE * (j - 1)) / 3))) {
								peice.setboardx(j);
							}
							if (peice.getypos() >= (boardxorigin + ((BOARD_SIZE * (j - 1)) / 3))) {
								peice.setboardy(j);
							}
						}

						//set peice type (x or o)
						peice.settype(themarks.gettype());

						//move onto next peice
						peices.push_back(peice);
						objectFound = true;

						refArea = area;
					}

					else objectFound = false;

					//check for smaller objects (board positions)

					if (area > MIN_BOARDMARK_SIZE && area < MAX_BOARDMARK_SIZE)
					{
						//store each object as a seperate board position
						mark peice;;

						//set each peices position
						peice.setxpos(moment.m10 / area);
						peice.setypos(moment.m10 / area);

						//set peices gameboard positions (3x3 board)
						for (int j = 1; j <= 3; j++) {
							if (peice.getxpos() >= ((FRAME_WIDTH * (j - 1)) / 3)) {
								peice.setboardx(j);
							}
						}

						for (int j = 1; j <= 3; j++) {
							if (peice.getypos() >= ((FRAME_HEIGHT * (j - 1)) / 3)) {
								peice.setboardy(j);
							}
						}
						peice.settype(themarks.gettype());
						peices.push_back(peice);
					}
				}
			}
			//check to see if its computers turn
			turncheck(peices);

			//if an object is found
			if (objectFound) {
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject(peices, cameraFeed);

				//Change Board State
				boardchange(peices);

				//if its the computers turn
				if (turncheck) {
					//execute tic tac toe logic to decide on a move
				}


			}

			if (numObjects > MAX_NUM_OBJECTS)
			{
				putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
			}
	}

}




int main(int argc, char* argv[])
{
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold crosses image
	Mat threshold;
	
	//matrix storage for binary threshold crosses image
	Mat threshold2;

	Mat threshold3;

	//board threshmatrix
	Mat threshold4;

	//gamestart function
	gamestart();

	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	
	//calibration variables
	bool colorcalibration = false;
	bool boardcalibration = false;

	if (colorcalibration) {
		//create slider bars for HSV filtering
		createTrackbars();
	}

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1) {
		//store image to matrix
		capture.read(cameraFeed);
		
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		//set calibration mode to find min and max hsv values

		//mark setups (boardpos is for case where all board positions are signified by a small dot)
		mark cross("cross"), naught("naught"), boardpos("board");


		if (colorcalibration)
		{	
			//trackbars for adjusting and finding right values (edit values in mark.cpp)
			inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
			morphOps(threshold);
			trackcalib(threshold, HSV, cameraFeed);

			imshow("crosses", threshold);
			imshow("OG", cameraFeed);
		}


		//Track Gameboard and set board size/position
		if (boardcalibration)
		{

			cout << "Commencing Board Calibration, press Space when board position is correct " << endl;
			inRange(HSV, naught.getHSVmin(), naught.getHSVmax(), threshold4);
			morphOps(threshold4);
			findboard(threshold4, HSV, cameraFeed);
			imshow("Boardstate", threshold4);
		}


		if (!boardcalibration && !colorcalibration)
		{

				//track valid board positions (color matches naught color)
				inRange(HSV, cross.getHSVmin(), cross.getHSVmax(), threshold3);
				morphOps(threshold3);
				track(boardpos, threshold3, HSV, cameraFeed);

			//track crosses
				inRange(HSV, cross.getHSVmin(), cross.getHSVmax(), threshold);
				morphOps(threshold);
				track(cross, threshold, HSV, cameraFeed);


			//then naughts
				inRange(HSV,naught.getHSVmin(), naught.getHSVmax(), threshold2);
				morphOps(threshold2);
				track(naught, threshold2, HSV, cameraFeed);

				//show frames 
				//imshow("Gameboard", Boardpic);
				imshow("crosses", threshold);
				imshow("naughts", threshold2);
				imshow("Boardstate", threshold3);
				imshow("OG", cameraFeed);
		}
		//wait 30ms to refresh screen
		waitKey(30);
	}

	return 0;
}
