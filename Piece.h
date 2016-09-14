#pragma once

#include <vector>
#include "Point.h"

/// Pointの集合をPieceとする
class Piece
{
private:
	std::vector<Point> bresenham(Point from, Point to);		// ブレゼンハムのアルゴリズムにより直線を作る
public:
	/// 
	class BoundingBox {
	public:
		int top, bottom, left, right;
		BoundingBox(int t, int b, int l, int r): top(t), bottom(b), left(l), right(r) {}
		BoundingBox() {}

		/// 点の集合のBoundingBoxを作る
		static BoundingBox Create(const std::vector<Point>& pts);
	};

	std::vector<Point> frame;		// ピースの外形
	std::vector<Point> fill_pts;	// ピースの中身（外形を含む）
	BoundingBox boundingBox;		// ハコ


	std::vector<Point> fill();		// frameの中身を埋めた点の集合を返す
	
	Piece(const std::vector<Point>& pts) : frame(pts) {
		boundingBox = BoundingBox::Create(frame);
	}
	~Piece() {}
};

