#include "matrix_factory.h"
#include "matrices.h"
#include "vectors.h"
#include "quaternion.h"
#include "math_helpers.h"

/****************
*	Matrix2x2	*
*****************/

const mat2 MatrixFactory::createZeroMat2() {
	mat2 m;
	return m;
}

const mat2 MatrixFactory::createIdentityMat2() {
	mat2 m(1);
	return m;
}

const mat2 MatrixFactory::createScaleMat2(const vec2& v) {
	mat2 m;
	
	m.at(0, 0) = v.x;
	m.at(1, 1) = v.y;

	return m;
}

const mat2 MatrixFactory::createRotationMat2(float angle) {
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	mat2 m(cos, -sin, sin, cos);

	return m;
}

/****************
*	Matrix3x3	*
*****************/

const mat3 MatrixFactory::createZeroMat3() {
	mat3 m;
	return m;
}

const mat3 MatrixFactory::createIdentityMat3() {
	mat3 m(1);
	return m;
}

const mat3 MatrixFactory::createDualMat3(const vec3& v) {
	mat3 m(   0, -v.z, v.y ,
		    v.z,    0, -v.x,
		   -v.y,  v.x,    0);

	return m;
}

const mat3 MatrixFactory::createScaleMat3(const vec3& v) {
	mat3 m;

	m.at(0, 0) = v.x;
	m.at(1, 1) = v.y;
	m.at(2, 2) = v.z;

	return m;
}

/*
const mat3 MatrixFactory::createRotationMat3(float angle, const vec3& axis) {
	if (!fcmp(length(axis), 1.f))
		throw std::exception("Axis is not normalized");

	float sin = std::sin(angle);
	float cos = std::cos(angle);

	mat3 dual = createDualMat3(axis);

	return createIdentityMat3() + sin * dual + (1 - cos) * dual * dual;
*/

const mat3 MatrixFactory::createRotationMat3(float angle, const vec3& axis) {
	if (!fcmp(length(axis), 1.f))
		throw std::exception("Axis is not normalized");

	float sin = std::sin(angle);
	float cos = std::cos(angle);

	mat3 m(         cos + axis.x * axis.x * (1 - cos), axis.x * axis.y * (1 - cos) - axis.z * sin, axis.x * axis.z * (1 - cos) + axis.y * sin,
		   axis.y * axis.x * (1 - cos) + axis.z * sin,          cos + axis.y * axis.y * (1 - cos), axis.y * axis.z * (1 - cos) - axis.x * sin,
		   axis.z * axis.x * (1 - cos) - axis.y * sin, axis.z * axis.y * (1 - cos) + axis.x * sin,          cos + axis.z * axis.z * (1 - cos));

	return m;
}

const mat3 MatrixFactory::createMat3FromMat4(const mat4& m) {
	mat3 m3(m.at(0, 0), m.at(0, 1), m.at(0, 2),
		    m.at(1, 0), m.at(1, 1), m.at(1, 2),
		    m.at(2, 0), m.at(2, 1), m.at(2, 2));

	return m3;
}

/****************
*	Matrix4x4	*
*****************/

const mat4 MatrixFactory::createZeroMat4() {
	mat4 m;
	return m;
}

const mat4 MatrixFactory::createIdentityMat4() {
	mat4 m(1);
	return m;
}

const mat4 MatrixFactory::createMat4FromGLMatrix(const float* m) {
	mat4 res;

	for (int r = 0; r < res.size; r++) 
		for (int c = 0; c < res.size; c++)
			res.at(c, r) = m[index(res.size, r, c)];

	return res;
}

const mat4 MatrixFactory::createMat4FromQtrn(const qtrn& q) {
	qtrn qn = normalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	mat4 res(1.0f - 2.0f * (yy + zz),        2.0f * (xy + zt),        2.0f * (xz - yt), 0.0f,
		            2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz),        2.0f * (yz + xt), 0.0f,
		            2.0f * (xz + yt),        2.0f * (yz - xt), 1.0f - 2.0f * (xx + yy), 0.0f,
		                        0.0f,                    0.0f,                    0.0f, 1.0f);

	return transpose(res);
}

const mat4 MatrixFactory::createScaleMat4(const vec3& v) {
	return createMat4FromMat3(createScaleMat3(v));
}

const mat4 MatrixFactory::createRotationMat4(float angle, const vec3& axis) {
	return createMat4FromMat3(createRotationMat3(angle, axis));
}

const mat4 MatrixFactory::createTranslationMat4(const vec3& v) {
	mat4 m(1);

	m.at(0, 3) = v.x;
	m.at(1, 3) = v.y;
	m.at(2, 3) = v.z;

	return m;
}

const mat4 MatrixFactory::createMat4FromMat3(const mat3& m) {
	mat4 m4(m.at(0, 0), m.at(0, 1), m.at(0, 2), 0,
		    m.at(1, 0), m.at(1, 1), m.at(1, 2), 0,
		    m.at(2, 0), m.at(2, 1), m.at(2, 2), 0,
		             0,          0,          0, 1);

	return m4;
}

const mat4 MatrixFactory::createViewMatrix(const vec3& eye, const vec3& center, const vec3& up) {
	vec3 view = center - eye;
	vec3 v = normalize(view);

	vec3 side = cross(v, up);
	vec3 s = normalize(side);

	vec3 u = cross(s, v);

	vec3 t(-eye.x, -eye.y, -eye.z);

	mat3 r( s.x,  s.y,  s.z,
		    u.x,  u.y,  u.z,
		   -v.x, -v.y, -v.z);

	return createMat4FromMat3(r) * createTranslationMat4(t);
}

const mat4 MatrixFactory::createOrtographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far) {
	mat4 orto(2 / (right - left),                  0,                0, (left + right) / (left - right),
		                       0, 2 / (top - bottom),                0, (bottom + top) / (bottom - top),
		                       0,                  0, 2 / (near - far),     (near + far) / (near - far),
		                       0,                  0,                0,                               1);

	return orto;
}

const mat4 MatrixFactory::createPerspectiveProjectionMatrix(const float fovy, const float aspect, const float nearZ, const float farZ) {
	float angle = fovy / 2;
	float d = 1 / std::tan(angle);
	
	mat4 persp(d / aspect, 0,                               0,                                   0,
		                0, d,                               0,                                   0,
	                    0, 0, (nearZ + farZ) / (nearZ - farZ), (2 * farZ * nearZ) / (nearZ - farZ),
		                0, 0,                              -1,                                   0);

	return persp;
}

