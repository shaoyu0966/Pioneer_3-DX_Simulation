# include "Aria.h"
# include <iostream>
# include <iomanip>
# include <string>
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
    // ArFunctor1C<ArRobot, double> forward;
	// ArFunctor1C<ArRobot, double> backward;
	ArFunctorC<MyRobot> safe_forward;
	ArFunctorC<MyRobot> safe_backward;

public:
	MyRobot(int* argc, char** argv):
				connector(argc, argv),
				leftRotate(robot, &ArRobot::setDeltaHeading, DELTA_HEADING),
				rightRotate(robot, &ArRobot::setDeltaHeading, -DELTA_HEADING),
				// forward(robot, &ArRobot::move, MOVE_STEP),
				// backward(robot, &ArRobot::move, -MOVE_STEP),
				safe_forward(*this, &MyRobot::safeForward),
				safe_backward(*this, &MyRobot::safeBackward)
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
		robot.lock();
		Aria::setKeyHandler(&keyHandler);
		robot.attachKeyHandler(&keyHandler);
		keyHandler.addKeyHandler(ArKeyHandler::LEFT, &leftRotate);
		keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &rightRotate);
		// keyHandler.addKeyHandler(ArKeyHandler::UP, &forward);
		// keyHandler.addKeyHandler(ArKeyHandler::DOWN, &backward);
		keyHandler.addKeyHandler(ArKeyHandler::UP, &safe_forward);
		keyHandler.addKeyHandler(ArKeyHandler::DOWN, &safe_backward);
		robot.unlock();
	}

	void printControlMsg() {
		printf("You may press escape to exit.\n");
		printf("Press LEFT to left-rotate.\n");
		printf("Press RIGHT to left-rotate.\n");
		printf("Press UP to move forward.\n");
		printf("Press DOWN to move backward.\n");	
	}

	double getX() { return robot.getX(); }
	double getY() { return robot.getY(); }
	double getTh() { return robot.getTh(); }
	double getVel() { return robot.getVel(); }
	double getRotVel() { return robot.getRotVel(); }
    double getSonarReading(int start_angle, int end_angle) { return sonar.currentReadingPolar(start_angle, end_angle); }
    ArPose getPose() { return robot.getPose(); }
	
	void safeForward() {
        robot.lock();
		double range = sonar.currentReadingPolar(-45, 45);
		robot.move(range >= 5 * MOVE_STEP? MOVE_STEP : range * 0.2);
        robot.unlock();
	}
	void safeBackward() {
        robot.lock();
		double range = sonar.currentReadingPolar(135, 225);
		robot.move(range >= 6 * MOVE_STEP? -MOVE_STEP : range * (-0.1));
        robot.unlock();
	}

    void moveTo(double x, double y, double rad) {
		std::cout << "angle in degree: " << ArMath::radToDeg(rad) << std::endl;
        ArPose target(x, y, ArMath::radToDeg(rad));
        double move_angle = robot.getPose().findAngleTo(target);
        double move_distance = robot.findDistanceTo(target);
        std::cout << "move angle: " << move_angle << std::endl;
        std::cout << "move distance: " << move_distance << std::endl;
        
        robot.lock();
        robot.setHeading(move_angle);
        robot.unlock();
		
		while(!robot.isHeadingDone()) ;
		robot.lock();
		robot.move(move_distance);
		robot.unlock();

		while(!robot.isMoveDone()) ;
		robot.lock();
		robot.setHeading(ArMath::radToDeg(rad));
		robot.unlock();

		while(!robot.isHeadingDone()) ;
    }
};

/* ------------------------------ MAIN ------------------------------ */

int main(int argc, char **argv)
{	
    double target_x = 0, target_y = 0, target_th = 0;
	std::cout << "Please type your target X, Y, and Theta(in radian):" << std::endl;
	std::cin >> target_x >> target_y >> target_th;

	Aria::init();
	MyRobot robot(&argc, argv);
	robot.addKeyControl();
	// robot.printControlMsg();
	
	robot.moveTo(target_x, target_y, target_th);

	// for(int i = 0; i >= 0; i++){

	// 	if(!(i % 10)){
	// 		std::cout << std::string(54, '-') << std::endl;
	// 		printf("%10s %10s %10s %10s %10s\n", "X", "Y", "Theta", "Vel", "RotVel");
	// 	}

	// 	printf("%10.2f %10.2f %10.2f %10.2f %10.2f\n", robot.getX(), robot.getY(), robot.getTh(), robot.getVel(), robot.getRotVel());
	// 	ArUtil::sleep(300);
	// }

	Aria::shutdown();
	Aria::exit(0);
}
