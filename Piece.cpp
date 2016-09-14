#include "Piece.h"
#include <limits>

Piece::BoundingBox Piece::BoundingBox::Create(const std::vector<Point>& pts)
{
	int t = std::numeric_limits<int>::max(),
		b = std::numeric_limits<int>::min(),
		l = std::numeric_limits<int>::max(),
		r = std::numeric_limits<int>::min();

	for (const auto &p : pts) {
		if (p.x < l) {
			l = p.x;
		}
		if (p.x > r) {
			r = p.x;
		}
		if (p.y < t) {
			t = p.y;
		}
		if (p.y > b) {
			b = p.y;
		}
	}

	return BoundingBox(t, b, l, r);
}


std::vector<Point> Piece::bresenham(Point from, Point to)
{
	/// 直線を構成する点ら
	std::vector<Point> ps;

	/// 二点間の距離
	Point d = { to.x - from.x, to.y - from.y };

	/// 現在位置
	Point p = from;

	/// 変化量の少ない方の積立移動量
	int e = 0;

	/// x, yの移動量
	Point s{ (d.x > 0) ? 1 : -1,
		(d.y > 0) ? 1 : -1 };

	d.x = std::abs(d.x);
	d.y = std::abs(d.y);

	if (d.x > d.y) {
		/// xの方が大きく変化している場合
		while (p.x != to.x) {
			ps.push_back(p);

			e += d.y;
			if (e > d.x) {
				/// y方向の移動量の積立が溜まって、x方向の一回の移動量を超えた時
				e -= d.x;
				p.y += s.y;
			}
			p.x += s.x;
		}

		return ps;
	}
	else {
		/// yの方が大きく変化している場合
		while (p.y != to.y) {
			ps.push_back(p);

			e += d.x;
			if (e > d.y) {
				e -= d.y;
				p.x += s.x;
			}
			p.y += s.y;
		}
		return ps;
	}
}

#include <iostream>

std::vector<Point> Piece::fill()
{
	std::vector<std::vector<int>> field;		// ここを塗りつぶす。1pxずつ幅を持っている
	field.resize(boundingBox.bottom - boundingBox.top + 1);
	for (int i = 0; i < field.size(); i++) {
		field.at(i).assign(boundingBox.right - boundingBox.left + 1, 0);				// 0で初期化（塗り無し）
	}

	for (int i = 0; i < frame.size(); i++) {
		auto frameLine = bresenham(frame.at(i), frame.at((i + 1) % frame.size()));		// iとi+1の点を結ぶ直線
		for (const auto &p : frameLine) {
			field.at(p.y - boundingBox.top).at(p.x - boundingBox.left) = 1;		// 線を引く
		}
	}

	for (int y = 0; y < field.size(); y++) {
		int beginX = -1;		// 図形のなかに入った時のx座標
		for (int x = 0; x < field.at(y).size(); x++) {
			if (beginX == -1 && field.at(y).at(x) != 0) {
				beginX = x;		// 図形に入った
			}
			else if (beginX != -1 && field.at(y).at(x) != 0) {
				beginX = -1;		// 図形から出た
			}
			if (beginX != -1) {
				field.at(y).at(x) = 1;		// 塗る
			}
		}
		if (beginX != -1) {
			// 最後にbeginXが-1じゃないのは塗り過ぎなので戻る
			for (int x = beginX + 1; x < field.at(y).size(); x++) {
				field.at(y).at(x) = 0;
			}
		}
	}

	std::vector<Point> fill_pts;
	/// 収集
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field.at(y).size(); x++) {
			if (field.at(y).at(x) != 0) {
				fill_pts.push_back(Point{ x + boundingBox.left, y + boundingBox.top });
			}
		}
	}

	return fill_pts;
}
