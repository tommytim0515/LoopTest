#include "global.hpp"
#include <iostream>

#include "COORD.hardcode"
// Global pointer to camera buffer
const uint8_t *CAM_BUFFER = nullptr;

// DeviceDriver::Led *LED_1 = nullptr, *LED_2 = nullptr;
// DeviceDriver::St7735r *LCD = nullptr;
// DeviceDriver::MT9V034 *CAMERA = nullptr;
// DeviceDriver::Servo *SERVO = nullptr;
// DeviceDriver::AlternateMotor *MOTOR_L = nullptr, *MOTOR_R = nullptr;
// DeviceDriver::Bluetooth *BLUETOOTH = nullptr;
// DeviceDriver::XPT2046 *TOUCH = nullptr;
// DeviceDriver::Encoder *ENCODER_L = nullptr, *ENCODER_R = nullptr;
// MovementController *MOVCTL = nullptr;

volatile int volatile_dummy = 0;

void ERROR(const char *msg) {
    //BLUETOOTH->SendString("Error occured. Will hang");
	// Message::Text("Error occured. Will hang").send();
	// Message::Text(msg).send();
	// SAFEHANG();
	std::cout << "Error occured. Will hang" << std::endl;
	std::cout << msg << std::endl;
}
void ERROR(std::string msg) { return ERROR(msg.c_str()); }
// void SAFEHANG() {
//     if (MOTOR_L != nullptr) MOTOR_L->SetSpeed(0);
//     if (MOTOR_R != nullptr) MOTOR_R->SetSpeed(0);
//     if (SERVO != nullptr) SERVO->SetDegree(SERVO_MIDDLE_ANGLE * 10);
//     while (true) volatile_dummy++;
// }
// void SYSTEMRESET() {
// 	__NVIC_SystemReset();
// }

std::string StringFormat(const char *format, va_list *vl) {
	static char product[64];
	vsnprintf(product, 64 * sizeof(char), format, *vl);
	product[63] = '\0';
	return product;
}
std::string StringFormat(const char *format, ...) {
	va_list vl;
	va_start(vl, format);
	const std::string &product = StringFormat(format, &vl);
	va_end(vl);
	return product;
}

volatile float MOTOR_L_SPEED = 0.0f;
volatile float MOTOR_R_SPEED = 0.0f;

volatile int CURSOR_Y = SCREEN_HEIGHT / 2;
volatile int CURSOR_X = SCREEN_WIDTH / 2;
volatile bool CURSOR_DOWN = false;
volatile int DRAG_DELTA_Y = 0;
volatile int DRAG_DELTA_X = 0;
volatile int ACCUMULATED_DRAG_DELTA_Y = 0;
volatile int ACCUMULATED_DRAG_DELTA_X = 0;

volatile int CURSOR_DELTA_Y_LOCAL = 0;
volatile int CURSOR_DELTA_X_LOCAL = 0;
volatile int CURSOR_Y_LOCAL = CURSOR_Y;
volatile int CURSOR_X_LOCAL = CURSOR_X;
volatile bool CURSOR_DOWN_LOCAL = false;
volatile int CURSOR_OLD_Y_LOCAL = CURSOR_Y;
volatile int CURSOR_OLD_X_LOCAL = CURSOR_X;
volatile bool CURSOR_OLD_DOWN_LOCAL = false;

volatile int CURSOR_DELTA_Y_REMOTE = 0;
volatile int CURSOR_DELTA_X_REMOTE = 0;
volatile bool CURSOR_DOWN_REMOTE = false;

volatile bool RC_MODE = false;
volatile int RC_DEADLINE = 0;

volatile bool CAM_REQ = false;
volatile bool CAM_REQ_FULLRES = false;

volatile bool DEBUG_CENTER_CONNECTED = false;
volatile bool COLLEAGUE_CONNECTED = false; // feel free to rename to something better xd

// Heavy debug outputs should check for this flag first
//volatile bool HIGH_PERF_NO_DEBUG = false;
volatile bool DISABLE_MOTORS_OVERRIDE = false;

// Bt messaging, currently assumes all messages come from debug center
// volatile bool SETTINGS[256] = {0};
// volatile bool SRL_ALLOWED[256];
// void SRL_init() { for (int i = 0; i < 256; i++) SRL_ALLOWED[i] = true; }
// void SRL(uint8_t id) {
// 	SRL_ALLOWED[id] = false;
// 	Message::SmartRateLimit msg(id);
// 	while ( ! msg.send() ) volatile_dummy++;
// }

std::vector<uint8_t> BT_RX_BUF;

// It's better to make this function as fast as possible, because it runs in an interrupt
// void BT_PROCESS_INPUT() {
// 	if (BT_RX_BUF.size() < 2) {
// 		BT_RX_BUF.clear();
// 		return;
// 	} 
// 	if (BT_RX_BUF[0] != Message::MAGIC_BYTE) {
// 		BT_RX_BUF.clear();
// 		return;
// 	}

// 	Message::MessageType *message = Message::unpack(BT_RX_BUF);
// 	BT_RX_BUF.clear();

// 	if (message->type == Message::MTYPE_INVALID) Message::Text("dear master, i received invalid msg").send();
// 	else if (message->type == Message::MTYPE_TEXT) Message::Text(std::string("dear master, i received this: ") + ((Message::Text*)message)->text ).send();
// 	else if (message->type == Message::MTYPE_CURSOR) {
// 		Message::Cursor::Body &cursorBody = ((Message::Cursor*)message)->body;
// 		CURSOR_DELTA_Y_REMOTE += cursorBody.deltaY;
// 		CURSOR_DELTA_X_REMOTE += cursorBody.deltaX;
// 		CURSOR_DOWN_REMOTE = cursorBody.pressedState;
// 	} else if (message->type == Message::MTYPE_RC) {
// 		Message::RC::Body &rcBody = ((Message::RC*)message)->body;
// 		RC_MODE = true;
// 		RC_DEADLINE = System::Systick::GetTimeInMS() + 200;
// 		//MOVCTL->setTargetSpeed(rcBody.motorSpeed, true); // enable again after pid tuning
// 		MOVCTL->forcePwm(-rcBody.motorSpeed, true);
// 		MOVCTL->setTargetPoint( mp(sin(rcBody.servoAngle/1800.0f*M_PI), cos(rcBody.servoAngle/1800.0f*M_PI)) , true);
// 	} else if (message->type == Message::MTYPE_CAMREQ) {
// 		Message::CamReq::Body &camReqBody = ((Message::CamReq*)message)->body;
// 		if (camReqBody.highResolution) CAM_REQ_FULLRES = true;
// 		else CAM_REQ = true;
// 	} else if (message->type == Message::MTYPE_RESETREQ) {
// 		SYSTEMRESET();
// 	} else if (message->type == Message::MTYPE_SYNC_REQUEST) {
// 		DEBUG_CENTER_CONNECTED = false;
// 		int rv = ((Message::SyncRequest*)message)->req_val;
// 		Message::SyncResponse(rv).send();
// 	} else if (message->type == Message::MTYPE_SYNC_RESPONSE) {
// 		int rv = ((Message::SyncResponse*)message)->res_val;
// 		if (rv == 0) DEBUG_CENTER_CONNECTED = true;
// 	} else if (message->type == Message::MTYPE_DMO) {
// 		Message::DMO::Body &dmoBody = ((Message::DMO*)message)->body;
// 		DISABLE_MOTORS_OVERRIDE = dmoBody.dmoVal;
// 	} else if (message->type == Message::MTYPE_SETTING) {
// 		Message::Setting::Body &sBody = ((Message::Setting*)message)->body;
// 		SETTINGS[sBody.id] = sBody.value;
// 	} else if (message->type == Message::MTYPE_SMART_RATE_LIMIT) {
// 		SRL_ALLOWED[ ((Message::SmartRateLimit*)message)->limitId ] = true;;
// 	} else {
// 		ERROR("received correct magic but unknown type message");
// 	}

// 	delete message;
// }