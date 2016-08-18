#pragma once
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include "actuator.h"
#include "perceptor.h"
#include "state.h"
#include "strategy.h"

using namespace cv;

// the Agent object will actually play the game
// by decoupling acting/making moves from perception, we can call them in different loops.
// for instance we might act every 500ms while we perceive every 30ms.
class Agent
{
public:
	Agent(Actuator actuator, Perceptor perceptor, Strategy strategy);
	~Agent();

	// perceiving updates our state with info from the environment
	void perceive(const Mat &vision);

	// acting uses our state to make actions
	void act();

private:
	State state;
	Actuator actuator;
	Perceptor perceptor;
};

