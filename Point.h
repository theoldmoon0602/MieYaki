#pragma once

/// (x, y)��int�Ŏ���
class Point
{
public:
	using T = int;		// ���ƂŌ^��ς������Ȃ邩������Ȃ�

	T x, y;				// ���W

	Point() {}
	Point(T x, T y) : x(x), y(y) {}
	~Point() {}
};

