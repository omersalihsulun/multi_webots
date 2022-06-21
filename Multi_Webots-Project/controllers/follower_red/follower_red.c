#include <stdio.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define SPEED 4
#define TIME_STEP 64

int main() {
  WbDeviceTag camera, left_motor, right_motor;
  int i;
  wb_robot_init();
  
  camera = wb_robot_get_device("mod_camera");
  wb_camera_enable(camera, TIME_STEP);
  wb_camera_recognition_enable(camera, TIME_STEP);

    
  while (wb_robot_step(TIME_STEP) != -1) {
    int number_of_objects = wb_camera_recognition_get_number_of_objects(camera);
    //printf("\nRecognized %d objects.\n", number_of_objects);
    
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);
    for (i = 0; i < number_of_objects; ++i) {
    int x, y, height, width;
    int area;
    int color_red,color_green,color_blue;
    x = objects[i].position_on_image[0];
    y = objects[i].position_on_image[1];
    height = objects[i].size_on_image[0];
    width = objects[i].size_on_image[1];
    color_red = objects[i].colors[0];
    color_green = objects[i].colors[1];
    color_blue = objects[i].colors[2];
    area = height * width;
    WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
    WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);

 int number_of_objects = wb_camera_recognition_get_number_of_objects(camera);
 //printf("\nRecognized %d objects.\n", number_of_objects);
   
   
  if (x < 362 && x >= 1 && color_red == 1){
    wb_motor_set_velocity(left_motor, -SPEED);
    wb_motor_set_velocity(right_motor, SPEED);
    }
  else if (x > 662 && x <= 1024 && color_red == 1){
    wb_motor_set_velocity(left_motor, SPEED);
    wb_motor_set_velocity(right_motor, -SPEED);
    }
  else if (area > 362404 && area <= 10000000 && color_red == 1){
    wb_motor_set_velocity(left_motor, -SPEED);
    wb_motor_set_velocity(right_motor, -SPEED);
    } 
  else if (area < 362404 && area >= 124 && color_red == 1){
    wb_motor_set_velocity(left_motor, SPEED);
    wb_motor_set_velocity(right_motor, SPEED);
    }     
     // printf("Xcenter, Ycenter = %d , %d\n", x, y);
     // printf("height, width = %d , %d\n", height, width);
      printf("area = %d\n", area);
    }
    
  }

  wb_robot_cleanup();

  return 0;
}