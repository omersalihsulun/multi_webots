#include <webots/robot.h>

// Added a new include file
#include <webots/motor.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
 wb_robot_init();

 // get the motor devices
 WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
 WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
 // set the target position of the motors
 wb_motor_set_position(left_motor, 10.0);
 wb_motor_set_position(right_motor, 10.0);

 while (wb_robot_step(TIME_STEP) != -1);

 wb_robot_cleanup();

 return 0;
}