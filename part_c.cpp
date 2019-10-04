# include "Aria.h"
# include <iostream>

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

    ArKeyHandler keyHandler;

	// TODO: control the robot
	robot.lock(); 
	ArFunctor1C<ArRobot, double> leftRotate(&robot, &ArRobot::setDeltaHeading, 30), rightRotate(&robot, &ArRobot::setDeltaHeading, -30);
    ArFunctor1C<ArRobot, double> forward(&robot, &ArRobot::setVel, 200), backward(&robot, &ArRobot::setVel, -200);
	ArFunctorC<ArRobot> stop(&robot, &ArRobot::stop);
	keyHandler.addKeyHandler(ArKeyHandler::LEFT, &leftRotate);
    keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &rightRotate);
	keyHandler.addKeyHandler(ArKeyHandler::UP, &forward);
	keyHandler.addKeyHandler(ArKeyHandler::DOWN, &backward);
	keyHandler.addKeyHandler(ArKeyHandler::SPACE, &stop);
		
    
    Aria::setKeyHandler(&keyHandler);
    robot.attachKeyHandler(&keyHandler);
    printf("You may press escape to exit\n");

	robot.unlock();

	while(true){
		printf("%f %f %f %f\n", robot.getX(), robot.getY(), robot.getTh(), robot.getRotVel());
		ArUtil::sleep(300);
	}

	Aria::shutdown();
	Aria::exit(0);
}
