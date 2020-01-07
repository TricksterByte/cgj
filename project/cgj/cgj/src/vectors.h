#pragma once
#ifndef __VECTORS__
#define __VECTORS__

#include <iostream>

class vec2;
class vec3;
class vec4;

class vec2 {
public:
	float x, y;

	vec2();
	vec2(float n);
	vec2(float x, float y);
	vec2(const vec2& v);

	const float* data() const;
	vec2& clean();

	bool isZero() const;
	bool isUnitVector() const;

	bool operator== (const vec2& v) const;
	bool operator!= (const vec2& v) const;

	vec2& zero();
	vec2& normalizeInPlace();
	vec2& operator=  (const vec2& v); // vector = vector
	vec2& operator+= (const vec2& v); // vector += vector
	vec2& operator-= (const vec2& v); // vector -= vector
	vec2& operator*= (float f);       // vector *= float
	vec2& operator/= (float f);       // vector /= float

	vec2 operator+ (const vec2& v) const; // vector - vector
	vec2 operator- (const vec2& v) const; // vector + vector
	vec2 operator- () const;              // -vector
	vec2 operator* (float f) const;       // vector * float
	vec2 operator/ (float f) const;       // vector / float

	friend vec2 operator* (float f, const vec2& v); // float * vector

	friend std::istream& operator>> (std::istream& is, vec2& v);
	friend std::ostream& operator<< (std::ostream& os, const vec2& v);

	friend vec2 orthogonal(const vec2& v);
	friend vec2 normalize(const vec2& v);
	friend const float dot(const vec2& v, const vec2& other);
	friend const float distance(const vec2& v, const vec2& other);
	friend const float angle(const vec2& v, const vec2& other);
	friend const float length(const vec2& v);
	friend const float quadrance(const vec2& v);
};

class vec3 {
public:
	float x, y, z;

	vec3();
	vec3(float n);
	vec3(float x, float y, float z);
	vec3(const vec3& v);
	vec3(const vec4& v);

	const float* data() const;
	vec3& clean();
	
	bool isZero() const;
	bool isUnitVector() const;

	bool operator== (const vec3& v) const;
	bool operator!= (const vec3& v) const;

	vec3& zero();
	vec3& normalizeInPlace();
	vec3& operator=  (const vec3& v);
	vec3& operator+= (const vec3& v);
	vec3& operator-= (const vec3& v);
	vec3& operator*= (float f);
	vec3& operator/= (float f);

	vec3 operator+ (const vec3& v) const;
	vec3 operator- (const vec3& v) const;
	vec3 operator- () const;
	vec3 operator* (float f) const;
	vec3 operator/ (float f) const;

	friend vec3 operator* (float f, const vec3& v);

	friend std::istream& operator>> (std::istream& is, vec3& v);
	friend std::ostream& operator<< (std::ostream& os, const vec3& v);

	friend vec3 normalize(const vec3& v);
	friend vec3 cross(const vec3& v, const vec3& other);
	friend const float dot(const vec3& v, const vec3& other);
	friend const float distance(const vec3& v, const vec3& other);
	friend const float angle(const vec3& v, const vec3& other);
	friend const float length(const vec3& v);
	friend const float quadrance(const vec3& v);
};

class vec4 {
public:
	float x, y, z, w;

	vec4();
	vec4(float n);
	vec4(float x, float y, float z);
	vec4(float x, float y, float z, float w);
	vec4(const vec3& v);
	vec4(const vec4& v);

	const float* data() const;
	vec4& clean();

	bool operator== (const vec4& v) const;
	bool operator!= (const vec4& v) const;

	friend std::istream& operator>> (std::istream& is, vec4& v);
	friend std::ostream& operator<< (std::ostream& os, const vec4& v);
};

const vec3 AXIS_X = vec3(1, 0, 0);
const vec3 AXIS_Y = vec3(0, 1, 0);
const vec3 AXIS_Z = vec3(0, 0, 1);

#endif