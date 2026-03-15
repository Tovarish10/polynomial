#include"poly.h"
poly::poly(v2&& v, bool reverse) :arg(v), d(v.size() - 1) { if (reverse)this->reverse(); }
poly::poly(std::initializer_list<double> list, bool reverse) :poly(static_cast<v2>(list), reverse) {}

poly poly::operator+(const poly& other) {
	return poly(arg + other.arg);
}
poly poly::operator-(const poly& other) {
	return poly(arg - other.arg);
}
poly poly::operator*(const poly& other) {
	uint64_t newD = d * other.d + 1;
	v2 ans(newD);
	for (uint64_t i = 0; i <= d; i++) {
		auto tmp = v2(other.d, newD).shift(-static_cast<int>(i));//可能的错误：若定义超过21亿元素的数组，则乘法无法正确计算
		ans += tmp * arg[i];
	}
	return poly(std::move(ans));
}
poly poly::operator/(const poly& other) {
	if (d < other.d)return poly({ 0 });
	uint64_t newD = d - other.d;
	v2 ans(newD+1),oldarg(arg);
	// iterate from newD down to 0 (inclusive) without risking unsigned-wrap infinite-loop warning
	for (uint64_t i = newD + 1; i-- > 0; ){
		ans[i]=oldarg[i+other.d]/other.arg[other.d];
		// subtract ans[i] * other shifted by i
		for (uint64_t j = 0; j < other.d; ++j) {
			oldarg[i + j] -= ans[i] * other.arg[j];
		}
	}
	return poly(std::move(ans));
}
poly poly::operator%(const poly& other) {
	if (d < other.d)return *this;
	uint64_t newD = d - other.d;
	v2 ans(arg);
	for (uint64_t i = newD + 1; i-- > 0; ) {
		for (uint64_t j = 0; j <= other.d; ++j) {
			ans[i + j] -= (ans[i + other.d] / other.arg[other.d]) * other.arg[j];
		}
	}
	for (newD--; (!ans[newD]) && newD; newD--);
	return poly(ans[std::slice(0,newD+1,1)]);
}