#pragma once
#include <valarray>
#include<initializer_list>
#include<iosfwd>
using v2 = std::valarray<double>;

class poly {
public:
	poly(v2&&,bool=false);
	poly(std::initializer_list<double>,bool=false);
	poly(const poly&) = default;

	inline poly operator+(const poly&);
	inline poly operator-(const poly&);
	poly operator*(const poly&);
	poly operator/(const poly&);
	poly operator%(const poly&);

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

