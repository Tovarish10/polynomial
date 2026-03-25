### 多项式类
---
##### 构造函数
- `poly(std::vector<double>&&,bool=false)`（参数1：参数数组，参数二:是否按高次向低次排序(默认否)）
- `poly(std::initializer_list<double>,bool=false)`（同上）
- 拷贝构造函数

##### 方法
- 多项式的四则运算
	- `operator+`
	- `operator-`
	- `operator*`
	- `operator/`
	- `operator%`
- `operator==`:判断两个多项式是否相等
- `operator(double)`:返回该点的值
- `operator<<(std::basic_ostream<T1,T2>&,const poly&)`: 重载输出流
- `poly.deg()`:获取多项式的次数
- `gcd(poly,poly)` `lcm(poly,poly)`:多项式的最大公因式和最小公倍式
- `poly::diff(uint=1)`:多项式的n阶导数
