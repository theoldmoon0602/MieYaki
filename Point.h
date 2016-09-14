#pragma once

/// (x, y)‚ğint‚Å‚Â
class Point
{
public:
	using T = int;		// ‚ ‚Æ‚ÅŒ^‚ğ•Ï‚¦‚½‚­‚È‚é‚©‚à‚µ‚ê‚È‚¢

	T x, y;				// À•W

	Point() {}
	Point(T x, T y) : x(x), y(y) {}
	~Point() {}
};

