#include"poly.h"
poly::poly(v2&& v, bool reverse) :arg(std::move(v)), d(v.size() - 1) { if (reverse)this->reverse(); if (!(d||arg[0]))d = -1; }
poly::poly(std::initializer_list<double> list, bool reverse) :poly(static_cast<v2>(list), reverse) {}

poly poly::operator+(const poly& other) const {
    if (!(d + 1)) return other;
    if (!(other.d + 1)) return *this;

    uint64_t maxD = d > other.d ? d : other.d;
    v2 ans(maxD + 1);
    for (uint64_t i = 0; i <= maxD; ++i) {
        ans[i] = ((i <= d) ? arg[i] : 0.0) + ((i <= other.d) ? other.arg[i] : 0.0);
    }
    return poly(std::move(ans));
}
poly poly::operator-()const {
	v2 ans(arg);
	for (auto&& i : ans)i = -i;
	return poly(std::move(ans));
}
poly poly::operator-(const poly& other)const {
	return -other+*this;
}
poly poly::operator*(const poly& other) const {
	if (!((d + 1) && (other.d + 1))) { return poly({ 0 }); }
    uint64_t newD = d + other.d;
    v2 ans(newD + 1);
    for (uint64_t i = 0; i <= d; ++i) {
        double ai = arg[i];
        for (uint64_t j = 0; j <= other.d; ++j) {
            ans[i + j] += ai * other.arg[j];
        }
    }
    return poly(std::move(ans));
}
poly poly::div(const poly& other, poly* r)const {
  // small epsilon to treat tiny values as zero and improve numerical stability
	// guard against division by (near) zero polynomial
	if (!(other.d + 1) || std::abs(other.arg[other.d]) < EPS) {
#ifdef _MSC_VER
		throw std::exception("ZeroDivisionError");
#else
		throw std::exception();
#endif
	}
	// if degree of dividend is smaller, quotient is zero and remainder is dividend
	if (d < other.d) {
		if (r != nullptr) *r = *this;
		return poly({ 0 });
	}
	uint64_t newD = d - other.d;
	v2 ans(newD + 1, 0.0), oldarg(arg);
	// iterate from newD down to 0 (inclusive) without risking unsigned-wrap infinite-loop warning
	for (uint64_t i = newD + 1; i-- > 0; ) {
		double lead = oldarg[i + other.d];
		// if leading term is numerically zero, skip this step
		if (std::abs(lead) < EPS) {
			ans[i] = 0.0;
			continue;
		}
		ans[i] = lead / other.arg[other.d];
		// subtract ans[i] * other shifted by i
		for (uint64_t j = 0; j <= other.d; ++j) {
			oldarg[i + j] -= ans[i] * other.arg[j];
		}
	}
	// normalize remainder: trim trailing coefficients that are numerically zero
	if (r != nullptr) {
		int64_t dr = static_cast<int64_t>(oldarg.size()) - 1;
		while (dr > 0 && std::abs(oldarg[static_cast<size_t>(dr)]) < EPS) {
			oldarg.pop_back();
			--dr;
		}
		*r = poly(std::move(oldarg));
	}
	// normalize quotient as well
	int64_t qr = static_cast<int64_t>(ans.size()) - 1;
	while (qr > 0 && std::abs(ans[static_cast<size_t>(qr)]) < EPS) {
		ans.pop_back();
		--qr;
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
	for (auto&& i : A.arg)i /= A.arg[A.d];
	return A;
}
poly lcm(const poly& a, const poly& b) {
	return (a / gcd(a, b)) * b;
}
poly poly::diff(uint64_t stage)const {
	if (!(d>=stage))return poly{0};
	v2 ans = arg;
	for (uint64_t j = 0; j < stage; j++) {
		for (uint64_t i = 0; i <= d; i++) {
			ans[i] *= (i-j);
		}
	}
	v2 ANS(ans.begin() + stage, ans.begin() + d + 1);
	return poly(std::move(ANS));
}

void poly::reverse() {
	uint64_t i = 0, j = d;
	while (i < j) {
		std::swap(arg[i], arg[j]);
		++i; --j;
	}
}
bool poly::operator==(const poly& other)const {
	return (d == other.d) && (arg == other.arg);
}
double poly::operator()(double num)const {
	double ans = 0;
	double num_time(num);
	for (auto&& i : arg) {
		ans += i * num_time;
		num_time *= num;
	}
	return ans;
}