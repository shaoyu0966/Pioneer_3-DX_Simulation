# include "Aria.h"
# include <iostream>

# define DELTA_HEADING 20
# define MOVE_STEP 600

class ControlHandler
{
private:
	ArKeyHandler keyHandler;
	ArFunctor1C<ArRobot, double> leftRotate;
	ArFunctor1C<ArRobot, double> rightRotate;
    ArFunctor1C<ArRobot, double> forward;
	ArFunctor1C<ArRobot, double> backward;
	ArFunctorC<ArRobot> stop;
public:
	ControlHandler(ArRobot& robot): leftRotate(robot, &ArRobot::setDeltaHeading, DELTA_HEADING),
									rightRotate(robot, &ArRobot::setDeltaHeading, -DELTA_HEADING),
									forward(robot, &ArRobot::move, MOVE_STEP),
									backward(robot, &ArRobot::move, -MOVE_STEP) {}
	void addKeyControl(ArRobot& robot) {
		Aria::setKeyHandler(&keyHandler);
		robot.attachKeyHandler(&keyHandler);
		keyHandler.addKeyHandler(ArKeyHandler::LEFT, &leftRotate);
		keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &rightRotate);
		keyHandler.addKeyHandler(ArKeyHandler::UP, &forward);
		keyHandler.addKeyHandler(ArKeyHandler::DOWN, &backward);
		keyHandler.addKeyHandler(ArKeyHandler::SPACE, &stop);
	}
	void printControlMsg() {
		printf("You may press escape to exit.\n");
		printf("Press LEFT to left-rotate.\n");
		printf("Press RIGHT to left-rotate.\n");
		printf("Press UP to move forward.\n");
		printf("Press DOWN to move backward.\n");	
	}
};

int main(int argc, char **argv)
{
	ArRobot robot;
	ArSonarDevice sonar;
	ArSimpleConnector connector(&argc,argv);

	robot.addRangeDevice(&sonar);
	Aria::init();
	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}
	robot.comInt(ArCommands::ENABLE, 1);
	robot.runAsync(false);

	// TODO: control the robot
	robot.lock();
	ControlHandler control_handler(robot);
	control_handler.addKeyControl(robot);
	control_handler.printControlMsg();	
	robot.unlock();

	while(true){
		printf("%f %f %f %f %f\n", robot.getX(), robot.getY(), robot.getTh(), robot.getVel(), robot.getRotVel());
		ArUtil::sleep(300);
	}

	Aria::shutdown();
	Aria::exit(0);
}
