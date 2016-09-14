#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include "Piece.h"
#include "Point.h"


/// 焼きなまし法Solver
class Yakinamasi
{
public:
	using sysclock = std::chrono::system_clock;
	using time_t = std::chrono::time_point<std::chrono::system_clock>;
private:
	time_t		startT;			// Yakinamasi開始時刻
	std::chrono::milliseconds yakinamasiT;	// 焼き鈍す時間

	std::vector<Piece> ps;		// ピース

	std::vector<std::vector<int>> field;		// 盤面
	const int fieldSize = 300;			// 盤面のサイズ

	std::uniform_real_distribution<float> rd_f;			// [-fieldSize, fieldSize]の実数の乱数を一様に分布させる。
	std::mt19937 rd;										// メルセンヌツイスタ


public:
	/// コンストラクタ。本当はわくも取らないといけない
	Yakinamasi(unsigned int milliSeconds, const std::vector<Piece>& ps, int seed = 0);

	std::vector<Piece> yakinamasu();

private:
	Yakinamasi();

	void clearField();			// フィールドを0埋めする
	int evalField();				// 現在の盤面の評価値を返す
	void movePiece(unsigned int id);			// id番のピースを動かす
	void rotPiece(unsigned int id);			// id番のピースを回す
	void initState();				// 初期盤面を生成する
	void initPutPiece(unsigned int id);		// 最初にピースを置くときだけ使う
	void nextState(unsigned int id);		// 次の状態を作る。id番のピースを回して動かす
	void putPiece(unsigned int id, const std::vector<Point> &oldPiece);			// id番のピースを置き直す
	bool isIn(const Piece::BoundingBox& b);		// BoundingBoxがfieldに収まっているか判定する
	bool isIn(unsigned int id);						// ps.at(id)がfieldに収まっているかを判定する
	void dumpField();				// フィールドの状態をダンプする

	void rotPoint(Point & p, const Point & c, float r);// pを、点cを中心として、反時計回りにrラジアン回転する。
	unsigned int elapsed(time_t t);
	unsigned int elapsed(time_t to, time_t from);
	std::ostream& logHead();
	void log(const std::string& msg);
};

