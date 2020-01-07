#include "vectors.h"
#include "math_helpers.h"
#include <cmath>

/**************
*   Vector2   *
***************/

vec2::vec2() : x(0), y(0) {}

vec2::vec2(float n) : x(n), y(n) {}

vec2::vec2(float x, float y) : x(x), y(y) {}

vec2::vec2(const vec2& v) : x(v.x), y(v.y) {}

const float* vec2::data() const {
	return &x;
}

vec2& vec2::clean() {
	if (fcmp(x, std::truncf(x)))
		x = std::truncf(x);

	if (fcmp(y, std::truncf(y)))
		y = std::truncf(y);

	return *this;
}


bool vec2::isZero() const {
	return fcmp(x, 0.f) && fcmp(y, 0.f);
}

bool vec2::isUnitVector() const {
	return fcmp(quadrance(*this), 1.f);
}

bool vec2::operator== (const vec2& v) const {
	return fcmp(x, v.x) && fcmp(y, v.y);
}

bool vec2::operator!= (const vec2& v) const {
	return !this->operator==(v);
}

vec2& vec2::zero() {
	x = y = 0.f;

	return *this;
}

vec2& vec2::normalizeInPlace() {
	float len = length(*this);

	if (fcmp(len, 0.f)) 
		return *this;

	x /= len;
	y /= len;

	return *this;
}

vec2& vec2::operator= (const vec2& v) {
	x = v.x;
	y = v.y;

	return *this;
}

vec2& vec2::operator+= (const vec2& v) {
	x += v.x;
	y += v.y;

	return *this;
}

vec2& vec2::operator-= (const vec2& v) {
	x -= v.x;
	y -= v.y;

	return *this;
}

vec2& vec2::operator*= (float f) {
	x *= f;
	y *= f;

	return *this;
}

vec2& vec2::operator/= (float f) {
	x /= f;
	y /= f;

	return *this;
}

vec2 vec2::operator+ (const vec2& v) const {
	vec2 res;
	res.x = x + v.x;
	res.y = y + v.y;

	return res;
}

vec2 vec2::operator- (const vec2& v) const {
	vec2 res;
	res.x = x - v.x;
	res.y = y - v.y;

	return res;
}

vec2 vec2::operator- () const {
	vec2 res;
	res.x = -x;
	res.y = -y;

	return res;
}

vec2 vec2::operator* (float f) const {
	vec2 res;
	res.x = x * f;
	res.y = y * f;

	return res;
}

vec2 vec2::operator/ (float f) const {
	vec2 res;
	res.x = x / f;
	res.y = y / f;

	return res;
}

vec2 operator* (float f, const vec2& v) {
	vec2 res;
	res.x = v.x * f;
	res.y = v.y * f;

	return res;
}

std::istream& operator>> (std::istream& is, vec2& v) {
	is >> v.x >> v.y;

	return is;
}

std::ostream& operator<< (std::ostream& os, const vec2& v) {
	std::streamsize prec = os.precision();
	os.precision(5);

	os << "[ " << v.x << " " << v.y << " ]";

	os.precision(prec);

	return os;
}

vec2 orthogonal(const vec2& v) {
	vec2 res;
	res.x = v.y;
	res.y = -v.x;

	return res;
}

vec2 normalize(const vec2& v) {
	float len = length(v);
	vec2 res;

	if (fcmp(len, 0.f)) {
		res = v;
		return res;
	}

	res = v / len;

	return res;
}

const float dot(const vec2& v, const vec2& other) {
	return v.x * other.x + v.y * other.y;
}

const float distance(const vec2 v, const vec2& other) {
	return length(v - other);
}

const float angle(const vec2& v, const vec2& other) {
	float tmp = dot(v, other);
	return std::acos(tmp / (length(v) * length(other)));
}

const float length(const vec2& v) {
	float quad = quadrance(v);
	return std::sqrt(quadrance(v));
}

const float quadrance(const vec2& v) {
	return v.x * v.x + v.y * v.y;
}

/**************
*	Vector3   *
***************/

vec3::vec3() : x(0.f), y(0.f), z(0.f) {}

vec3::vec3(float n) : x(n), y(n), z(n) {}

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

vec3::vec3(const vec3& v): x(v.x), y(v.y), z(v.z) {}

vec3::vec3(const vec4& v) {
	x = v.x / v.w;
	y = v.y / v.w;
	z = v.z / v.w;
}

const float* vec3::data() const {
	return &x;
}

vec3& vec3::clean() {
	if (fcmp(x, std::truncf(x)))
		x = std::truncf(x);

	if (fcmp(y, std::truncf(y)))
		y = std::truncf(y);

	if (fcmp(z, std::truncf(z)))
		z = std::truncf(z);

	return *this;
}

bool vec3::isZero() const {
	return fcmp(x, 0.f) && fcmp(y, 0.f) && fcmp(z, 0.f);
}

bool vec3::isUnitVector() const {
	return fcmp(quadrance(*this), 1.f);
}

bool vec3::operator== (const vec3& v) const {
	return fcmp(x, v.x) && fcmp(y, v.y) && fcmp(z, v.z);
}

bool vec3::operator!= (const vec3& v) const {
	return !this->operator==(v);
}

vec3& vec3::zero() {
	x = y = z = 0.f;

	return *this;
}

vec3& vec3::normalizeInPlace() {
	float len = length(*this);

	if (fcmp(len, 0.f))
		return *this;

	x /= len;
	y /= len;
	z /= len;

	return *this;
}

vec3& vec3::operator= (const vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

vec3& vec3::operator+= (const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

vec3& vec3::operator-= (const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

vec3& vec3::operator*= (float f) {
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

vec3& vec3::operator/= (float f) {
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

vec3 vec3::operator+ (const vec3& v) const {
	vec3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;

	return res;
}

vec3 vec3::operator- (const vec3& v) const {
	vec3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;

	return res;
}

vec3 vec3::operator- () const {
	vec3 res;
	res.x = -x;
	res.y = -y;
	res.z = -z;

	return res;
}

vec3 vec3::operator* (float f) const {
	vec3 res;
	res.x = x * f;
	res.y = y * f;
	res.z = z * f;

	return res;
}

vec3 vec3::operator/ (float f) const {
	vec3 res;
	res.x = x / f;
	res.y = y / f;
	res.z = z / f;

	return res;
}

vec3 operator* (float f, const vec3& v) {
	vec3 res;
	res.x = v.x * f;
	res.y = v.y * f;
	res.z = v.z * f;

	return res;
}

std::istream& operator>> (std::istream& is, vec3& v) {
	is >> v.x >> v.y >> v.z;

	return is;
}

std::ostream& operator<< (std::ostream& os, const vec3& v) {
	std::streamsize prec = os.precision();
	os.precision(5);

	os << "[ " << v.x << " " << v.y << " " << v.z << " ]";

	os.precision(prec);

	return os;
}

vec3 normalize(const vec3& v) {
	float len = length(v);
	vec3 res;

	if (fcmp(len, 0.f)) {
		res = v;
		return res;
	}

	res = v / len;

	return res;
}

vec3 cross(const vec3& v, const vec3& other) {
	vec3 res;
	res.x = v.y * other.z - v.z * other.y;
	res.y = v.z * other.x - v.x * other.z;
	res.z = v.x * other.y - v.y * other.x;

	return res;
}

const float dot(const vec3& v, const vec3& other) {
	return v.x * other.x + v.y * other.y + v.z * other.z;
}

const float distance(const vec3& v, const vec3& other) {
	return length(v - other);
}

const float angle(const vec3& v, const vec3& other) {
	float tmp = dot(v, other);
	return std::acos(tmp / (length(v) * length(other)));
}

const float length(const vec3& v) {
	float quad = quadrance(v);
	return std::sqrt(quadrance(v));
}

const float quadrance(const vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

/**************
*	Vector4   *
***************/

vec4::vec4() : x(0), y(0), z(0), w(1) {}

vec4::vec4(float x, float y, float z) : x(x), y(y), z(z), w(1.0f) {}

vec4::vec4(float n) : x(n), y(n), z(n), w(1) {}

vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

vec4::vec4(const vec3& v) : x(v.x), y(v.y), z(v.z), w(1) {}

vec4::vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

const float* vec4::data() const {
	return &x;
}

vec4& vec4::clean() {
	if (fcmp(x, std::truncf(x)))
		x = std::truncf(x);

	if (fcmp(y, std::truncf(y)))
		y = std::truncf(y);

	if (fcmp(z, std::truncf(z)))
		z = std::truncf(z);

	if (fcmp(w, std::truncf(w)))
		w = std::truncf(w);

	return *this;
}

bool vec4::operator== (const vec4& v) const {
	return fcmp(x, v.x) && fcmp(y, v.y) && fcmp(z, v.z) && fcmp(w, v.w);
}

bool vec4::operator!= (const vec4& v) const {
	return !this->operator==(v);
}

std::istream& operator>> (std::istream& is, vec4& v) {
	is >> v.x >> v.y >> v.z >> v.w;

	return is;
}

std::ostream& operator<< (std::ostream& os, const vec4& v) {
	std::streamsize prec = os.precision();
	os.precision(5);

	os << "[ " << v.x << " " << v.y << " " << v.z << " " << v.w << " ]";

	os.precision(prec);

	return os;
}