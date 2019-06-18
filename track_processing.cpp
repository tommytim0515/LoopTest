#include "track_processing.hpp"
#include "global.hpp"

using namespace std;

namespace TrackProcessing {

	ii getCcw(ii a) { return mp(-a.second, a.first); }
	ii getCw(ii a) { return mp(a.second, -a.first); }
	ff IMAGE_Ccw(ff a) { return mp(-a.second, a.first); }
	ff IMAGE_Cw(ff a) { return mp(a.second, -a.first); }
	ff MATH_Ccw(ff a) { return mp(a.second, -a.first); }
	ff MATH_Cw(ff a) { return mp(-a.second, a.first); }
	ii goUntilObstacle(ii start, ff direction) {
		direction = normalizeVector(direction);
		if (std::isnan(direction.first) || std::isnan(direction.second)) ERROR("NaN direction");
		float curY = start.first;
		float curX = start.second;
	    while ( ! Image::isEdge(CAM_BUFFER, curY + direction.first, curX + direction.second)) curY += direction.first, curX += direction.second;
	    return mp((int)curY, (int)curX);
	}
	ii goUntilObstacle(ii start, ii direction) {
		return goUntilObstacle(start, mp((float)direction.first, (float)direction.second));
	}
	bool onImageBorder(ii x) {
		return x.first <= 1 || x.first >= Image::HEIGHT - 2 || x.second <= 1 || x.second >= Image::WIDTH - 2;
	}
	bool outsideImage(ii x) {
		return x.first < 0 || x.first >= Image::HEIGHT || x.second < 0 || x.second >= Image::WIDTH;
	}
	float normalizeAngle(float angle) {
		angle = fmod(angle, 360.0f);
		if (angle < -180.0f) return angle + 360.0f;
		else if (angle > 180.0f) return angle - 360.0f;
		else return angle;
	}
	ff normalizeVector(ff vec) {
	    float mag = sqrt(vec.first * vec.first + vec.second * vec.second);
		return vec / mag;
	}
	ff getCoords(ii yx) {
	    return mp(COORD[yx.first][yx.second][0], COORD[yx.first][yx.second][1]) / 10.0f + mp(3.0f, 0.0f); // 3 cm y-forward constant offset
	}
	Corner getCornerType(ii a, ii b, ii c) {
	    ff fa = getCoords(a); fa.first *= -1;
	    ff fb = getCoords(b); fb.first *= -1;
	    ff fc = getCoords(c); fc.first *= -1;
		ff ab = fb - fa, bc = fc - fb;
		float dot = ab.second * bc.second + ab.first * bc.first;
		float det = ab.second * bc.first - ab.first * bc.second;
		float angleDiff = 180 / M_PI * atan2(det, dot);
		//cerr << angleDiff << endl;
	    if (std::isnan(angleDiff)) return Corner::INVALID; // NaN can happen due to unmappable coords
		if (abs(angleDiff) < 60.0f) return Corner::NONE; // too smooth to be a corner
		// 75 was too high, also this value also depends on corner interval
	
		//float angleFirst = 180 / M_PI * atan2(ab.first, ab.second);
		//float angleMidRotation = normalizeAngle(angleFirst + angleDiff / 2.0f);
	
		bool ccw = (ab.first * bc.second - ab.second * bc.first > 0); // whether counterclockwise
		/*if (angleMidRotation >= 0 && angleMidRotation < 90) return ccw ? Corner::L : Corner::BR;
		else if (angleMidRotation >= 90) return ccw ? Corner::R : Corner::BL;
		else if (angleMidRotation < 0 && angleMidRotation >= -90) return ccw ? Corner::BL : Corner::R;
		else if (angleMidRotation < -90) return ccw ? Corner::BR : Corner::L;
		else ERROR("UNEXPECTED CRNR");
		return Corner::INVALID;
		// i hope i didnt make any error here lol*/
		return ccw ? Corner::CCW : Corner::CW;
	}
	bool isActuallyCorner(Corner c) {
		return (int)c >= 1 && (int)c <= 2;
	}
	viic goUntilCorner_LE(ii current, ii direction, int limit) {
		vii ret;
		while (current.first > HEIGHT_LIMIT && limit--) {
			ret.push_back(current);
		
			if (ret.size() > 2*CORNER_INTERVAL) {
				Corner c = getCornerType(ret[ret.size()-1-2*CORNER_INTERVAL], ret[ret.size()-1-CORNER_INTERVAL], ret[ret.size()-1]);
				if (c != Corner::NONE) return mp(std::move(ret), c);
			}
		
			ii frontCell = current + direction;
			ii leftCell = current + getCcw(direction);
			if ( ! Image::isEdge(CAM_BUFFER, leftCell)) {
				direction = getCcw(direction);
				frontCell = current + direction;
				leftCell = current + getCcw(direction);
			}
			if (Image::isEdge(CAM_BUFFER, frontCell)) {
				direction = getCw(direction);
				frontCell = current + direction;
				leftCell = current + getCcw(direction);
			}
			if (Image::isEdge(CAM_BUFFER, frontCell)) {
				direction = getCw(direction);
				frontCell = current + direction;
				leftCell = current + getCcw(direction);
			}
			current += direction;

			if (onImageBorder(current)) break;
			if (isnan(getCoords(current).first) || isnan(getCoords(current).second)) break;
		}
		return mp(std::move(ret), Corner::NONE);
	}
	viic goUntilCorner_RE(ii current, ii direction, int limit) {
		vii ret;
		while (current.first > HEIGHT_LIMIT && limit--) {
			ret.push_back(current);
		
			if (ret.size() > 2*CORNER_INTERVAL) {
				Corner c = getCornerType(ret[ret.size()-1-2*CORNER_INTERVAL], ret[ret.size()-1-CORNER_INTERVAL], ret[ret.size()-1]);
				if (c != Corner::NONE) return mp(std::move(ret), c);
			}
		
			ii frontCell = current + direction;
			ii rightCell = current + getCw(direction);

			if ( ! Image::isEdge(CAM_BUFFER, rightCell)) {
				direction = getCw(direction);
				frontCell = current + direction;
				rightCell = current + getCw(direction);
			}
			if (Image::isEdge(CAM_BUFFER, frontCell)) {
				direction = getCcw(direction);
				frontCell = current + direction;
				rightCell = current + getCw(direction);
			}
			if (Image::isEdge(CAM_BUFFER, frontCell)) {
				direction = getCcw(direction);
				frontCell = current + direction;
				rightCell = current + getCw(direction);
			}
		
			current += direction;

			if (onImageBorder(current)) break;
			if (isnan(getCoords(current).first) || isnan(getCoords(current).second)) break;
		}
		return mp(std::move(ret), Corner::NONE);
	}
	bool checkZebraLine() {
		const int Y = Image::HEIGHT * 5 / 8;
		const int startX = Image::WIDTH * 1 / 8;
		const int endX = Image::WIDTH * 7 / 8;
		const int scanjump = 5;
		int cnt = 0;
		for (int i = startX + scanjump; i < endX; i += scanjump)
			cnt += (Image::isEdge(CAM_BUFFER, Y, i-scanjump)) != (Image::isEdge(CAM_BUFFER, Y, i));
		return cnt >= 5;
	}
	const int CAR_WIDTH = 16;
	inline int cross(const ii &a, const ii &b) { return a.first * b.second - a.second * b.first; }
	ff amrutSelector(vff midline) {
		for (auto it = midline.end()-1; it != midline.begin(); it--) {
			auto compute = [&](const ii &a){ return abs(cross(a, *it)); };
			auto compare = [&](const ii &a, const ii &b){ return compute(a) < compute(b); };
			// std hypot impl is slow //float maxValue = compute(*std::max_element(midline.begin(), it, compare)) / std::hypot(it->first, it->second);
			float maxValue = compute(*std::max_element(midline.begin(), it, compare)) / sqrt(it->first * it->first + it->second * it->second);
			if (maxValue < CAR_WIDTH/2.0f) return *it;
		}
		return ff(1, 0);
	}
	void filterMidline(vff &midline) {
		int firstPositiveIndex = midline.size();
		for (int i = 0; i < midline.size(); i++) {
			if (midline[i].first > 0) {
				firstPositiveIndex = i;
				break;
			}
		}
		midline.erase(midline.begin(), midline.begin()+firstPositiveIndex);
	}
	vff getMidlineFromLeft(const vii &leftEdge) {
		vff midline;
		for (int i = CORNER_INTERVAL; i+CORNER_INTERVAL+CORNER_INTERVAL < leftEdge.size(); i++) {
			ff a = getCoords(leftEdge[i-CORNER_INTERVAL]);
			ff b = getCoords(leftEdge[i]);
			ff c = getCoords(leftEdge[i+CORNER_INTERVAL]);
			ff ac = c - a;
			ff perp_ac = MATH_Cw(ac);
			midline.push_back( b + normalizeVector(perp_ac) * 19.0f );
		}
		filterMidline(midline);
		if (midline.size() == 0) midline.push_back(mp(1, 0));
		return midline;
	}
	vff getMidlineFromRight(const vii &rightEdge) {
		vff midline;
		for (int i = CORNER_INTERVAL; i+CORNER_INTERVAL+CORNER_INTERVAL < rightEdge.size(); i++) {
			ff a = getCoords(rightEdge[i-CORNER_INTERVAL]);
			ff b = getCoords(rightEdge[i]);
			ff c = getCoords(rightEdge[i+CORNER_INTERVAL]);
			ff ac = c - a;
			ff perp_ac = MATH_Ccw(ac);
			midline.push_back( b + normalizeVector(perp_ac) * 19.0f );
		}
		filterMidline(midline);
		if (midline.size() == 0) midline.push_back(mp(1, 0));
		return midline;
	}
	
}