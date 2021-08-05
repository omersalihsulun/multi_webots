#include <webots/accelerometer.h>
#include <webots/camera.h>
#include <webots/keyboard.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/utils/ansi_codes.h>

#define MAX_SPEED 4
#define TIME_STEP 64
#define NB_LEDS 8

WbDeviceTag led[NB_LEDS];
WbDeviceTag cam;
WbDeviceTag accelerometer;
WbDeviceTag left_motor, right_motor;

double left_speed = 0;
double right_speed = 0;

WbDeviceTag ps[8];
  char ps_names[8][4] = {
    "ps0", "ps1", "ps2", "ps3",
    "ps4", "ps5", "ps6", "ps7"
  };


static void reset(void) {
  
  
  printf("Yon tuslari ile robotu yonlendirebilirsiniz.");
  printf("A tusuna basarak otomatik suruse gecebilirsiniz.");
  int i=0;
  char text[5] = "led0";
  for (i = 0; i < NB_LEDS; i++) {
    led[i] = wb_robot_get_device(text); 
    text[3]++;
  }


  accelerometer = wb_robot_get_device("accelerometer");
  wb_accelerometer_enable(accelerometer, TIME_STEP);

  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  wb_keyboard_enable(TIME_STEP);
  
  for (i = 0; i < NB_LEDS; i++)
    wb_led_set(led[i], 0);
}

static void automated(void){
  printf("X tusuna basarak manuel moda donebilirsiniz");
  int i = 0;
  for (i = 0; i < 8 ; i++) {
    ps[i] = wb_robot_get_device(ps_names[i]);
    wb_distance_sensor_enable(ps[i], TIME_STEP);
  } 
  
 while (wb_robot_step(TIME_STEP) != -1) {
    
    double ps_values[8];
    for (i = 0; i < 8 ; i++)
      ps_values[i] = wb_distance_sensor_get_value(ps[i]);

    
    bool right_obstacle =
      ps_values[0] > 80.0 ||
      ps_values[1] > 80.0 ||
      ps_values[2] > 80.0;
    bool left_obstacle =
      ps_values[5] > 80.0 ||
      ps_values[6] > 80.0 ||
      ps_values[7] > 80.0;

   
    double left_speed  = 1 * MAX_SPEED;
    double right_speed = 1 * MAX_SPEED;

  
    if (left_obstacle) {

      left_speed  = 1 * MAX_SPEED;
      right_speed = -1 * MAX_SPEED;
    }
    else if (right_obstacle) {
    
      left_speed  = -1 * MAX_SPEED;
      right_speed = 1 * MAX_SPEED;
    }

   
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
    
    int key = wb_keyboard_get_key();
    if (key == 'X')
    break;
  }


}

static void run(void) {
  int key = wb_keyboard_get_key();


  switch (key) {
    case WB_KEYBOARD_UP:
      if (left_speed < MAX_SPEED)
        left_speed += 1;
      if (right_speed < MAX_SPEED)
        right_speed += 1;
      break;
    case WB_KEYBOARD_DOWN:
      if (left_speed > -MAX_SPEED)
        left_speed -= 1;
      if (right_speed > -MAX_SPEED)
        right_speed -= 1;
      break;
    case WB_KEYBOARD_LEFT:
      if (right_speed < MAX_SPEED)
        right_speed += 1;
      if (left_speed > -MAX_SPEED)
        left_speed -= 1;
      break;
    case WB_KEYBOARD_RIGHT:
      if (left_speed < MAX_SPEED)
        left_speed += 1;
      if (right_speed  > -MAX_SPEED)
        right_speed  -= 1;
      break;
    case 'A':
      automated();
      break;
    case 'R':
      left_speed = right_speed  = 0;
      break;
    default:
      break;
  }

   if (right_speed  < 0 && left_speed < 0)
    wb_led_set(led[4], 1);
  else
    wb_led_set(led[4], 0);

  wb_motor_set_velocity(left_motor, left_speed );
  wb_motor_set_velocity(right_motor,right_speed );

  return;
}

int main() {
  wb_robot_init(); 
  
  
  reset();
  while (wb_robot_step(TIME_STEP) != -1){
    run();
}
  wb_robot_cleanup();

  return 0;
}
