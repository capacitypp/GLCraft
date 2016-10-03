//3次元ベクトルクラス

#ifndef ___Class_Vector
#define ___Class_Vector

#include <iostream>
#include <cmath>

class Vector {
	double x, y, z;
public:
	Vector(void) : x(0.0), y(0.0), z(0.0) {}
	Vector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	inline double getX(void) const { return x; }
	inline double getY(void) const { return y; }
	inline double getZ(void) const { return z; }
	inline double& getX(void) { return x; }
	inline double& getY(void) { return y; }
	inline double& getZ(void) { return z; }
	inline void print(void) const {
		std::cout << x << ", " << y << ", " << z << std::endl;
	}
	inline double norm(void) { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
	inline Vector outerProduct(const Vector& v) {
		Vector vector;
		vector.x = y * v.z - z * v.y;
		vector.y = z * v.x - x * v.z;
		vector.z = x * v.y - y * v.x;
		return vector;
	}
	inline Vector componentProduct(const Vector& v) {
		Vector vector(*this);
		vector.x *= v.x;
		vector.y *= v.y;
		vector.z *= v.z;
		return vector;
	}
	inline Vector operator-(void) const{
		return Vector(-x, -y, -z);
	}
	inline Vector operator*(double k) const {
		Vector v(*this);
		v *= k;
		return v;
	}
	inline Vector operator/(double k) const { return operator*(1 / k); }
	inline Vector operator+(const Vector& v) const {
		Vector vector(*this);
		vector += v;
		return vector;
	}
	inline Vector operator-(const Vector& v) const { return operator+(-v); }
	inline double operator*(const Vector& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	inline void operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	inline void operator-=(const Vector& v) { operator+=(-v); }
	inline void operator*=(double k) {
		x *= k;
		y *= k;
		z *= k;
	}
	inline void operator/=(double k) { operator*=(1 / k); }
	inline double operator[](int idx) const {
		switch (idx) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
		return x;
	}
	inline double& operator[](int idx) {
		switch (idx) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
		return x;
	}
};

#endif
