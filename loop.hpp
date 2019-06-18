#pragma once

#include "global.hpp"
#include "image.hpp"
#include "track_processing.hpp"
#include "GrayBMP24.hpp"

enum class State {
	INVALID,
	INITIAL,
	NORMAL,
	CROSS_MID_S1,
	CROSS_MID_S2,

	// PRE_LOOP_L,
	// MID_LOOP_L,
	// IN_LOOP_N_L,
	// IN_LOOP_X_L,
	// POST_LOOP_L,
	// PRE_LOOP_R,
	// MID_LOOP_R,
	// IN_LOOP_N_R,
	// IN_LOOP_X_R,
	// POST_LOOP_R,
	
    PRE_LOOP_L,
    INTO_LOOP_L,
    IN_LOOP_L,
    OUT_LOOP_L,
    END_LOOP_L,
    LOOP_TO_NOR_L,
    PRE_LOOP_R,
    INTO_LOOP_R,
    IN_LOOP_R,
    OUT_LOOP_R,
    END_LOOP_R,
    LOOP_TO_NOR_R,

	PRE_MAG,
	IN_MAG,
	PRE_FIN,
	IN_FIN,
	IMPOSSIBLE
};

//State STATE = State::NORMAL;

const ii checkPoint(Image::HEIGHT / 2, Image::WIDTH / 2);
const ii outLoopPoint(2*Image::HEIGHT / 5, Image::WIDTH / 2);
const ii endLoopPoint(Image::HEIGHT / 2, Image::WIDTH / 2);
const ii bottomPoint(Image::HEIGHT - 2, Image::WIDTH / 2);

const int shift_x_L = -30;
const int shift_y_L = 15;
const int shift_x_R = 30;
const int shift_y_R = 15;

void PRE_LOOP_L(GrayBMP &bmp);
void INTO_LOOP_L(GrayBMP &bmp);
void IN_LOOP_L(GrayBMP &bmp);
void OUT_LOOP_L(GrayBMP &bmp);
void END_LOOP_L(GrayBMP &bmp);
void LOOP_TO_NOR_L(GrayBMP &bmp);

void PRE_LOOP_R(GrayBMP &bmp);
void INTO_LOOP_R(GrayBMP &bmp);
void IN_LOOP_R(GrayBMP &bmp);
void OUT_LOOP_R(GrayBMP &bmp);
void END_LOOP_R(GrayBMP &bmp);
void LOOP_TO_NOR_R(GrayBMP &bmp);

