#pragma once
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <iostream>

class vec3;

class qtrn {
public:
	float t, x, y, z;

	qtrn();
	qtrn(float t, float x, float y, float z);
	qtrn(float angle, const vec3& axis);

	qtrn& clean();

	void toAngleAxis(float& theta, vec3& axis) const;
	void printAngleAxis(std::string& s) const;

	bool operator== (const qtrn& q) const;
	bool operator!= (const qtrn& q) const;
	
	qtrn& normalizeInPlace();
	qtrn& operator=  (const qtrn& q);
	qtrn& operator+= (const qtrn& q);
	qtrn& operator-= (const qtrn& q);
	qtrn& operator*= (const qtrn& q);
	qtrn& operator*= (float f);

	qtrn operator+ (const qtrn& q) const;
	qtrn operator- (const qtrn& q) const;
	qtrn operator* (const qtrn& q) const;
	qtrn operator* (float f) const;

	friend qtrn operator* (float f, const qtrn& q);

	friend std::ostream& operator<< (std::ostream& os, const qtrn& q);

	friend const qtrn conjugate(const qtrn& q);
	friend const qtrn inverse(const qtrn& q);
	friend const qtrn normalize(const qtrn& q);
	friend const qtrn lerp(const qtrn& q0, const qtrn& q1, float k);
	friend const qtrn slerp(const qtrn& q0, const qtrn& q1, float k);
	friend const float length(const qtrn& q);
	friend const float quadrance(const qtrn& q);
};

#endif