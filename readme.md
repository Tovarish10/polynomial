### 多项式类
---
##### 构造函数
- `poly(std::valarray<double>&&,bool=false)`（参数1：参数数组，参数二:是否按高次向低次排序(默认否)）
- `poly(std::initializer_list<double>,bool=false`（同上）
- 拷贝构造函数

##### 方法
- 多项式的四则运算
	- `operator+`
	- `operator-`
	- `operator*`
	- `operator/`
	- `operator%`
- `operator<<(std::basic_ostream<T1,T2>&,const poly&)`: 重载输出流
- `deg()`:获取多项式的次数