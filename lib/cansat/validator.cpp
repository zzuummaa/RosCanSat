/*
 * validator.cpp
 *
 *  Created on: 13 ���. 2017 �.
 *      Author: Cntgfy
 */

#include "validator.h"
#include "limits.h"

#include <chrono>

using namespace std::chrono;

/**
 * return current time in milliseconds
 */
int getCurTime() {
	milliseconds curTime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	return curTime.count();
}

validator::validator() {
}


validator::~validator() {
	for (std::map<int, telemetryConditions*>::iterator it=telCons.begin(); it!=telCons.end(); ++it) {
		delete it->second;
	}
}

void validator::addType(int tel_type) {
	if (telCons.find(tel_type) != telCons.end()) {
		cerr << "Error: telemetry with type " << tel_type << " already exist" << endl;
		exit(0);
	}

	telCons[tel_type] = new telemetryConditions();
}

telemetryConditions* validator::getWithType(int tel_type) {
	std::map<int, telemetryConditions*>::iterator iter = telCons.find(tel_type);
	if (iter == telCons.end()) {
		cerr << "Error: element with type " << tel_type << " not found" << endl;
		exit(0);
	}

	return iter->second;
}

telemetryConditions::telemetryConditions() {
	maxWaitTimeMS = INT_MAX;
	lastTimeMS = getCurTime();
}

telemetryConditions::~telemetryConditions() {
}

void telemetryConditions::setFieldDiapason(string field, double begin, double end) {
	this->begin[field] = begin;
	this->end[field]   = end;
}

bool telemetryConditions::checkFieldDiapason(string field, double val) {
	if (begin.find(field) == begin.end()) {
		cerr << "Error: field '" << field << "', and not found" << endl;
		exit(0);
	}

	if (begin[field] < val && val < end[field]) {
		return true;
	} else {
		return false;
	}
}

void telemetryConditions::setMaxWaitTime(int ms) {
	maxWaitTimeMS = ms;
}

bool telemetryConditions::checkWaitTime() {
	int waitTimeMS;
	return checkWaitTime(&waitTimeMS);
}

bool telemetryConditions::checkWaitTime(int* waitTimeMS) {
	int curTimeMS = getCurTime();
	*waitTimeMS = curTimeMS - lastTimeMS;

	bool flag = *waitTimeMS < maxWaitTimeMS;

	lastTimeMS = curTimeMS;

	return flag;
}