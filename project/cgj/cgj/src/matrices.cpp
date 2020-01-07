#include "matrices.h"
#include "vectors.h"
#include "math_helpers.h"

#include <cmath>
#include <iomanip>
#include <exception>

/****************
*	Matrix2x2	*
*****************/

mat2::mat2() {}

mat2::mat2(float n) {
	data[0] = data[3] = n;
}

mat2::mat2(float m00, float m01,
	float m10, float m11) {
	data[0] = m00; data[1] = m01; 
	data[2] = m10; data[3] = m11;
}

mat2::mat2(const mat2& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);
}

mat2& mat2::clean() {
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			float fl = std::truncf(at(r, c));

			if (fcmp(at(r, c), fl))
				at(r, c) = fl;
		}
	}

	return *this;
}

bool mat2::operator== (const mat2& m) const {
	bool b = true;

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			b = b && fcmp(at(r, c), m.at(r, c));

	return b;
}

bool mat2::operator!= (const mat2& m) const {
	return !this->operator==(m);
}

float mat2::at(int r, int c) const {
	return data[index(size, r, c)];
}

float& mat2::at(int r, int c) {
	return data[index(size, r, c)];
}

mat2& mat2::operator= (const mat2& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);

	return *this;
}

mat2& mat2::operator+= (const mat2& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) += m.at(r, c);

	return *this;
}

mat2& mat2::operator-= (const mat2& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) -= m.at(r, c);

	return *this;
}

mat2& mat2::operator*= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) *= f;

	return *this;
}

mat2& mat2::operator/= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) /= f;

	return *this;
}

mat2 mat2::operator+ (const mat2& m) const {
	mat2 res(*this);
	res += m;

	return res;
}

mat2 mat2::operator- (const mat2& m) const {
	mat2 res(*this);
	res -= m;

	return res;
}

mat2 mat2::operator* (const mat2& m) const {
	mat2 res;
	mat2 t = transpose(m);

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			for (int k = 0; k < size; k++)
				res.at(r, c) += at(r, k) * t.at(c, k);

	return res;
}

vec2 mat2::operator* (const vec2& v) const {
	vec2 res;

	res.x = at(0, 0) * v.x + at(0, 1) * v.y;
	res.y = at(1, 0) * v.x + at(1, 1) * v.y;

	return res;
}

mat2 mat2::operator* (float f) const {
	mat2 res(*this);
	res *= f;

	return res;
}

mat2 mat2::operator/ (float f) const {
	mat2 res(*this);
	res /= f;

	return res;
}

mat2 operator* (float f, const mat2& m) {
	mat2 res(m);
	res *= f;

	return res;
}

std::istream& operator>> (std::istream& is, mat2& m) {
	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++) {
			std::cout << "Input for value at (" << r << "," << c << "): ";
			is >> m.at(r, c);
		}

	return is;
}

std::ostream& operator<< (std::ostream& os, const mat2& m) {
	std::streamsize prec = os.precision();
	os.precision(5);

	for (int r = 0; r < m.size; r++) {
		os << "[";
		for (int c = 0; c < m.size; c++)
			os << std::setw(10) << m.at(r, c);
		os << " ]" << std::endl;
	}

	os.precision(prec);

	return os;
}

float determinant(const mat2& m) {
	return m.at(0, 0) * m.at(1, 1) - m.at(0, 1) * m.at(1, 0);
}

mat2 transpose(const mat2& m) {
	mat2 res;

	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++)
			res.at(c, r) = m.at(r, c);
	
	return res;
}

mat2 inverse(const mat2& m) {
	if (fcmp(determinant(m), 0.f))
		throw std::exception("Matrix doesn't have an inverse. Determinant is 0");

	mat2 res;
	res.at(0, 0) = m.at(1, 1);
	res.at(0, 1) = -m.at(0, 1);
	res.at(1, 0) = -m.at(1, 0);
	res.at(1, 1) = m.at(0, 0);

	float det = determinant(m);
	res /= det;

	return res;
}

/****************
*	Matrix3x3	*
*****************/

mat3::mat3() {}

mat3::mat3(float n) {
	data[0] = data[4] = data[8] = n;
}

mat3::mat3(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22) {
	data[0] = m00; data[1] = m01; data[2] = m02;
	data[3] = m10; data[4] = m11; data[5] = m12;
	data[6] = m20; data[7] = m21; data[8] = m22;
}

mat3::mat3(const mat3& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);
}

mat3& mat3::clean() {
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			float fl = std::truncf(at(r, c));

			if (fcmp(at(r, c), fl))
				at(r, c) = fl;
		}
	}

	return *this;
}

bool mat3::operator== (const mat3& m) const {
	bool b = true;

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			b = b && fcmp(at(r, c), m.at(r, c));
	
	return b;
}

bool mat3::operator!= (const mat3& m) const {
	return !this->operator==(m);
}

float mat3::at(int r, int c) const {
	return data[index(size, r, c)];
}

float& mat3::at(int r, int c) {
	return data[index(size, r, c)];
}

mat3& mat3::operator= (const mat3& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);

	return *this;
}

mat3& mat3::operator+= (const mat3& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) += m.at(r, c);

	return *this;
}

mat3& mat3::operator-= (const mat3& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) -= m.at(r, c);

	return *this;
}

mat3& mat3::operator*= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) *= f;

	return *this;
}

mat3& mat3::operator/= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) /= f;

	return *this;
}

mat3 mat3::operator+ (const mat3& m) const {
	mat3 res(*this);
	res += m;

	return res;
}

mat3 mat3::operator- (const mat3& m) const {
	mat3 res(*this);
	res -= m;

	return res;
}

mat3 mat3::operator* (const mat3& m) const {
	mat3 res;
	mat3 t = transpose(m);

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			for (int k = 0; k < size; k++)
				res.at(r, c) += at(r, k) * t.at(c, k);

	return res;
}

vec3 mat3::operator* (const vec3& v) const {
	vec3 res;
	
	res.x = at(0, 0) * v.x + at(0, 1) * v.y + at(0, 2) * v.z;
	res.y = at(1, 0) * v.x + at(1, 1) * v.y + at(1, 2) * v.z;
	res.z = at(2, 0) * v.x + at(2, 1) * v.y + at(2, 2) * v.z;

	return res;
}

mat3 mat3::operator* (float f) const {
	mat3 res(*this);
	res *= f;

	return res;
}

mat3 mat3::operator/ (float f) const {
	mat3 res(*this);
	res /= f;

	return res;
}

mat3 operator* (float f, const mat3& m) {
	mat3 res(m);
	res *= f;

	return res;
}

std::istream& operator>> (std::istream& is, mat3& m) {
	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++) {
			std::cout << "Input for value at (" << r << "," << c << "): ";
			is >> m.at(r, c);
		}

	return is;
}

std::ostream& operator<< (std::ostream& os, const mat3& m) {
	std::streamsize prec = os.precision();
	os.precision(5);

	for (int r = 0; r < m.size; r++) {
		os << "[";
		for (int c = 0; c < m.size; c++)
			os << std::setw(10) << m.at(r, c);
		os << " ]" << std::endl;
	}

	os.precision(prec);

	return os;
}

float determinant(const mat3& m) {
	return m.at(0, 0) * (m.at(1, 1) * m.at(2, 2) - m.at(1, 2) * m.at(2, 1))
		 + m.at(0, 1) * (m.at(1, 2) * m.at(2, 0) - m.at(1, 0) * m.at(2, 2))
		 + m.at(0, 2) * (m.at(1, 0) * m.at(2, 1) - m.at(1, 1) * m.at(2, 0));
}

mat3 transpose(const mat3& m) {
	mat3 res;

	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++)
			res.at(c, r) = m.at(r, c);

	return res;
}

mat3 inverse(const mat3& m) {
	if (fcmp(determinant(m), 0.f))
		throw std::exception("Matrix doesn't have an inverse. Determinant is 0");

	mat3 res;
	res.at(0, 0) = m.at(1, 1) * m.at(2, 2) - m.at(1, 2) * m.at(2, 1);
	res.at(0, 1) = m.at(0, 2) * m.at(2, 1) - m.at(0, 1) * m.at(2, 2);
	res.at(0, 2) = m.at(0, 1) * m.at(1, 2) - m.at(0, 2) * m.at(1, 1);
	res.at(1, 0) = m.at(1, 2) * m.at(2, 0) - m.at(1, 0) * m.at(2, 2);
	res.at(1, 1) = m.at(0, 0) * m.at(2, 2) - m.at(0, 2) * m.at(2, 0);
	res.at(1, 2) = m.at(0, 2) * m.at(1, 0) - m.at(0, 0) * m.at(1, 2);
	res.at(2, 0) = m.at(1, 0) * m.at(2, 1) - m.at(1, 1) * m.at(2, 0);
	res.at(2, 1) = m.at(0, 1) * m.at(2, 0) - m.at(0, 0) * m.at(2, 1);
	res.at(2, 2) = m.at(0, 0) * m.at(1, 1) - m.at(0, 1) * m.at(1, 0);
	
	float det = determinant(m);
	res /= det;

	return res;
}

/*
mat3 inverse(const mat3& m) {
	if (fcmp(determinant(m), 0.f))
		throw std::exception("Matrix doesn't have an inverse. Determinant is 0");

	mat3 res = m;
	m.adjugate(res);
	res = transpose(res);

	float detinv = 1 / determinant(m);
	res *= detinv;

	return res;
}

void mat3::adjugate(mat3& m) const {
	mat2 minor;

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++) {
			if ((r + c) & 1u)
				m.at(r, c) = -minors(minor, r, c);
			else
				m.at(r, c) = minors(minor, r, c);
		}
}

float mat3::minors(mat2& m, int rowToCut, int colToCut) const {
	int rowOffset = 0;
	int colOffset = 0;

	for (int r = 0; r < size; r++)

		if (r == rowToCut)
			rowOffset = -1;

		else {

			for (int c = 0; c < size; c++) {

				if (c == colToCut)
					colOffset = -1;
				else
					m.at(r + rowOffset, c + colOffset) = at(r, c);
			}

			colOffset = 0;
		}

	return determinant(m);
}
*/

/****************
*	Matrix4x4	*
*****************/

mat4::mat4() {}

mat4::mat4(float n) {
	data[0] = data[5] = data[10] = data[15] = n;
}

mat4::mat4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) {
	data[0] = m00; data[1] = m01; data[2] = m02; data[3] = m03;
	data[4] = m10; data[5] = m11; data[6] = m12; data[7] = m13;
	data[8] = m20; data[9] = m21; data[10] = m22; data[11] = m23;
	data[12] = m30; data[13] = m31; data[14] = m32; data[15] = m33;
}

mat4::mat4(const mat4& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);
}

mat4& mat4::clean() {
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			float fl = std::truncf(at(r, c));

			if (fcmp(at(r, c), fl))
				at(r, c) = fl;
		}
	}
	
	return *this;
}

bool mat4::operator== (const mat4& m) const {
	bool b = true;

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			b = b && fcmp(at(r, c), m.at(r, c));

	return b;
}

bool mat4::operator!= (const mat4& m) const {
	return !this->operator==(m);
}

float mat4::at(int r, int c) const {
	return data[index(size, r, c)];
}

float& mat4::at(int r, int c) {
	return data[index(size, r, c)];
}

mat4& mat4::operator= (const mat4& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) = m.at(r, c);

	return *this;
}

mat4& mat4::operator+= (const mat4& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) += m.at(r, c);

	return *this;
}

mat4& mat4::operator-= (const mat4& m) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) -= m.at(r, c);

	return *this;
}

mat4& mat4::operator*= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) *= f;

	return *this;
}

mat4& mat4::operator/= (float f) {
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			at(r, c) /= f;

	return *this;
}

mat4 mat4::operator+ (const mat4& m) const {
	mat4 res(*this);
	res += m;

	return res;
}

mat4 mat4::operator- (const mat4& m) const {
	mat4 res(*this);
	res -= m;

	return res;
}

mat4 mat4::operator* (const mat4& m) const {
	mat4 res;
	mat4 t = transpose(m);

	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			for (int k = 0; k < size; k++)
				res.at(r, c) += at(r, k) * t.at(c, k);

	return res;
}

vec4 mat4::operator* (const vec4& v) const {
	vec4 res;

	res.x = at(0, 0) * v.x + at(0, 1) * v.y + at(0, 2) * v.z + at(0, 3) * v.w;
	res.y = at(1, 0) * v.x + at(1, 1) * v.y + at(1, 2) * v.z + at(1, 3) * v.w;
	res.z = at(2, 0) * v.x + at(2, 1) * v.y + at(2, 2) * v.z + at(2, 3) * v.w;
	res.w = at(3, 0) * v.x + at(3, 1) * v.y + at(3, 2) * v.z + at(3, 3) * v.w;

	return res;
}

mat4 mat4::operator* (float f) const {
	mat4 res(*this);
	res *= f;

	return res;
}

mat4 mat4::operator/ (float f) const {
	mat4 res(*this);
	res /= f;

	return res;
}

mat4 operator* (float f, const mat4& m) {
	mat4 res(m);
	res *= f;

	return res;
}

std::ostream& operator<< (std::ostream& os, const mat4& m) {
	std::streamsize prec = os.precision();
	os.precision(5);

	for (int r = 0; r < m.size; r++) {
		os << "[";
		for (int c = 0; c < m.size; c++)
			os << std::setw(10) << m.at(r, c);
		os << " ]" << std::endl;
	}

	os.precision(prec);

	return os;
}

mat4 transpose(const mat4& m) {
	mat4 res;

	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++)
			res.at(c, r) = m.at(r, c);

	return res;
}

void convertToColumnMajor(const mat4& m, float glfloat[]) {
	mat4 t = transpose(m);
	std::copy(std::begin(t.data), std::end(t.data), glfloat);
}

float* convertToColumnMajor(const mat4& m) {
	float* data = new float[16];

	for (int r = 0; r < m.size; r++)
		for (int c = 0; c < m.size; c++)
			data[index(m.size, c, r)] = m.at(r, c);

	return data;
}

