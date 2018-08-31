#pragma once
#include "Package.h"
enum Priority { LOW_PRIORITY = 0, MEDIUM_PRIORITY, HIGH_PRIORITY, URGENT_PRIORITY };
enum Propagation { LOW_PROPAGATION = 0, MEDIUM_PROPAGATION, HIGH_PROPAGATION, URGENT_PROPAGATION };
class PackageWraper
{
	Package package;
	int priority;
	int propegation;
	bool active;
public:
	inline PackageWraper() 
	{ 
		setPriority(MEDIUM_PRIORITY);
		setPropegation(MEDIUM_PROPAGATION);
		active = false;
	}
	inline PackageWraper(Package& p) {
		package = p;
		setPriority(p);
		setPropegation(p);
		setActive(true);
	}

	inline void setPriority(Package p) {
		switch (p.opcodeAndPriority.b) {
		case LOW_PRIORITY:
			priority = 5;
			break;
		case MEDIUM_PRIORITY:
			priority = 3;
			break;
		case HIGH_PRIORITY:
			priority = 2;
			break;
		case URGENT_PRIORITY:
			priority = 1;
			break;
		}
	}

	inline void setPropegation(Package p) {
		switch (p.opcodeAndPriority.c) {
		case LOW_PROPAGATION:
			propegation = 5;
			break;
		case MEDIUM_PROPAGATION:
			propegation = 10;
			break;
		case HIGH_PROPAGATION:
			propegation = 15;
			break;
		case URGENT_PROPAGATION:
			propegation = 20;
			break;
		}
	}

	inline void setPropegation(int prop) {
		propegation = prop;
	}

	inline void setPriority(int prio) {
		priority = prio;
	}

	inline int getPriority() {
		return priority;
	}

	inline int getPropegation() {
		return propegation;
	}

	inline Package& getPackage() {
		return package;
	}

	inline bool isActive() {
		return active;
	}

	inline void setActive(bool activeFlag) {
		active = activeFlag;
	}

	inline Package operator=(Package a) {
		package = a;
		setPriority(a);
		setPropegation(a);
		active = true;
		return a;
	}

	inline PackageWraper operator=(PackageWraper a) {
		package = a.getPackage();
		propegation = a.getPropegation();
		priority = a.getPriority();
		active = a.isActive();
		return a;
	}

	inline bool operator==(PackageWraper _package) {
		bool status = false;
		if (package == _package.getPackage() && priority == _package.getPriority() && propegation == _package.getPropegation() && active == _package.isActive()) {
			status = true;
		}
		return status;
	}

	inline bool operator==(Package _package) {
		bool status = false;
		if (package == _package) {
			status = true;
		}
		return status;
	}

	~PackageWraper() {}
};