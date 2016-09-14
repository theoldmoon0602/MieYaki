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
	/// �������\������_��
	std::vector<Point> ps;

	/// ��_�Ԃ̋���
	Point d = { to.x - from.x, to.y - from.y };

	/// ���݈ʒu
	Point p = from;

	/// �ω��ʂ̏��Ȃ����̐ϗ��ړ���
	int e = 0;

	/// x, y�̈ړ���
	Point s{ (d.x > 0) ? 1 : -1,
		(d.y > 0) ? 1 : -1 };

	d.x = std::abs(d.x);
	d.y = std::abs(d.y);

	if (d.x > d.y) {
		/// x�̕����傫���ω����Ă���ꍇ
		while (p.x != to.x) {
			ps.push_back(p);

			e += d.y;
			if (e > d.x) {
				/// y�����̈ړ��ʂ̐ϗ������܂��āAx�����̈��̈ړ��ʂ𒴂�����
				e -= d.x;
				p.y += s.y;
			}
			p.x += s.x;
		}

		return ps;
	}
	else {
		/// y�̕����傫���ω����Ă���ꍇ
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
	std::vector<std::vector<int>> field;		// ������h��Ԃ��B1px�����������Ă���
	field.resize(boundingBox.bottom - boundingBox.top + 1);
	for (int i = 0; i < field.size(); i++) {
		field.at(i).assign(boundingBox.right - boundingBox.left + 1, 0);				// 0�ŏ������i�h�薳���j
	}

	for (int i = 0; i < frame.size(); i++) {
		auto frameLine = bresenham(frame.at(i), frame.at((i + 1) % frame.size()));		// i��i+1�̓_�����Ԓ���
		for (const auto &p : frameLine) {
			field.at(p.y - boundingBox.top).at(p.x - boundingBox.left) = 1;		// ��������
		}
	}

	for (int y = 0; y < field.size(); y++) {
		int beginX = -1;		// �}�`�̂Ȃ��ɓ���������x���W
		for (int x = 0; x < field.at(y).size(); x++) {
			if (beginX == -1 && field.at(y).at(x) != 0) {
				beginX = x;		// �}�`�ɓ�����
			}
			else if (beginX != -1 && field.at(y).at(x) != 0) {
				beginX = -1;		// �}�`����o��
			}
			if (beginX != -1) {
				field.at(y).at(x) = 1;		// �h��
			}
		}
		if (beginX != -1) {
			// �Ō��beginX��-1����Ȃ��͓̂h��߂��Ȃ̂Ŗ߂�
			for (int x = beginX + 1; x < field.at(y).size(); x++) {
				field.at(y).at(x) = 0;
			}
		}
	}

	std::vector<Point> fill_pts;
	/// ���W
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field.at(y).size(); x++) {
			if (field.at(y).at(x) != 0) {
				fill_pts.push_back(Point{ x + boundingBox.left, y + boundingBox.top });
			}
		}
	}

	return fill_pts;
}
