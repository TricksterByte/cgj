#include "quaternion.h"
#include "vectors.h"
#include "math_helpers.h"

qtrn::qtrn(): t(1), x(0), y(0), z(0) {}

qtrn::qtrn(float t, float x, float y, float z): t(t), x(x), y(y), z(z) {}

qtrn::qtrn(float angle, const vec3& axis) {
	if (!axis.isUnitVector())
		throw std::exception("Axis is not normalized");

	float sin = std::sin(angle / 2.0f);

	t = std::cos(angle / 2.0f);
	x = axis.x * sin;
	y = axis.y * sin;
	z = axis.z * sin;
}

qtrn& qtrn::clean() {
	if (fcmp(t, std::truncf(t)))
		t = std::truncf(t);
	
	if (fcmp(x, std::truncf(x)))
		x = std::truncf(x);

	if (fcmp(y, std::truncf(y)))
		y = std::truncf(y);

	if (fcmp(z, std::truncf(z)))
		z = std::truncf(z);

	return *this;
}

void qtrn::toAngleAxis(float& angle, vec3& axis) const {
	qtrn n = normalize(*this);

	angle = radiansToDegrees(2.0f * std::acos(t));
	float s = std::sqrt(1.0f - t * t);

	if (fcmp(s, 0)) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
	} else {
		float sinv = 1 / s;
		axis.x = x * sinv;
		axis.y = y * sinv;
		axis.z = z * sinv;
	}
}

void qtrn::printAngleAxis(std::string& s) const {
	std::cout << s << " = [" << std::endl;

	float angle;
	vec3 axis;
	toAngleAxis(angle, axis);
	
	std::cout << "  angle = " << angle << std::endl;
	std::cout << "  axis " << axis << std::endl;
	std::cout << "]" << std::endl;
}

bool qtrn::operator== (const qtrn& q) const {
	return fcmp(t, q.t) && fcmp(x, q.x) && fcmp(y, q.y) && fcmp(z, q.z);
}

bool qtrn::operator!= (const qtrn& q) const {
	return !this->operator==(q);
}

qtrn& qtrn::normalizeInPlace() {
	float len = length(*this);

	if (fcmp(len, 0))
		return *this;

	float norminv = 1.0f / len;

	t *= norminv;
	x *= norminv;
	y *= norminv;
	z *= norminv;

	return *this;
}

qtrn& qtrn::operator= (const qtrn& q) {
	t = q.t;
	x = q.x;
	y = q.y;
	z = q.z;

	return *this;
}

qtrn& qtrn::operator+= (const qtrn& q) {
	t += q.t;
	x += q.x;
	y += q.y;
	z += q.z;

	return *this;
}

qtrn& qtrn::operator-= (const qtrn& q) {
	t -= q.t;
	x -= q.x;
	y -= q.y;
	z -= q.z;

	return *this;
}

qtrn& qtrn::operator*= (const qtrn& q) {
	vec3 img(x, y, z);
	vec3 other(q.x, q.y, q.z);
	vec3 c = cross(img, other);

	t = t * q.t - dot(img, other);
	x = t * q.x + x * q.t + c.x;
	y = t * q.y + y * q.t + c.y;
	z = t * q.z + z * q.t + c.z;

	return *this;
}

qtrn& qtrn::operator*= (float f) {
	t *= f;
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

qtrn qtrn::operator+ (const qtrn& q) const {
	qtrn res;
	
	res.t = t + q.t;
	res.x = x + q.x;
	res.y = y + q.y;
	res.z = z + q.z;

	return res;
}

qtrn qtrn::operator- (const qtrn& q) const {
	qtrn res;

	res.t = t - q.t;
	res.x = x - q.x;
	res.y = y - q.y;
	res.z = z - q.z;

	return res;
}

qtrn qtrn::operator* (const qtrn& q) const {
	vec3 img(x, y, z);
	vec3 other(q.x, q.y, q.z);
	vec3 c = cross(img, other);
	
	qtrn res;

	res.t = t * q.t - dot(img, other);
	res.x = t * q.x + x * q.t + c.x;
	res.y = t * q.y + y * q.t + c.y;
	res.z = t * q.z + z * q.t + c.z;

	return res;
}
qtrn qtrn::operator* (float f) const {
	qtrn res;

	res.t = t * f;
	res.x = x * f;
	res.y = y * f;
	res.z = z * f;

	return res;
}

qtrn operator* (float f, const qtrn& q) {
	qtrn res;

	res.t = q.t * f;
	res.x = q.x * f;
	res.y = q.y * f;
	res.z = q.z * f;

	return res;
}

std::ostream& operator<< (std::ostream& os, const qtrn& q) {
	std::cout << "Quaternion(t: " << q.t << ", x: " << q.x << ", y: " << q.y << ", z: " << q.z << ") @ 0x" << &q << std::endl;

	return os;
}

const qtrn conjugate(const qtrn& q) {
	qtrn conj;

	conj.t = q.t;
	conj.x = -q.x;
	conj.y = -q.y;
	conj.z = -q.z;

	return conj;
}

const qtrn inverse(const qtrn& q) {
	qtrn conj = conjugate(q);
	float quad = quadrance(q);

	if (fcmp(quad, 0))
		throw std::exception("Quaternion norm is zero");

	return conj * (1.0f / quad);
}

const qtrn normalize(const qtrn& q) {
	float len = length(q);

	if (fcmp(len, 0.f))
		return q;

	float norm = 1.0f / len;

	qtrn res = q * norm;

	return res;
}

const qtrn lerp(const qtrn& q0, const qtrn& q1, float k) {
	float cos_angle = q0.t * q1.t + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;

	qtrn q = q0 * k0 + q1 * k1;
	return normalize(q);
}

const qtrn slerp(const qtrn& q0, const qtrn& q1, float k) {
	float angle = std::acos(q0.t * q1.t + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z);
	float k0 = std::sin((1.0f - k) * angle) / std::sin(angle);
	float k1 = std::sin(k * angle) / std::sin(angle);

	qtrn q = q0 * k0 + q1 * k1;
	return normalize(q);
}

const float length(const qtrn& q) {
	return std::sqrt(quadrance(q));
}

const float quadrance(const qtrn& q) {
	return q.t * q.t + q.x * q.x + q.y * q.y + q.z * q.z;
}