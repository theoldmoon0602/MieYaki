#pragma once

#include <vector>
#include "Point.h"

/// Point�̏W����Piece�Ƃ���
class Piece
{
private:
	std::vector<Point> bresenham(Point from, Point to);		// �u���[���n���̃A���S���Y���ɂ�蒼�������
public:
	/// 
	class BoundingBox {
	public:
		int top, bottom, left, right;
		BoundingBox(int t, int b, int l, int r): top(t), bottom(b), left(l), right(r) {}
		BoundingBox() {}

		/// �_�̏W����BoundingBox�����
		static BoundingBox Create(const std::vector<Point>& pts);
	};

	std::vector<Point> frame;		// �s�[�X�̊O�`
	std::vector<Point> fill_pts;	// �s�[�X�̒��g�i�O�`���܂ށj
	BoundingBox boundingBox;		// �n�R


	std::vector<Point> fill();		// frame�̒��g�𖄂߂��_�̏W����Ԃ�
	
	Piece(const std::vector<Point>& pts) : frame(pts) {
		boundingBox = BoundingBox::Create(frame);
	}
	~Piece() {}
};

