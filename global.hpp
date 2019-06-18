#pragma once

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <cstdarg>

// #include "system/system.h"
// #include "system/systick.h"
// #include "device_driver/led.h"
// #include "device_driver/st7735r.h"
// #include "device_driver/MT9V034.h"
// #include "device_driver/servo.h"
// #include "device_driver/alternate_motor.h"
// #include "device_driver/bluetooth.h"
// #include "device_driver/xpt2046.h"
// #include "device_driver/encoder.h"
// #include "driver/pit.h"

// #include "lvgl/lvgl.h"
// #include "image.hpp"
// #include "messaging.hpp"
// //#include "persistence.hpp"
// #include "movement_controller.hpp"
// #include "time_consuming_debug.hpp"
// #include "essential_system_tasks.hpp"

#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH 128

// 90 30 old
#define SERVO_MIDDLE_ANGLE 84.0f
#define SERVO_ANGLE_RANGE 36.0f
// below format (direction * servo_from_actual_deltaangle_ratio)
#define SERVO_DELTA_MULTIPLIER (-1.0f * 0.8f)

// Filled by COORD.hardcode
extern const float COORD[120][184][2];
// Global pointer to camera buffer
extern const uint8_t *CAM_BUFFER;

// extern DeviceDriver::Led *LED_1, *LED_2;
// extern DeviceDriver::St7735r *LCD;
// extern DeviceDriver::MT9V034 *CAMERA;
// extern DeviceDriver::Servo *SERVO;
// extern DeviceDriver::AlternateMotor *MOTOR_L, *MOTOR_R;
// extern DeviceDriver::Bluetooth *BLUETOOTH;
// extern DeviceDriver::XPT2046 *TOUCH;
// extern DeviceDriver::Encoder *ENCODER_L, *ENCODER_R;
// extern MovementController *MOVCTL;

extern volatile int volatile_dummy;

void ERROR(const char *msg);
void ERROR(std::string msg);
void SAFEHANG();
void SYSTEMRESET();


std::string StringFormat(const char *format, ...);


inline uint16_t RGB888toRGB565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

// Things that make sense
typedef std::pair<int, int> ii;
typedef std::pair<float, float> ff;
typedef std::vector<ii> vii;
typedef std::vector<ff> vff;
#define mp(x, y) std::make_pair(x, y)
#define ff2ii(x) (mp((int)(x).first, (int)(x).second))
#define ffDist(x) ( sqrt((x).first * (x).first + (x).second * (x).second) )
template <typename T, typename S>
std::pair<T, S> operator+(const std::pair<T, S> &a, const std::pair<T, S> &b) { return {a.first + b.first, a.second + b.second}; }
template <typename T, typename S>
std::pair<T, S>& operator+=(std::pair<T, S> &a, const std::pair<T, S> &b) { return a = mp(a.first + b.first, a.second + b.second); }
template <typename T, typename S>
std::pair<T, S> operator-(const std::pair<T, S> &a, const std::pair<T, S> &b) { return {a.first - b.first, a.second - b.second}; }
template <typename T, typename S>
std::pair<T, S>& operator-=(std::pair<T, S> &a, const std::pair<T, S> &b) { return a = mp(a.first - b.first, a.second - b.second); }
template <typename T>
std::pair<T, T> operator*(const std::pair<T, T> &a, T b) { return {a.first * b, a.second * b}; }
template <typename T>
std::pair<T, T>& operator*=(std::pair<T, T> &a, T b) { return a = mp(a.first * b, a.second * b); }
template <typename T>
std::pair<T, T> operator/(const std::pair<T, T> &a, T b) { return {a.first / b, a.second / b}; }
template <typename T>
std::pair<T, T>& operator/=(std::pair<T, T> &a, T b) { return a = mp(a.first / b, a.second / b); }
template <typename T, typename S>
std::ostream& operator<<(std::ostream &os, const std::pair<T, S> &p) { return os << p.first << " " << p.second; }

//extern volatile bool BTTX_EMPTY;

extern volatile float MOTOR_L_SPEED;
extern volatile float MOTOR_R_SPEED;

extern volatile int CURSOR_Y;
extern volatile int CURSOR_X;
extern volatile bool CURSOR_DOWN;
extern volatile int DRAG_DELTA_Y;
extern volatile int DRAG_DELTA_X;
extern volatile int ACCUMULATED_DRAG_DELTA_Y;
extern volatile int ACCUMULATED_DRAG_DELTA_X;

extern volatile int CURSOR_DELTA_Y_LOCAL;
extern volatile int CURSOR_DELTA_X_LOCAL;
extern volatile int CURSOR_Y_LOCAL;
extern volatile int CURSOR_X_LOCAL;
extern volatile bool CURSOR_DOWN_LOCAL;
extern volatile int CURSOR_OLD_Y_LOCAL;
extern volatile int CURSOR_OLD_X_LOCAL;
extern volatile bool CURSOR_OLD_DOWN_LOCAL;

extern volatile int CURSOR_DELTA_Y_REMOTE;
extern volatile int CURSOR_DELTA_X_REMOTE;
extern volatile bool CURSOR_DOWN_REMOTE;

extern volatile bool RC_MODE;
extern volatile int RC_DEADLINE;

extern volatile bool CAM_REQ;
extern volatile bool CAM_REQ_FULLRES;

/**
 * SETTINGS DEFINITIONS
 * 
 * [0] is EdgeCorner being requested
 * [1] is Whether temporary task enabled
 * [2] is Sobel in full res image
 * [3] is Sobel in printed image
 * [4] is Request FSM init
 * [5] is Enable/Disable running FSM
 * 
 * [10] is HIGH_PERF_NO_DEBUG
 */
extern volatile bool SETTINGS[256];

/**
 * SMART RATE LIMIT ASSIGNMENT
 * 
 * [0] allows unassigned
 * [1] allows EdgeCorner in temporary task
 * [2] allows EncoderMotorServo in independent debug
 */
extern volatile bool SRL_ALLOWED[256];
extern void SRL(uint8_t id);
extern void SRL_init();

extern volatile bool DEBUG_CENTER_CONNECTED;
extern volatile bool COLLEAGUE_CONNECTED; // feel free to rename to something better xd

// Heavy debug outputs should check for this flag first
//extern volatile bool HIGH_PERF_NO_DEBUG;
#define HIGH_PERF_NO_DEBUG SETTINGS[10]
extern volatile bool DISABLE_MOTORS_OVERRIDE;

// bt messaging
extern std::vector<uint8_t> BT_RX_BUF;
extern void BT_PROCESS_INPUT();