#pragma once
#ifndef __MATRICES__
#define __MATRICES__

#include <iostream>

class vec2;
class vec3;
class vec4;

class mat2;
class mat3;
class mat4;

class mat2 {
public:
	const int size = 2;
	float data[4] = { 0.f };

	mat2();
	mat2(float n);
	mat2(float m00, float m01,
		float m10, float m11);
	mat2(const mat2& m);

	mat2& clean();

	bool operator== (const mat2& m) const;
	bool operator!= (const mat2& m) const;
	float at(int r, int c) const;
	float& at(int r, int c);

	mat2& operator=  (const mat2& m);
	mat2& operator+= (const mat2& m);
	mat2& operator-= (const mat2& m);
	mat2& operator*= (float f);
	mat2& operator/= (float f);

	mat2 operator+ (const mat2& m) const;
	mat2 operator- (const mat2& m) const;
	mat2 operator* (const mat2& m) const;
	vec2 operator* (const vec2& v) const;
	mat2 operator* (float f) const;
	mat2 operator/ (float f) const;

	friend mat2 operator* (float f, const mat2& m);

	friend std::istream& operator>> (std::istream& is, mat2& m);
	friend std::ostream& operator<< (std::ostream& os, const mat2& m);

	friend float determinant(const mat2& m);
	friend mat2 transpose(const mat2& m);
	friend mat2 inverse(const mat2& m);
};

class mat3 {
public:
	const int size = 3;
	float data[9] = { 0.f };

	mat3();
	mat3(float n);
	mat3(float m00, float m01, float m02, 
		float m10, float m11, float m12, 
		float m20, float m21, float m22);
	mat3(const mat3& m);

	mat3& clean();

	bool operator==  (const mat3& m) const;
	bool operator!=  (const mat3& m) const;
	float at(int r, int c) const;
	float& at(int r, int c);

	mat3& operator=  (const mat3& m);
	mat3& operator+= (const mat3& m);
	mat3& operator-= (const mat3& m);
	mat3& operator*= (float f);
	mat3& operator/= (float f);

	mat3 operator+ (const mat3& m) const;
	mat3 operator- (const mat3& m) const;
	mat3 operator* (const mat3& m) const;
	vec3 operator* (const vec3& v) const;
	mat3 operator* (float f) const;
	mat3 operator/ (float f) const;

	friend mat3 operator* (float f, const mat3& m);

	friend std::istream& operator>> (std::istream& is, mat3& m);
	friend std::ostream& operator<< (std::ostream& os, const mat3& m);

	friend float determinant(const mat3& m);
	friend mat3 transpose(const mat3& m);
	friend mat3 inverse(const mat3& m);

	//void adjugate(mat3& m) const;
	//float minors(mat2& m, int r, int c) const;
};

class mat4 {
public:
	const int size = 4;
	float data[16] = { 0.f };

	mat4();
	mat4(float n);
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	mat4(const mat4& m);

	mat4& clean();

	bool operator== (const mat4& m) const;
	bool operator!= (const mat4& m) const;
	float at(int r, int c) const;
	float& at(int r, int c);

	mat4& operator=  (const mat4& m);
	mat4& operator+= (const mat4& m);
	mat4& operator-= (const mat4& m);
	mat4& operator*= (float f);
	mat4& operator/= (float f);

	mat4 operator+ (const mat4& m) const;
	mat4 operator- (const mat4& m) const;
	mat4 operator* (const mat4& m) const;
	vec4 operator* (const vec4& v) const;
	mat4 operator* (float f) const;
	mat4 operator/ (float f) const;

	friend mat4 operator* (float f, const mat4& m);

	friend std::ostream& operator<< (std::ostream& os, const mat4& m);

	friend mat4 transpose(const mat4& m);
	friend void convertToColumnMajor(const mat4& m, float glFloat[]);
	friend float* convertToColumnMajor(const mat4& m);
};

#endif