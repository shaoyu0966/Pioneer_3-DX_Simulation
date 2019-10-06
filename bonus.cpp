# include "Aria.h"
# include <iostream>
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
	ArFunctorC<MyRobot> safe_forward;
	ArFunctorC<MyRobot> safe_backward;

public:
	MyRobot(int* argc, char** argv):
				connector(argc, argv),
				leftRotate(robot, &ArRobot::setDeltaHeading, DELTA_HEADING),
				rightRotate(robot, &ArRobot::setDeltaHeading, -DELTA_HEADING),
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

    double safeRange() {
        return (getSonarReading(-30, 30) - 2 * robot.getRobotRadius());
    }
    void atomicMove(double distance) {
        robot.lock();
        robot.move(distance);
        robot.unlock();
        while(!robot.isMoveDone()) ;
    }
    void atomicRotate(double deg) {
        robot.lock();
        robot.setHeading(deg);
        robot.unlock();
        while(!robot.isHeadingDone()) ;
    }

    void moveTo(double x, double y, double rad) {

		ArPose target(x, y, ArMath::radToDeg(rad));
        ArPose recent_pose = getPose();
        bool no_update_recently = false;
        double front_range = 0;
        double move_angle = 0;
        double move_distance = 0;
        
        for(int i = 0; i >= 0; i++){
			// define target
            move_angle = robot.getPose().findAngleTo(target);
            move_distance = robot.findDistanceTo(target);
			no_update_recently = false;
            if(!(i % 3)){
                no_update_recently = robot.findDistanceTo(recent_pose) < move_distance * 0.1;
                recent_pose = getPose();
            }
			
			// attempt to reach the target directly
            atomicRotate(move_angle);
            front_range = getSonarReading(-30, 30);
            if(front_range >= move_distance) {
                atomicMove(move_distance);
                atomicRotate(ArMath::radToDeg(rad));
                break;
            }
			
			// offset required, move-turn-move
            atomicMove(safeRange() * 0.75);
            atomicRotate(robot.getTh() + (no_update_recently? 120 : 60) * (getSonarReading(15, 105) > getSonarReading(-105, -15)? 1 : (-1)));
            atomicMove(safeRange() * 0.5);
        }

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
	
	robot.moveTo(target_x, target_y, target_th);

	Aria::shutdown();
	Aria::exit(0);
}
