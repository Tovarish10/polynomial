#include"poly.h"
poly::poly(v2&& v, bool reverse) :arg(std::move(v)), d(v.size() - 1) { if (reverse)this->reverse(); if (!(d||arg[0]))d = -1; }
poly::poly(std::initializer_list<double> list, bool reverse) :poly(static_cast<v2>(list), reverse) {}

poly poly::operator+(const poly& other)const {
	return poly(arg + other.arg);
}
poly poly::operator-(const poly& other)const {
	return poly(arg - other.arg);
}
poly poly::operator*(const poly& other) const {
	if (!((d + 1) && (other.d + 1))) { return poly({ 0 }); }
    uint64_t newD = d + other.d;
    v2 ans(newD + 1);
    ans = 0.0; 
    for (uint64_t i = 0; i <= d; ++i) {
        double ai = arg[i];
        for (uint64_t j = 0; j <= other.d; ++j) {
            ans[i + j] += ai * other.arg[j];
        }
    }
    return poly(std::move(ans));
}
poly poly::div(const poly& other, poly* r)const {
	if (d < other.d) {
		if (!(other.d + 1))throw std::exception("ZeroDivisionError");
		if (r != nullptr)*r = *this;
		return poly({ 0 });
	}
	uint64_t newD = d - other.d;
	v2 ans(newD + 1), oldarg(arg);
	// iterate from newD down to 0 (inclusive) without risking unsigned-wrap infinite-loop warning
	for (uint64_t i = newD + 1; i-- > 0; ) {
		ans[i] = oldarg[i + other.d] / other.arg[other.d];
		// subtract ans[i] * other shifted by i
		for (uint64_t j = 0; j <= other.d; ++j) {
			oldarg[i + j] -= ans[i] * other.arg[j];
		}
	}
	if (r != nullptr) {
		for (newD--; (!oldarg[newD]) && newD; newD--);
		*r = poly(oldarg[std::slice(0, newD + 1, 1)]);
	}
	return poly(std::move(ans));
}
poly poly::operator/(const poly& other)const {
	return div(other);
}
poly poly::operator%(const poly& other)const {
	poly r;
	div(other, &r);
	return r;
}

poly gcd(const poly& a, const poly& b) {
	if (!(a.d && b.d))return poly({ 1 });
	if (!((a.d + 1) && (b.d + 1)))return poly({ 0 });
	poly A,B;
	if (a.d >= b.d) { B = a % b; A = b; }
	else { B = b % a; A = a; }
	while (B != poly{ 0 }) {
		poly tmp = A % B;
		A = std::move(B);
		B = std::move(tmp);
	}
	A.arg /= A.arg[A.d];
	return A;
}
poly lcm(const poly& a, const poly& b) {
	return (a / gcd(a, b)) * b;
}

void poly::reverse() {
	uint64_t i = 0, j = d;
	while (i < j) {
		std::swap(arg[i], arg[j]);
		++i; --j;
	}
}