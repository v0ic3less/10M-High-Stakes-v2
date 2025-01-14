// #include "main.h"
// #include "lemlib/api.hpp"
// #include "constants.hpp"
// #include "pros/adi.hpp"
// #include "pros/misc.h"
// #include "pros/misc.hpp"
// #include "pros/motors.h"
// #include "pros/rotation.hpp"

// //color
// enum AllianceColor { BLUE = 1, RED = 2 };
// AllianceColor alliance_color = BLUE;

// // init motorgroups
// pros::MotorGroup left_motors({LEFT_MOTOR_FRONT, LEFT_MOTOR_TOP, LEFT_MOTOR_BACK});
// pros::MotorGroup right_motors({RIGHT_MOTOR_FRONT, RIGHT_MOTOR_TOP, RIGHT_MOTOR_BACK});

// // init motors
// pros::Motor intake(INTAKE_PORT);
// pros::Motor hook(HOOK_PORT, pros::MotorGearset::blue);
// pros::Motor lb(LB_PORT);

// // init pneumatics
// pros::adi::DigitalOut clamp(CLAMP_PORT);
// pros::adi::DigitalOut doinker(DOINKER_PORT);

// // init sensors
// pros::Imu imu(IMU_PORT);
// pros::Optical optical(OPTICAL_PORT);
// pros::Rotation lb_encoder(LB_ROTATION_PORT);
// // pros::Rotation vertical_encoder(VERTICAL_TRACKING_ROTATION_PORT);
// // pros::Rotation horizontal_encoder(HORTIZONTAL_TRACKING_ROTATION_PORT);

// //init controller
// pros::Controller controller(pros::E_CONTROLLER_MASTER);

// // init drivetrain
// lemlib::Drivetrain drivetrain(&left_motors, // left motor group
//                               &right_motors, // right motor group
//                               12.24, // track width
//                               lemlib::Omniwheel::NEW_325, // wheel diameter
//                               450, // drivetrain rpm
//                               2 // horizontal drift is 2 (for now)
// );

// // init tracking wheels
// // horizontal tracking wheel
// // lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
// // vertical tracking wheel
// // lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// //init odom
// lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
//                             nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
//                             nullptr, // horizontal tracking wheel 1
//                             nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
//                             &imu // inertial sensor
// );

// // init PID
// // lateral PID controller
// lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               3, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in inches
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in inches
//                                               500, // large error range timeout, in milliseconds
//                                               20 // maximum acceleration (slew)
// );

// // angular PID controller
// lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               10, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in degrees
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in degrees
//                                               500, // large error range timeout, in milliseconds
//                                               0 // maximum acceleration (slew)
// );

// // init input curve

// // input curve for throttle input during driver control
// lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
//                                      10, // minimum output where drivetrain will move out of 127
//                                      1.019 // expo curve gain
// );

// // input curve for steer input during driver control
// lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
//                                   10, // minimum output where drivetrain will move out of 127
//                                   1.019 // expo curve gain
// );

// //init chassis
// lemlib::Chassis chassis(drivetrain,
//                         lateral_controller,
//                         angular_controller,
//                         sensors,
//                         &throttle_curve, 
//                         &steer_curve
// );

// //init boolean control
// bool clamp_down = false;
// bool doinker_down = false;

// //init color sort vars
// int optical_value;
// int color_low_range;
// int color_high_range;
// int opposing_color_low_range;
// int opposing_color_high_range;

// //init lb vars
// const double LB_LOW_POSITION = 0;   
// const double LB_MID_POSITION = 50;  
// const double LB_HIGH_POSITION = 200; 

// //ladybrown control
// // Define constants for PID tuning (needs to be tuned)
// const double lb_kP = 1.0; // Proportional constant
// const double lb_kI = 0.0; // Integral constant
// const double lb_kD = 0.0; // Derivative constant


// const double lb_max_velocity = 400;  // Maximum motor speed
// class LbArmPID {
// private:
//     double previous_error;
//     double integral;
//     double target_position;

// public:
//     // Constructor to init PID vars
//     LbArmPID() {
//         previous_error = 0;
//         integral = 0;
//         target_position = 0;
//     }

//     void setTargetPosition(double position) {
//         target_position = position;
//     }

// 	double getTargetPosition() {
// 		return target_position;
// 	}

//     void pidControl() {
//         double error = target_position - lb_encoder.get_position();  // Calculate the error
//         integral += error; // Integrate the error over time
//         double derivative = error - previous_error; // Calculate the derivative (change in error)

//         // PID formula
//         double output = (lb_kP * error) + (lb_kI * integral) + (lb_kD * derivative);

//         // Limit the output to the max motor speed
//         if (output > lb_max_velocity) {
//             output = lb_max_velocity;
//         } else if (output < -lb_max_velocity) {
//             output = -lb_max_velocity;
//         }

//         // Apply the calculated output to the motor
//         lb.move_velocity(output);

//         // Update previous error for the next iteration
//         previous_error = error;
//     }

//     void moveArmToTarget() {
//         lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 		int iterations = 0;
// 		while (iterations < 1000) { // Maximum of 1000 iterations
// 			pidControl();
// 			if (std::abs(target_position - lb_encoder.get_position()) < 5) {
// 				lb.move_velocity(0);
// 				break;
// 			}
// 			pros::delay(20);
// 			iterations++;
// 		}
//     }

//     double getCurrentPosition() {
//         return lb_encoder.get_position();
//     }
// };



// void initialize() {
// 	pros::lcd::initialize(); // initialize brain screen
// 	chassis.calibrate(); // calibrate sensors
// 	hook.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 	lb_encoder.reset();
// 	// horizontal_tracking_wheel.reset();
// 	// horizontal_tracking_wheel.reset();
// 	optical.set_led_pwm(100);
// 	optical.set_integration_time(20);
// 	while (true) {
//         // print measurements from the rotation sensor
//         // pros::lcd::print(0, "Vertical: %i", vertical_encoder.get_position());
// 		// pros::lcd::print(1, "Horizontal: %i", horizontal_encoder.get_position());
// 		pros::lcd::print(2, "Ladybrown: %i", lb_encoder.get_position());

// 		// print robot location to the brain screen
// 		pros::lcd::print(3, "X: %f", chassis.getPose().x); // x
// 		pros::lcd::print(4, "Y: %f", chassis.getPose().y); // y
// 		pros::lcd::print(5, "Theta: %f", chassis.getPose().theta); // heading

// 		//sensor values
// 		pros::lcd::print(6, "Optical Hue: %f", optical.get_hue());
// 		// delay to save resources
// 		pros::delay(20);
//     }
// }

// void disabled() {}

// void competition_initialize() {}

// void autonomous() {
//     intake.move_velocity(400);
// }

// void pneumatic_control() {
// 	while (true) {
// 		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
// 			doinker_down = !doinker_down;
// 			doinker.set_value(doinker_down);
// 		} else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
// 			clamp_down = !clamp_down;
// 			clamp.set_value(clamp_down);
// 		}
// 		pros::delay(20);
// 	}
// }

// LbArmPID lbArmPID;
// void lb_control() {
//     while (true) {
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
//             lbArmPID.setTargetPosition(LB_HIGH_POSITION);
//             lbArmPID.moveArmToTarget();
//         } 
//         else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
//             lbArmPID.setTargetPosition(LB_MID_POSITION);
//             lbArmPID.moveArmToTarget();
//         } 
//         else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//             lbArmPID.setTargetPosition(LB_LOW_POSITION);
//             lbArmPID.moveArmToTarget();
//         }

//         pros::delay(20);
//     }
// }


// void intake_control(void* color) {
// 	if (*reinterpret_cast<AllianceColor*>(color) == BLUE){ // blue
// 		color_low_range = 200;
// 		color_high_range = 240;
// 		opposing_color_low_range = 0;
// 		opposing_color_high_range = 20;
// 	} else { // red
// 		color_low_range = 0;
// 		color_high_range = 20;
// 		opposing_color_low_range = 200;
// 		opposing_color_high_range = 240;
// 	}
//     while (true) {
//         optical_value = optical.get_hue();
//         if (optical_value >= opposing_color_low_range && optical_value <= opposing_color_high_range) {
//             // If optical sensor detects a value between 0 and 20, keep motors stopped for 30ms
// 			pros::delay(30);
//             intake.move_velocity(0);
//             hook.brake();
// 			pros::delay(30);
//         } else if (optical_value >= color_low_range && optical_value <= color_high_range && lbArmPID.getTargetPosition() == LB_MID_POSITION) {
// 			pros::delay(30);
// 			hook.brake();
// 			intake.brake();
// 			pros::delay(1000);
// 		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
//             intake.move_velocity(400);
//             hook.move_velocity(600);
//         } else {
//             intake.move_velocity(0);
//             hook.brake();
//         }

//         pros::delay(20);
// 	}
// }

// void opcontrol() {
// 	// pros::Task pneumatics(pneumatic_control);
// 	// pros::Task intake(intake_control, &alliance_color);
// 	// pros::Task lb(lb_control);
// 	while (true) {
//         // get left y and right y positions
//         int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
//         int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

//         // move the robot
//         chassis.tank(leftY, rightY);

//         // delay to save resources
//         pros::delay(20);
// 	};
// }


#include "main.h"
#include <iostream>
#include "lemlib/api.hpp"
#include "constants.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rotation.hpp"
#include "pros/rtos.hpp"

// init motorgroups
pros::MotorGroup left_motors({LEFT_MOTOR_FRONT, LEFT_MOTOR_TOP, LEFT_MOTOR_BACK});
pros::MotorGroup right_motors({RIGHT_MOTOR_FRONT, RIGHT_MOTOR_TOP, RIGHT_MOTOR_BACK});

// init motors
pros::Motor intake(INTAKE_PORT);
pros::Motor hook(HOOK_PORT, pros::MotorGearset::blue);
pros::Motor lb(LB_PORT);

// init pneumatics
pros::adi::DigitalOut clamp(CLAMP_PORT);
pros::adi::DigitalOut doinker(DOINKER_PORT);

// init sensors
pros::Imu imu(IMU_PORT);
pros::Imu imu2(IMU2_PORT);
pros::Optical optical(OPTICAL_PORT);
pros::Rotation lb_encoder(LB_ROTATION_PORT);

// init controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

lemlib::PID lb_pid(1, 0, 1);

// init drivetrain
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              12.24, // track width
                              lemlib::Omniwheel::NEW_325, // wheel diameter
                              450, // drivetrain rpm
                              4 // horizontal drift is 2 (for now)
);

// init PID controllers
lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              4, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(3, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// init input curve
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                      10, // minimum output where drivetrain will move out of 127
                                      1.019 // expo curve gain
);

lemlib::ExpoDriveCurve steer_curve(6, // joystick deadband out of 127
                                   15, // minimum output where drivetrain will move out of 127
                                   1.01 // expo curve gain
);

//init odom
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu2 // inertial sensor
);

// init chassis
lemlib::Chassis chassis(drivetrain,
                        lateral_controller,
                        angular_controller,
                        sensors, // No tracking wheels
                        &throttle_curve, 
                        &steer_curve
);

bool clamp_down = false;
bool doinker_down = false;

float target_position = 0;

// Initialize intake control with color sorting
void intake_control() {
    while (true) {
        // If the R1 button is pressed, start the intake (200-240 Blue, 5-45 Red)
        if (optical.get_hue() > 5 & optical.get_hue() < 45){
            intake.move_velocity(0);
            hook.move_velocity(0);
            pros::delay(51);
        }
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move_velocity(600);  // Move intake forward
            hook.move_velocity(600);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            intake.move_velocity(-600);
            hook.move_velocity(-600);
        }
        // If neither button is pressed, stop the intake
        else {
            intake.move_velocity(0); // Stop the intake
            hook.move_velocity(0);
        }


        pros::delay(20);  // Small delay to prevent controller overload
    }
}


void pneumatic_control() {
	while (true) {
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			clamp_down = !clamp_down;
			clamp.set_value(clamp_down);
		}
		pros::delay(20);
	}
}

void lb_pid_task() {
    while (true) {
        double current_position = lb_encoder.get_position(); // Read motor encoder position
        double output = lb_pid.update(current_position-target_position); // Calculate PID output
        lb.move(output); // Apply output to the motor

        pros::delay(20); // Allow the system to update
    }
}

void lb_control(){
    while (true) {
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
            target_position = -83.5;
            lb.move_absolute(-275, 400);
        } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            target_position = 0;
            lb.move_absolute(0, 400);
        } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
            lb.move_absolute(-1600, 400);
        }
        pros::delay(20);
    }
}

void initialize() {
	pros::lcd::initialize(); // initialize brain screen
	chassis.calibrate(); // calibrate sensors
    lb.tare_position();
    optical.set_integration_time(20);
    optical.set_led_pwm(50);
    lb_encoder.reset_position();
    lb.set_brake_mode(pros::MotorBrake::hold);
	// lb_encoder.reset();
	// optical.set_led_pwm(100);
	// optical.set_integration_time(20);
	 pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(4, "Hook Draw: %f", hook.get_current_draw()); // draw
            pros::lcd::print(5, "Color Hue: %f", optical.get_hue()); // hue
            pros::lcd::print(6, "LB Position %f", lb_encoder.get_position() * 0.01);

            std::cout << "Position: " << lb_encoder.get_position() << std::endl;
            // delay to save resources
            pros::delay(20);
        }
    });
}

void autonomous() {
    chassis.setPose(0, 0, 0);
    clamp.set_value(false);

    //RED LEFT NEG
    // chassis.swingToHeading(-40, DriveSide::LEFT, 700, {}, false);
    // lb.move_relative(-1400, 600);
    // pros::delay(1000);
    // chassis.moveToPoint(0, -10, 700, {.forwards = false, .earlyExitRange=2}, false);
    // chassis.moveToPoint(12, -35, 1500, {.forwards=false, .maxSpeed=120}, false);
    // pros::delay(100);
    // intake.move_velocity(600);
    // hook.move_velocity(600);
    // clamp.set_value(true);
    // pros::delay(600);
    // chassis.moveToPoint(30, -48.5, 1000, {.maxSpeed=100}, false);
    // pros::delay(1000);
    // chassis.moveToPoint(41, -48.5, 1000, {.maxSpeed=60}, false);
    // pros::delay(1000);
    // chassis.moveToPoint(10, -38, 1000, {.forwards=false}, false);
    // chassis.moveToPoint(34, -30, 1000, {}, false);
    // pros::delay(1300);
    // chassis.moveToPoint(0, -39, 1800, {}, false);

    //RIGHT BLUE NEG
    // chassis.swingToHeading(40, DriveSide::RIGHT, 700, {}, false);
    // lb.move_relative(-1400, 600);
    // pros::delay(1000);
    // chassis.moveToPoint(0, -10, 700, {.forwards = false, .earlyExitRange=2}, false);
    // chassis.moveToPoint(-12, -35, 1500, {.forwards=false, .maxSpeed=120}, false);
    // pros::delay(100);
    // intake.move_velocity(600);
    // hook.move_velocity(600);
    // clamp.set_value(true);
    // pros::delay(600);
    // chassis.moveToPoint(-30, -48.5, 1000, {.maxSpeed=100}, false);
    // pros::delay(1000);
    // chassis.moveToPoint(-41, -48.5, 1000, {.maxSpeed=60}, false);
    // pros::delay(1000);
    // chassis.moveToPoint(-10, -38, 1000, {.forwards=false}, false);
    // chassis.moveToPoint(-34, -30, 1000, {}, false);
    // pros::delay(1300);
    // chassis.moveToPoint(0, -35, 1800, {}, false);

    //RED RIGHT
    chassis.moveToPoint(0, -10, 700, {.forwards = false, .earlyExitRange=2}, false);
    chassis.moveToPoint(-12, -35, 1500, {.forwards=false, .maxSpeed=120}, false);
    pros::delay(100);
    intake.move_velocity(600);
    hook.move_velocity(600);
    clamp.set_value(true);
    pros::delay(600);
    chassis.moveToPoint(34, -34, 1000, {}, false);
    chassis.moveToPoint(55, 5, 1000);
}   

void opcontrol() {
    // Start intake control in a separate task to manage intake behavior
    pros::Task intake_task(intake_control);
    pros::Task pneumatic_task(pneumatic_control);
    // pros::Task lb_pid_task_task(lb_pid_task);
    pros::Task lb_task(lb_control);
    
    while (true) {
        // get left y and right y positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // move the robot
        chassis.arcade(leftY, rightX);

        // delay to save resources
        pros::delay(20);
    }
}