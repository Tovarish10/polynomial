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
	poly(poly&&)noexcept = default;
	poly& operator=(const poly&)= default;
	poly& operator=(poly&&)noexcept = default;

	inline poly operator+(const poly&)const;
	inline poly operator-(const poly&)const;
	poly operator*(const poly&)const;
	poly operator/(const poly&)const;
	poly operator%(const poly&)const;
	inline bool operator==(const poly&)const;

	template<typename charT, typename traits>
	friend std::basic_ostream<charT, traits>& \
		operator<<(std::basic_ostream<charT, traits>& os, const poly& p) {
		for (uint64_t i = 0; i <= p.d; i++)os << p.arg[i] << "x^" << i << (i == p.d ?' ' : '+');
		return os;
	}

	inline auto deg()const{ return d; }
	poly div(const poly&, poly* = nullptr)const;
	friend poly gcd(const poly&,const poly&);
	friend poly lcm(const poly&, const poly&);
private:
	uint64_t d;
	v2 arg;
	void reverse();
	poly() :d(0), arg({0}) {}
};

