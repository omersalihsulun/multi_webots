#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <webots/robot.h>
#include <webots/differential_wheels.h>
#include <webots/camera.h>
#include <webots/pen.h>
#include <webots/distance_sensor.h>
#include <webots/compass.h>
#include <webots/gps.h>

#define TIMESTEP 64
#define DIST_TOL 0.1
#define ANGLE_TOL 0.1
#define SPEED 6.28

typedef struct Vector{
  float u;
  float v;
} Vector; 

void minus(Vector *v, Vector *v1, Vector *v2) {
  v->u = v1->u - v2->u;
  v->v = v1->v - v2->v;
}

void sum(Vector *v, Vector *v1, Vector *v2) {
  v->u = v1->u + v2->u;
  v->v = v1->v + v2->v;
}

float norm(Vector *v) {
  return sqrt(v->u*v->u + v->v*v->v);
}

void normalize(Vector *v) {
  double n = norm(v);
  v->u /= n;
  v->v /= n;
}

double angle(Vector *v1, Vector *v2) {
  double theta = atan2(v2->v, v2->u) - atan2(v1->v, v1->u);
  if (theta < 0)
    theta += 2*M_PI;
    
  return (theta);
}

void scale(double s, Vector *v1) {
  v1->u *= s;
  v1->v *= s;
}

int main(){


    WbDeviceTag compass, gps;  
    wb_robot_init();
    

    Vector target_loc = {-1.1, 1.9};

    compass = wb_robot_get_device("compass");
    wb_compass_enable(compass,TIMESTEP);
    
    gps = wb_robot_get_device("gps");
    wb_gps_enable(gps,TIMESTEP);
    
    WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
    WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
    
    while (wb_robot_step(TIMESTEP) != -1){

      double left_speed = SPEED;
      double right_speed = SPEED;
      
      

      const double *pos3D = wb_gps_get_values(gps);
      const double *north3D = wb_compass_get_values(compass);
      

      Vector pos = {pos3D[0], pos3D[2]};

      double rad = atan2(north3D[0], north3D[2]) - 1.5708;
      
      if (rad < 0.0)
        rad = rad + 2*M_PI;
      
      Vector heading = {sin(rad), -cos(rad)}; 
       
      Vector target_dir;
      minus(&target_dir, &target_loc, &pos);
      
      float obj_dist = norm(&target_dir);
        
      normalize(&heading);
      normalize(&target_dir);
      
      double beta = angle(&heading, &target_dir);
      
     
      if (beta < -ANGLE_TOL){
        left_speed = SPEED;
        right_speed = -SPEED;
      }
      else if (beta > ANGLE_TOL){
        left_speed = -SPEED;
        right_speed = SPEED;
      }
      else{
        left_speed = SPEED - M_PI + beta;
        right_speed = SPEED - M_PI - beta;
      }

      if (obj_dist < DIST_TOL){
        printf("Hedefe ulasildi \n");
        left_speed = 0;
        right_speed = 0;
      }  

      wb_motor_set_velocity(left_motor, left_speed);
      wb_motor_set_velocity(right_motor, right_speed);
    
    }
  wb_robot_cleanup();
  return 0;
}