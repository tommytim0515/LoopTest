#include "loop.hpp"
//#include "fsm.hpp"
#include "track_processing.hpp"
#include "imageProc.h"
#include <algorithm>
#include <iostream>
#include <iostream>

using namespace TrackProcessing;

std::string getStateStr(State s) {
	if (s == State::INVALID) return "INVALID";
	if (s == State::INITIAL) return "INITIAL";
	if (s == State::NORMAL) return "NORMAL";
	if (s == State::CROSS_MID_S1) return "CROSS_MID_S1";
	if (s == State::CROSS_MID_S2) return "CROSS_MID_S2";
	if (s == State::PRE_MAG) return "PRE_MAG";
	if (s == State::IN_MAG) return "IN_MAG";
	if (s == State::PRE_FIN) return "PRE_FIN";
	if (s == State::IN_FIN) return "IN_FIN";
	if (s == State::IMPOSSIBLE) return "IMPOSSIBLE";

    if (s == State::PRE_LOOP_L) return "PRE_LOOP_L";
    if (s == State::INTO_LOOP_L) return "INTO_LOOP_L";
    if (s == State::IN_LOOP_L) return "IN_LOOP_L";
    if (s == State::OUT_LOOP_L) return "OUT_LOOP_L";
    if (s == State::END_LOOP_L) return "END_LOOP_L";
    if (s == State::LOOP_TO_NOR_L) return "LOOP_TO_NOR_L";
    if (s == State::PRE_LOOP_R) return "PRE_LOOP_R";
    if (s == State::INTO_LOOP_R) return "INTO_LOOP_R";
    if (s == State::IN_LOOP_R) return "IN_LOOP_R";
    if (s == State::OUT_LOOP_R) return "OUT_LOOP_R";
    if (s == State::END_LOOP_R) return "END_LOOP_R";
    if (s == State::LOOP_TO_NOR_R) return "LOOP_TO_NOR_R";

	return "UNSTRINGIFIED";
}
//State STATE = State::INVALID;
//void SWITCH_STATE(State nextState);


vff subSimplePathPlan(const vii &edge, bool isLeft) {
    auto midline = isLeft ? getMidlineFromLeft(edge) : getMidlineFromRight(edge);
    ff targetPoint = TrackProcessing::amrutSelector(midline);
	//if (!HIGH_PERF_NO_DEBUG) Message::TargetPoint(targetPoint).send();
	//MOVCTL->setTargetPoint(targetPoint);
	return midline;
}

vff simplePathPlan(const vii &leftEdge, const vii &rightEdge) {
	bool chosenIsLeft = leftEdge.size() > rightEdge.size();
	auto midline = chosenIsLeft ? getMidlineFromLeft(leftEdge) : getMidlineFromRight(rightEdge);
	ff targetPoint = amrutSelector(midline);
	//if (!HIGH_PERF_NO_DEBUG) Message::TargetPoint(targetPoint).send();
	//MOVCTL->setTargetPoint(targetPoint);
	return midline;
}
/*
void PRE_LOOP() {
    ii leftEdgeSerchPoint = TrackProcessing::goUntilObstacle(checkPoint, mp(0, -1));
    ii rightEdgeSearchPoint = TrackProcessing::goUntilObstacle(checkPoint, mp(0, 1));
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSerchPoint, mp(-1, 0));
    viic leftDownEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSerchPoint, mp(1, 0));
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    vii leftEdge;
    leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightEdgeSearchPoint, mp(-1, 0));
    viic rightDownEdge = TrackProcessing::goUntilCorner_RE(rightEdgeSearchPoint, mp(-1, 0));
    std::reverse(rightUpEdge.first.begin(), rightUpEdge.first.end());
    vii rightEdge;
    rightEdge.reserve(rightUpEdge.first.size() + rightDownEdge.first.size());
    
}*/
//int boundaryThres = 20;

bool isBoundary(ii coor, int boundaryThres) {
    if (coor.first <= boundaryThres || coor.first >= (Image::HEIGHT - boundaryThres) || coor.second <= boundaryThres || coor.second >= (Image::WIDTH - boundaryThres))
        return true;
    return false;
}

void PRE_LOOP_R(GrayBMP &bmp) {
    // change something here
    ii leftEdgeSearPoint = TrackProcessing::goUntilObstacle(checkPoint, TrackProcessing::normalizeVector(mp(-0.1, -1.2)));
    ii rightEdgeSearPoint = TrackProcessing::goUntilObstacle(checkPoint,TrackProcessing::normalizeVector(mp(-0.1, 1.2)));
    makeSmallDot(bmp, checkPoint);
    makeSmallDot(bmp, leftEdgeSearPoint);
    makeSmallDot(bmp, rightEdgeSearPoint);
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSearPoint, mp(-1, 0));
    viic leftDownEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSearPoint, mp(1, 0));
    vii leftEdge;
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    viic detectCorner;
    detectCorner.second = Corner::NONE;
    if (!isBoundary(rightEdgeSearPoint, 10)) {
        detectCorner = TrackProcessing::goUntilCorner_RE(rightEdgeSearPoint, mp(-1, 0));
        makeSmallDot(bmp, detectCorner.first[detectCorner.first.size() - 1]);
        if (TrackProcessing::isActuallyCorner(detectCorner.second))
            std::cout << "Corner on the right detected" << std::endl;
    }
    if (isBoundary(rightEdgeSearPoint, 10) || (TrackProcessing::isActuallyCorner(detectCorner.second) 
        && !isBoundary(detectCorner.first[detectCorner.first.size() - 1 - CORNER_INTERVAL], 20))/* && !TrackProcessing::isActuallyCorner(leftDownEdge.second) && !TrackProcessing::isActuallyCorner(leftUpEdge.second) */) {
        showEdge(bmp, leftEdge);
        auto midline = subSimplePathPlan(leftEdge, 1);
        return;
    }
    viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightEdgeSearPoint, mp(-1, 0));
    viic rightDownEdge = TrackProcessing::goUntilCorner_RE(rightEdgeSearPoint, mp(1, 0));
    vii rightEdge;
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.reserve(rightUpEdge.first.size() + rightDownEdge.first.size());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    float min = getCoords(rightEdge[0]).second;
    int counter = 0;
    for (int i = 1; i < rightEdge.size(); ++i) {
        //std::cout << getCoords(rightEdge[i]).second << std::endl;
        if (getCoords(rightEdge[i]).second < min) {
            min = getCoords(rightEdge[i]).second;
            counter = i;
        }
    }
    //std::cout << "counter: " << counter << std::endl;
    //showEdge(bmp, rightEdge);
    vii adjustedRightEdge;
    adjustedRightEdge.reserve(rightEdge.size() - counter);
    adjustedRightEdge.insert(adjustedRightEdge.end(), rightEdge.begin() + counter, rightEdge.end());
    showEdge(bmp, adjustedRightEdge);
    std::cout << "adjusted size: " << adjustedRightEdge.size() << std::endl;
    auto midline = subSimplePathPlan(adjustedRightEdge, 0);
    std::cout << "Switch to INTO_LOOP_R" << std::endl;
    return;//  SWITCH_STATE(State::INTO_LOOP_R);
}

void INTO_LOOP_R(GrayBMP &bmp) {
    ii shiftedCheckPoint(checkPoint.first + shift_y_R, checkPoint.second + shift_x_R);
    makeSmallDot(bmp, shiftedCheckPoint);
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(shiftedCheckPoint, normalizeVector(mp(-0.5, 0.4)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(shiftedCheckPoint, normalizeVector(mp(0.2, 1)));
    makeSmallDot(bmp, leftSearchPoint);
    viic leftDownEdge = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(0, -1));
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 0));
    vii leftEdge;
    //if (leftDownEdge.second == TrackProcessing::Corner::CW) {
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin() + CORNER_INTERVAL, leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    //std::cout << leftDownEdge.first.size() << std::endl
    //    << leftUpEdge.first.size() << std::endl;
    showEdge(bmp, leftEdge);
    //showEdge(bmp, leftDownEdge.first);
    //}
    viic rightDownEdge, rightUpEdge;
    if (!isBoundary(rightSearchPoint, 10)) {
        rightDownEdge = TrackProcessing::goUntilCorner_LE(rightSearchPoint, mp(1, 0));
        rightUpEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(0, 1));
    }
    vii rightEdge;
    rightEdge.reserve(rightDownEdge.first.size() + rightUpEdge.first.size());
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    auto &right = rightEdge;
    showEdge(bmp, rightEdge);
    if (rightEdge.size()) {
        float min = getCoords(rightEdge[0]).second;
        int counter = 0;
        for (int i = 0; i < rightEdge.size(); i++) {
            if (getCoords(rightEdge[i]).second < min) {
                min = getCoords(rightEdge[0]).second;
                counter = i;
            }
        }
        vii adjustedRightEdge;
        adjustedRightEdge.reserve(rightEdge.size() - counter);
        adjustedRightEdge.insert(adjustedRightEdge.end(), rightEdge.begin() + counter, rightEdge.end());
        //showEdge(bmp, adjustedRightEdge);
        right = adjustedRightEdge;
    }
    //showEdge(bmp, rightEdge);
    auto &left = leftEdge;
    auto midline = simplePathPlan(left, right);
    if (!TrackProcessing::isActuallyCorner(leftDownEdge.second)) {
        std::cout << "Switch to IN_LOOP_R" << std::endl;
        return;// SWITCH_STATE(State::IN_LOOP_R);
    }
}

void IN_LOOP_R(GrayBMP &bmp) {
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(bottomPoint, normalizeVector(mp(-0.2, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(bottomPoint, normalizeVector(mp(-0.2, 1)));
    makeSmallDot(bmp, bottomPoint);
    makeSmallDot(bmp, leftSearchPoint.second, leftSearchPoint.first);
    makeSmallDot(bmp, rightSearchPoint.second, rightSearchPoint.first);
    //viic leftDownEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(0, -1));
    //viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(0, 1));
    viic leftEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 1));
    if (TrackProcessing::isActuallyCorner(leftEdge.second) && leftEdge.first[leftEdge.first.size() - CORNER_INTERVAL - 1].first > Image::HEIGHT / 2) {
        //ii searchPoint = TrackProcessing::goUntilObstacle(leftEdge.first[leftEdge.first.size() - 1], TrackProcessing::normalizeVector(mp(-1, 0.3)));
        //viic leftLeftEdge = TrackProcessing::goUntilCorner_RE(searchPoint, mp(0, -1));
        //viic newLeftEdge = TrackProcessing::goUntilCorner_LE(searchPoint, mp(-1, 2));
        //makeSmallDot(bmp, searchPoint);
        // vii newLeftEdge;
        // std::reverse(leftLeftEdge.first.begin(), leftLeftEdge.first.end());
        // newLeftEdge.insert(newLeftEdge.end(), leftLeftEdge.first.begin(), leftLeftEdge.first.end());
        // newLeftEdge.insert(newLeftEdge.end(), leftRightEdge.first.begin(), leftRightEdge.first.end());
        // auto &left = newLeftEdge;//leftEdge.first;
        vii newLeftEdge;
        newLeftEdge.reserve(leftEdge.first.size() - CORNER_INTERVAL);
        newLeftEdge.insert(newLeftEdge.end(), leftEdge.first.begin(), leftEdge.first.end() - CORNER_INTERVAL);
        showEdge(bmp, newLeftEdge);
        //showEdge(bmp, newLeftEdge.first);
        auto &left = newLeftEdge;
        auto midline = subSimplePathPlan(left, 1);
        std::cout << "Switch to OUT_LOOP_R" << std::endl;
        return;// SWITCH_STATE(State::OUT_LOOP_R);
    }
    // vii leftEdge;
    // leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    // std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    // leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    // leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    if (isBoundary(rightSearchPoint, 10)) {
        vii adjustedLeftEdge;
        adjustedLeftEdge.reserve(leftEdge.first.size() - CORNER_INTERVAL);
        adjustedLeftEdge.insert(adjustedLeftEdge.end(), leftEdge.first.begin(), leftEdge.first.end() - CORNER_INTERVAL);
        auto& edge = adjustedLeftEdge;
        auto midline = subSimplePathPlan(edge, 1);
        showEdge(bmp, leftEdge.first);
        return;
    }
    // viic rightDownEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(1, 0));
    // viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    viic rightEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, TrackProcessing::normalizeVector(mp(-1, 0)));
    //rightEdge.reserve(rightDownEdge.first.size() + rightUpEdge.first.size());
    //std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    //rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    //rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    vii adjustedLeftEdge;
    adjustedLeftEdge.reserve(leftEdge.first.size() - CORNER_INTERVAL);
    adjustedLeftEdge.insert(adjustedLeftEdge.end(), leftEdge.first.begin(), leftEdge.first.end() - CORNER_INTERVAL);
    auto &left = adjustedLeftEdge;
    auto &right = rightEdge.first;
    showEdge(bmp, adjustedLeftEdge);
    showEdge(bmp, rightEdge.first);
    auto midline = simplePathPlan(left, right);
    return;
}

void OUT_LOOP_R(GrayBMP &bmp) {
    // ii leftSearchPoint = TrackProcessing::goUntilObstacle(checkPoint, TrackProcessing::normalizeVector(mp(-1, -1)));
    // ii rightSearchPoint = TrackProcessing::goUntilObstacle(checkPoint, TrackProcessing::normalizeVector(mp(0, 1)));
    // viic leftDownEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(1, 0));
    // viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 0));
    // vii leftEdge;
    // leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    // std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    // leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    // leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(outLoopPoint, TrackProcessing::normalizeVector(mp(-0.3, -1)));
    if (isBoundary(leftSearchPoint, 10)) {
        ii searchPoint = goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-1, -0.4)));
        makeSmallDot(bmp, searchPoint);
        viic leftDownEdge = TrackProcessing::goUntilCorner_LE(searchPoint, mp(0, -1));
        viic leftUpEdge = TrackProcessing::goUntilCorner_LE(searchPoint, mp(0, 1));
        vii leftEdge;
        leftEdge.reserve(leftDownEdge.first.size() + leftUpEdge.first.size());
        std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
        leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
        leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
        auto &edge = leftEdge;
        //showEdge(bmp, leftEdge);
        auto midline = subSimplePathPlan(edge, 1);
        std::cout << "Switch to END_LOOP_R" << std::endl;
        return; //SWITCH_STATE(State::END_LOOP_R);
    }
    viic leftCorner = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 0));
    if (!TrackProcessing::isActuallyCorner(leftCorner.second)) {
        ii searchPoint = goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-1, -0.4)));
        //viic leftDownEdge = TrackProcessing::goUntilCorner_LE(searchPoint, mp(0, -1));
        viic leftUpEdge = TrackProcessing::goUntilCorner_LE(searchPoint, mp(-1, 1));
        // vii leftEdge;
        // leftEdge.reserve(leftDownEdge.first.size() + leftUpEdge.first.size());
        // std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
        // leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
        // leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
        auto &edge = leftUpEdge.first;
        showEdge(bmp, leftUpEdge.first);
        auto midline = subSimplePathPlan(edge, 1);
        std::cout << "Switch to END_LOOP_R" << std::endl;
        return;// SWITCH_STATE(State::END_LOOP_R);
    }
    ii searchPoint = leftCorner.first.back();
    makeSmallDot(bmp, searchPoint);
    ii edgePoint = TrackProcessing::goUntilObstacle(searchPoint, TrackProcessing::normalizeVector(mp(-1, 0)));
    viic leftLeftEdge = goUntilCorner_LE(edgePoint, mp(0, -1));
    viic leftRightEdge = goUntilCorner_LE(edgePoint, mp(0, 1));
    vii leftEdge;
    leftEdge.reserve(leftLeftEdge.first.size() + leftRightEdge.first.size());
    std::reverse(leftLeftEdge.first.begin(), leftLeftEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftLeftEdge.first.begin(), leftLeftEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftRightEdge.first.begin(), leftRightEdge.first.end());
    auto &edge = leftEdge;
    showEdge(bmp, leftEdge);
    auto midline = subSimplePathPlan(edge, 1);
    return;
}

void END_LOOP_R(GrayBMP &bmp) {
    makeSmallDot(bmp, endLoopPoint);
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, 1)));
    makeSmallDot(bmp, leftSearchPoint);
    makeSmallDot(bmp, rightSearchPoint);
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 0));
    viic leftDownEdge = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(1, 0));
    vii leftEdge;
    leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    showEdge(bmp, leftEdge);
    auto &edge = leftEdge;
    auto midline = subSimplePathPlan(edge, 1);
    if (isBoundary(rightSearchPoint, 10))
        return;
    viic rightCorner = TrackProcessing::goUntilCorner_LE(rightSearchPoint, mp(1, 0));
    if (rightCorner.second == TrackProcessing::Corner::CCW) {
        makeSmallDot(bmp, rightCorner.first[rightCorner.first.size() - CORNER_INTERVAL -1]);
        std::cout << "Switch to LOOP_TO_NOR_R" << std::endl;
        return;// SWITCH_STATE(State::LOOP_TO_NOR_R);
    }
    return;
}

void LOOP_TO_NOR_R(GrayBMP &bmp) {
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-0.5, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, 1)));
    makeSmallDot(bmp, rightSearchPoint);
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(-1, 0));
    viic leftDownEdge = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(1, 0));
    vii leftEdge;
    leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    auto &edge = leftEdge;
    showEdge(bmp, leftEdge);
    auto midline = subSimplePathPlan(edge, 1);
    if (isBoundary(rightSearchPoint, 10))
        return;
    viic rightCorner = TrackProcessing::goUntilCorner_LE(rightSearchPoint, mp(1, 0));
    //viic rightdd = goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    //showEdge(bmp, rightdd.first);
    //showEdge(bmp, rightCorner.first);
    std::cout << "Size:" << rightCorner.first.size() << std::endl;
    if (isBoundary(rightCorner.first.back(), 10)) {
        std::cout << "Switch to NORMAL" << std::endl;
        return;// SWITCH_STATE(State::NORMAL);
    }
    return;
}





void PRE_LOOP_L(GrayBMP &bmp) {
    ii leftEdgeSearPoint = TrackProcessing::goUntilObstacle(checkPoint, TrackProcessing::normalizeVector(mp(-0.1, -1.2)));
    ii rightEdgeSearPoint = TrackProcessing::goUntilObstacle(checkPoint, TrackProcessing::normalizeVector(mp(-0.1, 1.2)));
    makeSmallDot(bmp, checkPoint);
    makeSmallDot(bmp, leftEdgeSearPoint);
    makeSmallDot(bmp, rightEdgeSearPoint);
    viic rightUpEdge = TrackProcessing::goUntilCorner_LE(rightEdgeSearPoint, mp(-1, 0));
    viic rightDownEdge = TrackProcessing::goUntilCorner_LE(rightEdgeSearPoint, mp(1, 0));
    vii rightEdge;
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.reserve(rightUpEdge.first.size() + rightDownEdge.first.size());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    viic detectCorner;
    detectCorner.second = Corner::NONE;
    if (!isBoundary(leftEdgeSearPoint, 10)) {
        detectCorner = TrackProcessing::goUntilCorner_LE(leftEdgeSearPoint, mp(-1, 0));
        makeSmallDot(bmp, detectCorner.first[detectCorner.first.size() - 1]);
        if (TrackProcessing::isActuallyCorner(detectCorner.second))
            std::cout << "Corner on the left detected" << std::endl;
    }
    if (isBoundary(leftEdgeSearPoint, 10) || (TrackProcessing::isActuallyCorner(detectCorner.second) 
    && !isBoundary(detectCorner.first[detectCorner.first.size() - 1 - CORNER_INTERVAL], 20))) {
        showEdge(bmp, rightEdge);
        auto midline = subSimplePathPlan(rightEdge, 0);
        return;
    }
    viic leftUpEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSearPoint, mp(-1, 0));
    viic leftDownEdge = TrackProcessing::goUntilCorner_LE(leftEdgeSearPoint, mp(1, 0));
    vii leftEdge;
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.reserve(leftUpEdge.first.size() + leftDownEdge.first.size());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    float min = getCoords(leftEdge[0]).second;
    int counter = 0;
    for (int i = 1; i < leftEdge.size(); ++i) {
        if (getCoords(leftEdge[i]).second < min) {
            min = getCoords(leftEdge[i]).second;
            counter = i;
        }
    } 
    vii adjustedLeftEdge;
    adjustedLeftEdge.reserve(leftEdge.size() - counter);
    adjustedLeftEdge.insert(adjustedLeftEdge.end(), leftEdge.begin() + counter, leftEdge.end());
    showEdge(bmp, adjustedLeftEdge);
    std::cout << "Adjusted size; " << adjustedLeftEdge.size() << std::endl;
    auto midline = subSimplePathPlan(adjustedLeftEdge, 1);
    std::cout << "Switch to INTO_LOOP_L" << std::endl;
    return; //SWITCH_STATE(State::INTO_LOOP_L);
}

void INTO_LOOP_L(GrayBMP &bmp) {
    ii shiftedCheckPoint(checkPoint.first + shift_y_L, checkPoint.second + shift_x_L);
    makeSmallDot(bmp, shiftedCheckPoint);
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(shiftedCheckPoint, normalizeVector(mp(0.2, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(shiftedCheckPoint, normalizeVector(mp(-0.5, -0.4)));
    makeSmallDot(bmp, rightSearchPoint);
    viic rightDownEdge = TrackProcessing::goUntilCorner_LE(rightSearchPoint, mp(0, 1));
    viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    vii rightEdge;
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    showEdge(bmp, rightEdge);
    viic leftDownEdge, leftUpEdge;
    if (!isBoundary(leftSearchPoint, 10)) {
        leftDownEdge = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(1, 0));
        leftUpEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(0, -1));
    }
    vii leftEdge;
    leftEdge.reserve(leftDownEdge.first.size() + leftUpEdge.first.size());
    std::reverse(leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftDownEdge.first.begin(), leftDownEdge.first.end());
    leftEdge.insert(leftEdge.end(), leftUpEdge.first.begin(), leftUpEdge.first.end());
    auto &left = leftEdge;
    showEdge(bmp, leftEdge);
    if (leftEdge.size()) {
        float min = getCoords(leftEdge[0]).second;
        int counter = 0;
        for (int i = 0; i < leftEdge.size(); ++i) {
            if (getCoords(leftEdge[i]).second < min) {
                min = getCoords(leftEdge[i]).second;
                counter = i;
            }
        }
        vii adjustedLeftEdge;
        adjustedLeftEdge.reserve(leftEdge.size() - counter);
        adjustedLeftEdge.insert(adjustedLeftEdge.end(), leftEdge.begin() + counter, leftEdge.end());
        left = adjustedLeftEdge;   
    }
    auto &right = rightEdge;
    auto midline = simplePathPlan(left, right);
    if (!TrackProcessing::isActuallyCorner(rightDownEdge.second)) {
        std::cout << "SWitch to IN_LOOP_L" << std::endl;
        return; //SWITCH_STATE(State::IN_LOOP_L);
    }
}

void IN_LOOP_L(GrayBMP &bmp) {
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(bottomPoint, normalizeVector(mp(-0.2, 1)));
    ii rightSearchPoint  = TrackProcessing::goUntilObstacle(bottomPoint, normalizeVector(mp(0.2, 1)));
    makeSmallDot(bmp, bottomPoint);
    makeSmallDot(bmp, leftSearchPoint);
    makeSmallDot(bmp, rightSearchPoint);
    viic rightEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, -1));
    if (TrackProcessing::isActuallyCorner(rightEdge.second) && rightEdge.first[rightEdge.first.size() - CORNER_INTERVAL - 1].first > Image::HEIGHT / 2) {
        vii newRightEdge;
        newRightEdge.reserve(rightEdge.first.size() - CORNER_INTERVAL);
        newRightEdge.insert(newRightEdge.end(), rightEdge.first.begin(), rightEdge.first.end() - CORNER_INTERVAL);
        showEdge(bmp, newRightEdge);
        auto &right = newRightEdge;
        auto midline = subSimplePathPlan(right, 0);
        std::cout << "Switch tp OUT_LOOP_L" << std::endl;
        return; //SWITCH_STATE(State::OUT_LOOP_L);
    }
    if (isBoundary(leftSearchPoint, 10)) {
        vii adjustedRightEdge;
        adjustedRightEdge.reserve(rightEdge.first.size() - CORNER_INTERVAL);
        adjustedRightEdge.insert(adjustedRightEdge.end(), rightEdge.first.begin(), rightEdge.first.end() - CORNER_INTERVAL);
        auto& edge = adjustedRightEdge;
        auto midline = subSimplePathPlan(edge, 0);
        showEdge(bmp, rightEdge.first);
        return;
    }
    viic leftEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, TrackProcessing::normalizeVector(mp(-1, 0)));
    vii adjustedRightEdge;
    adjustedRightEdge.reserve(rightEdge.first.size() - CORNER_INTERVAL);
    adjustedRightEdge.insert(adjustedRightEdge.end(), rightEdge.first.begin(), rightEdge.first.end() - CORNER_INTERVAL);
    auto &left = leftEdge.first;
    auto &right = adjustedRightEdge;
    showEdge(bmp, leftEdge.first);
    showEdge(bmp, adjustedRightEdge);
    auto midline = simplePathPlan(left, right);
    return;
}

void OUT_LOOP_L(GrayBMP &bmp) {
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(outLoopPoint, TrackProcessing::normalizeVector(mp(-1, 0.4)));
    if (isBoundary(rightSearchPoint, 10)) {
        ii searchPoint = goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-1, 0.4)));
        makeSmallDot(bmp, searchPoint);
        viic rightDownEdge = TrackProcessing::goUntilCorner_RE(searchPoint, mp(0, 1));
        viic rightUpEdge = TrackProcessing::goUntilCorner_RE(searchPoint, mp(0, -1));
        vii rightEdge;
         rightEdge.reserve(rightDownEdge.first.size() + rightUpEdge.first.size());
         std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
         rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
         rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
         auto &edge = rightEdge;
         auto midline = subSimplePathPlan(edge, 0);
         std::cout << "SWtich tp END_LOOP_L" << std::endl;
         return; //SWITCH_STATE(State::END_LOOP_L);
    }
    viic rightCorner = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    if (!TrackProcessing::isActuallyCorner(rightCorner.second)) {
        ii searchPoint = goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-1, 0.4)));
        viic rightUpEdge = TrackProcessing::goUntilCorner_RE(searchPoint, mp(-1, -1));
        auto &edge = rightUpEdge.first;
        showEdge(bmp, rightUpEdge.first);
        auto midline = subSimplePathPlan(edge, 0);
        std:;cout << "Switch to END_LOOP_L" << std::endl;
        return; //SWITCH_STATE(State::END_LOOP_L);
    }
    ii searchPoint = rightCorner.first.back();
    makeSmallDot(bmp, searchPoint);
    ii edgePoint = TrackProcessing::goUntilObstacle(searchPoint, TrackProcessing::normalizeVector(mp(-1, 0)));
    viic rightLeftEdge = goUntilCorner_RE(edgePoint, mp(0, -1));
    viic rightRightEdge = goUntilCorner_LE(edgePoint, mp(0, 1));
    vii rightEdge;
    rightEdge.reserve(rightLeftEdge.first.size() + rightRightEdge.first.size());
    std::reverse(rightRightEdge.first.begin(), rightRightEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightRightEdge.first.begin(), rightRightEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightLeftEdge.first.begin(), rightLeftEdge.first.end());
    auto &edge = rightEdge;
    showEdge(bmp, rightEdge);
    auto midline = subSimplePathPlan(edge, 0);
    return;
}

void END_LOOP_L(GrayBMP &bmp) {
    makeSmallDot(bmp, endLoopPoint);
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, 1)));
    makeSmallDot(bmp, leftSearchPoint);
    makeSmallDot(bmp, rightSearchPoint);
    viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    viic rightDownEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(1, 0));
    vii rightEdge;
    rightEdge.reserve(rightUpEdge.first.size() + rightDownEdge.first.size());
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    showEdge(bmp, rightEdge);
    auto &edge = rightEdge;
    auto midline = subSimplePathPlan(edge, 0);
    if (isBoundary(leftSearchPoint, 10))
        return;
    viic leftCorner = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(1, 0));
    if (leftCorner.second == TrackProcessing::Corner::CW) {
        std::cout << "Switch to LOOP_TO_NOR_L" << std::endl;
        return;// SWITCH_STATE(State::LOOP_TO_NOR_L);
    }
    return;
}

void LOOP_TO_NOR_L(GrayBMP &bmp) {
    ii leftSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(0, -1)));
    ii rightSearchPoint = TrackProcessing::goUntilObstacle(endLoopPoint, TrackProcessing::normalizeVector(mp(-0.5, 1)));
    makeSmallDot(bmp, rightSearchPoint);
    viic rightUpEdge = TrackProcessing::goUntilCorner_RE(rightSearchPoint, mp(-1, 0));
    viic rightDownEdge = TrackProcessing::goUntilCorner_LE(leftSearchPoint, mp(1, 0));
    vii rightEdge;
    rightEdge.reserve(rightUpEdge.first.size() + rightDownEdge.first.size());
    std::reverse(rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightDownEdge.first.begin(), rightDownEdge.first.end());
    rightEdge.insert(rightEdge.end(), rightUpEdge.first.begin(), rightUpEdge.first.end());
    auto &edge = rightEdge;
    showEdge(bmp, rightEdge);
    auto midline = subSimplePathPlan(edge, 0);
    if (isBoundary(leftSearchPoint ,10))
        return;
    viic leftCorner = TrackProcessing::goUntilCorner_RE(leftSearchPoint, mp(1, 0));
    if (isBoundary(leftCorner.first.back(), 10)) {
        std::cout << "Switch to NORMAL" << std::endl;
        return;//SWITCH_STATE(State::NORMAL);
    }
    return;
}

// void SWITCH_STATE(State nextState) {
// 	//Message::Status(getStateStr(nextState)).send();
// 	switch (STATE) {
// 		case State::PRE_LOOP_L:
//             PRE_LOOP_L();
//             break;
//         case State::INTO_LOOP_L:
//             INTO_LOOP_L();
//             break;
//         case State::IN_LOOP_L:
//             INTO_LOOP_L();
//             break;
//         case State::OUT_LOOP_L:
//             OUT_LOOP_L();
//             break;
//         case State::END_LOOP_L:
//             END_LOOP_L();
//             break;
//         case State::LOOP_TO_NOR_L:
//             LOOP_TO_NOR_L();
//             break;
//         case State::PRE_LOOP_R:
//             PRE_LOOP_R();
//             break;
//         case State::INTO_LOOP_R:
//             INTO_LOOP_R();
//             break;
//         case State::IN_LOOP_R:
//             IN_LOOP_R();
//             break;
//         case State::OUT_LOOP_R:
//             OUT_LOOP_R();
//             break;
//         case State::END_LOOP_R:
//             END_LOOP_R();
//             break;
//         case State::LOOP_TO_NOR_R:
//             LOOP_TO_NOR_R();
//             break;
//         default:
//             std::cout << "NOT IN ANY STATE" << std::endl;
// 	}
// 	STATE = nextState;
// 	switch (STATE) {
// 		case State::PRE_LOOP_L:
//             PRE_LOOP_L();
//             break;
//         case State::INTO_LOOP_L:
//             INTO_LOOP_L();
//             break;
//         case State::IN_LOOP_L:
//             INTO_LOOP_L();
//             break;
//         case State::OUT_LOOP_L:
//             OUT_LOOP_L();
//             break;
//         case State::END_LOOP_L:
//             END_LOOP_L();
//             break;
//         case State::LOOP_TO_NOR_L:
//             LOOP_TO_NOR_L();
//             break;
//         case State::PRE_LOOP_R:
//             PRE_LOOP_R();
//             break;
//         case State::INTO_LOOP_R:
//             INTO_LOOP_R();
//             break;
//         case State::IN_LOOP_R:
//             IN_LOOP_R();
//             break;
//         case State::OUT_LOOP_R:
//             OUT_LOOP_R();
//             break;
//         case State::END_LOOP_R:
//             END_LOOP_R();
//             break;
//         case State::LOOP_TO_NOR_R:
//             LOOP_TO_NOR_R();
//             break;
//         default:
//             std::cout << "NOT IN ANY STATE" << std::endl;
// 	}
// }
