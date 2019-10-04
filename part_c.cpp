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

    // Used to perform actions when keyboard keys are pressed
    ArKeyHandler keyHandler;

	// TODO: control the robot
	robot.lock(); 
	// robot.setVel(150);
    ArFunctor1C<ArRobot, double> leftRotate(&robot, &ArRobot::setRotVel, 30);
    keyHandler.addKeyHandler(ArKeyHandler::LEFT, &leftRotate);
    ArFunctor1C<ArRobot, double> rightRotate(&robot, &ArRobot::setRotVel, -30);
    keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &rightRotate);
    
    Aria::setKeyHandler(&keyHandler);
    robot.attachKeyHandler(&keyHandler);
    printf("You may press escape to exit\n");

	robot.unlock();

	while(true){
		printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		ArUtil::sleep(300);
	}

	Aria::shutdown();
	Aria::exit(0);
}
