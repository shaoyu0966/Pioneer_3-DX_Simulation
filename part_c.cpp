# include "Aria.h"
# include <iostream>
# define DELTA_HEADING 20
# define MOVE_STEP 600

/* ------------------------------ CONTROL_HANDLER ------------------------------ */

class MyRobot
{
private:
	ArRobot robot;
	ArSonarDevice sonar;
	ArSimpleConnector connector;
	ArKeyHandler keyHandler;
	ArFunctor1C<ArRobot, double> leftRotate;
	ArFunctor1C<ArRobot, double> rightRotate;
    ArFunctor1C<ArRobot, double> forward;
	ArFunctor1C<ArRobot, double> backward;

public:
	MyRobot():	leftRotate(robot, &ArRobot::setDeltaHeading, DELTA_HEADING),
				rightRotate(robot, &ArRobot::setDeltaHeading, -DELTA_HEADING),
				forward(robot, &ArRobot::move, MOVE_STEP),
				backward(robot, &ArRobot::move, -MOVE_STEP)
	{
		robot.addRangeDevice(&sonar);
		if (!connector.connectRobot(&robot)){
			printf("Could not connect to robot... exiting\n");
			Aria::shutdown();
			Aria::exit(1);
		}
		robot.comInt(ArCommands::ENABLE, 1);
		robot.runAsync(false);
	}

	void addKeyControl() {
		robot.lock()
		Aria::setKeyHandler(&keyHandler);
		robot.attachKeyHandler(&keyHandler);
		keyHandler.addKeyHandler(ArKeyHandler::LEFT, &leftRotate);
		keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &rightRotate);
		keyHandler.addKeyHandler(ArKeyHandler::UP, &forward);
		keyHandler.addKeyHandler(ArKeyHandler::DOWN, &backward);
		robot.unlock();
	}

	void printControlMsg() {
		printf("You may press escape to exit.\n");
		printf("Press LEFT to left-rotate.\n");
		printf("Press RIGHT to left-rotate.\n");
		printf("Press UP to move forward.\n");
		printf("Press DOWN to move backward.\n");	
	}

	void getX() { return robot.getX(); }
	void getY() { return robot.getY(); }
	void getTh() { return robot.getTh(); }
	void getVel() { return robot.getVel(); }
	void getRotVel() { return robot.getRotVel(); }
};

/* ------------------------------ MAIN ------------------------------ */

int main(int argc, char **argv)
{
	Aria::init();
	
	MyRobot robot();
	robot.addKeyControl();
	robot.printControlMsg();

	while(true){
		printf("%f %f %f %f %f\n", robot.getX(), robot.getY(), robot.getTh(), robot.getVel(), robot.getRotVel());
		// printf("%f %f %f\n", sonar.currentReadingPolar(0, 180), sonar.currentReadingPolar(-180, 0), sonar.currentReadingPolar(-45, 45));
		ArUtil::sleep(300);
	}

	Aria::shutdown();
	Aria::exit(0);
}
