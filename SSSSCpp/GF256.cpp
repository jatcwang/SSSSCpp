#include "stdafx.h"
#include "GF256.h"
#include <vector>

using std::vector;

vector<unsigned int> _logTable(256, 0);
vector<unsigned int> _expTable(256, 0);

GF256elm::GF256elm() {
	val = 0;
}

GF256elm::GF256elm(int x) {
	val = ((x % 256) + 256) % 256;
}

GF256elm& GF256elm::operator=(const GF256elm& other) {
	val = other.val;
	return *this;
}

GF256elm& GF256elm::operator+=(const GF256elm& other)	{
	val = (val ^ other.val);
	return *this;
}

GF256elm& GF256elm::operator-=(const GF256elm& other) {
	val = (val ^ other.val);
	return *this;
}

GF256elm& GF256elm::operator*=(const GF256elm& other) {
	if ((val == 0) || other.val == 0) {
		val = 0;
		return *this;
	}
	int temp = (_logTable[val] + _logTable[other.val]) % 255;
	val = _expTable[temp];
	return *this;
}

GF256elm& GF256elm::operator/=(const GF256elm& other) {
	if (val == 0) {
		val = 0;
		return *this;
	}
	if (other.val == 0 ) {
		throw; //divison by zero
	}
	int t = _logTable[val] - _logTable[other.val];
	int temp =  ((t % 255) + 255) % 255;
	val = _expTable[temp];
	return *this;
}

bool GF256elm::operator==(const GF256elm& other) {
	return (this->val == other.val)? true:false;
} 

UINT GF256elm::getVal() {
	return val;
}


//Implement the binary operators. These are defined outside of the class according to guidelines. this allows us to write (for example)
//elm2 = 0x05 + elm1; (automatic conversion is done to convert 0x05 to an GF256 element first)

GF256elm operator+(GF256elm lhs, const GF256elm& rhs) {
	lhs += rhs;
	return lhs;
}

GF256elm operator-(GF256elm lhs, const GF256elm& rhs) {
	lhs -= rhs;
	return lhs;
}

GF256elm operator*(GF256elm lhs, const GF256elm& rhs) {
	lhs *= rhs;
	return lhs;
}

GF256elm operator/(GF256elm lhs, const GF256elm& rhs) {
	lhs /= rhs;
	return lhs;
}


void GF256init() {
	//0x03 to the power zero is 1
	unsigned int exp = 1;
	for (int i = 0; i < 255; i++) {
		exp &= 0xff;
		_expTable[i] = exp;

		unsigned int d = exp & 0x80;
		exp <<= 1;

		if (d == 0x80) {
			exp ^= 0x1b;
		}

		exp ^= _expTable[i];

		_logTable[_expTable[i]] = i;
	}
	_expTable[255] = _expTable[0];
	_logTable[0] = 0;
}