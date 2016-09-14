#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include "Piece.h"
#include "Point.h"


/// �Ă��Ȃ܂��@Solver
class Yakinamasi
{
public:
	using sysclock = std::chrono::system_clock;
	using time_t = std::chrono::time_point<std::chrono::system_clock>;
private:
	time_t		startT;			// Yakinamasi�J�n����
	std::chrono::milliseconds yakinamasiT;	// �Ă��݂�����

	std::vector<Piece> ps;		// �s�[�X

	std::vector<std::vector<int>> field;		// �Ֆ�
	const int fieldSize = 300;			// �Ֆʂ̃T�C�Y

	std::uniform_real_distribution<float> rd_f;			// [-fieldSize, fieldSize]�̎����̗�������l�ɕ��z������B
	std::mt19937 rd;										// �����Z���k�c�C�X�^


public:
	/// �R���X�g���N�^�B�{���͂킭�����Ȃ��Ƃ����Ȃ�
	Yakinamasi(unsigned int milliSeconds, const std::vector<Piece>& ps, int seed = 0);

	std::vector<Piece> yakinamasu();

private:
	Yakinamasi();

	void clearField();			// �t�B�[���h��0���߂���
	int evalField();				// ���݂̔Ֆʂ̕]���l��Ԃ�
	void movePiece(unsigned int id);			// id�Ԃ̃s�[�X�𓮂���
	void rotPiece(unsigned int id);			// id�Ԃ̃s�[�X����
	void initState();				// �����Ֆʂ𐶐�����
	void initPutPiece(unsigned int id);		// �ŏ��Ƀs�[�X��u���Ƃ������g��
	void nextState(unsigned int id);		// ���̏�Ԃ����Bid�Ԃ̃s�[�X���񂵂ē�����
	void putPiece(unsigned int id, const std::vector<Point> &oldPiece);			// id�Ԃ̃s�[�X��u������
	bool isIn(const Piece::BoundingBox& b);		// BoundingBox��field�Ɏ��܂��Ă��邩���肷��
	bool isIn(unsigned int id);						// ps.at(id)��field�Ɏ��܂��Ă��邩�𔻒肷��
	void dumpField();				// �t�B�[���h�̏�Ԃ��_���v����

	void rotPoint(Point & p, const Point & c, float r);// p���A�_c�𒆐S�Ƃ��āA�����v����r���W�A����]����B
	unsigned int elapsed(time_t t);
	unsigned int elapsed(time_t to, time_t from);
	std::ostream& logHead();
	void log(const std::string& msg);
};

