#pragma once
#include <valarray>
#include<initializer_list>
#include<iosfwd>
using v2 = std::valarray<double>;

class poly {
public:
	poly(v2&&,bool);
	poly(std::initializer_list<double>,bool);

	inline auto operator+(const poly&);
	inline auto operator-(const poly&);
	auto operator*(const poly&);
	auto operator/(const poly&);
	auto operator%(const poly&);

	template<typename charT, typename traits>
	friend std::basic_ostream<charT, traits>& \
		operator<<(std::basic_ostream<charT, traits>& os, const poly& p) {
		for (uint64_t i = 0; i <= p.d; i++)os << p.arg[i] << "x^" << i << (i == p.d ?' ' : '+');
		return os;
	}

	inline auto deg()const{ return d; }
private:
	uint64_t d;
	v2 arg;
	void reverse() {
		uint64_t i = 0, j = d;
		while (i < j) {
			std::swap(arg[i], arg[j]);
			++i; --j;
		}
	}
};

poly::poly(v2&& v, bool reverse = false) :arg(v), d(v.size()-1) { if (reverse)this->reverse(); }
poly::poly(std::initializer_list<double> list, bool reverse = false) :poly(static_cast<v2>(list), reverse){}

auto poly::operator+(const poly& other) {
	return v2(arg + other.arg);
}
auto poly::operator-(const poly& other) {
	return v2(arg - other.arg);
}
auto poly::operator*(const poly& other) {
	uint64_t newD = d * other.d+1;
	auto ans = v2(newD);
	for (uint64_t i = 0; i <= d;i++) {
		auto tmp = v2(other.d, newD).shift(-static_cast<int>(i));//可能的错误：若定义超过21亿元素的数组，则乘法无法正确计算
		ans+=tmp*arg[i];
	}
	return v2(ans);
}
auto poly::operator/(const poly& other) {
	if (d < other.d)return poly({ 0 });
	
}