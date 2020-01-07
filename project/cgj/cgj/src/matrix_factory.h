#pragma once
#ifndef __MATRIX_FACTORY__
#define __MATRIX_FACTORY__

class vec2;
class vec3;
class vec4;

class mat2;
class mat3;
class mat4;

class qtrn;

class MatrixFactory {
public:
	static const mat2 createZeroMat2();
	static const mat2 createIdentityMat2();
	static const mat2 createScaleMat2(const vec2& v);
	static const mat2 createRotationMat2(float angle);

	static const mat3 createZeroMat3();
	static const mat3 createIdentityMat3();
	static const mat3 createDualMat3(const vec3& v);
	static const mat3 createScaleMat3(const vec3& v);
	static const mat3 createRotationMat3(float angle, const vec3& axis);
	static const mat3 createMat3FromMat4(const mat4& m);

	static const mat4 createZeroMat4();
	static const mat4 createIdentityMat4();
	static const mat4 createMat4FromGLMatrix(const float* m);
	static const mat4 createMat4FromQtrn(const qtrn& q);
	static const mat4 createScaleMat4(const vec3& v);
	static const mat4 createRotationMat4(float angle, const vec3& axis);
	static const mat4 createTranslationMat4(const vec3& v);
	static const mat4 createMat4FromMat3(const mat3& m);
	static const mat4 createViewMatrix(const vec3& eye, const vec3& center, const vec3& up);
	static const mat4 createOrtographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
	static const mat4 createPerspectiveProjectionMatrix(const float fovy, const float aspect, const float nearZ, const float farZ);
};

#endif