#include "Yakinamasi.h"


Yakinamasi::Yakinamasi(unsigned int milliSeconds, const std::vector<Piece>& ps, int seed)
	: yakinamasiT(milliSeconds),
	ps(ps),
	rd_f(),
	rd()
{
	startT = sysclock::now();

	int rd_seed = (seed == 0) ? std::random_device()() : seed;
	rd.seed(rd_seed);

	logHead() << "Yakinamasi Seed is " << rd_seed << std::endl;

	clearField();
}

std::vector<Piece> Yakinamasi::yakinamasu()
{
	log("Yakinamasi Start");

	auto endT = startT + yakinamasiT;
	time_t curT;

	initState();
	int bestScore = evalField();
	auto bestState = ps;
	auto bestField = field;

	while (curT = sysclock::now(), curT < endT) {
		unsigned int id = rd() % ps.size();

		nextState(id);
		int curScore = evalField();

		logHead() << "Score: " << curScore;

		if (bestScore < curScore) {
			std::cerr << " [BEST]" << std::endl;
			bestScore = curScore;
			bestState = ps;
			bestField = field;
		}
		else {
			std::cerr << " [ABANDON]" << std::endl;

			ps = bestState;
			field = bestField;
		}

	}

	
	log("Yakinamasi Finished");
	std::cerr << "Best Score is: " << bestScore << std::endl;

	std::cerr << std::endl;
	dumpField();
	std::cerr << std::endl;

	return ps;
}


void Yakinamasi::clearField()
{
	field.resize(fieldSize);
	for (int i = 0; i < field.size(); i++) {
		field.at(i).assign(fieldSize, 0);
	}
}

int Yakinamasi::evalField()
{
	int value = 0;
	for (int i = 0; i < field.size(); i++) {
		value += std::count(field.at(i).begin(), field.at(i).end(), 1);
	}

	return value;
}

void Yakinamasi::movePiece(unsigned int id)
{
	/// ƒ‰ƒ“ƒ_ƒ€‚É“®‚©‚·‚ª‚Í‚İo‚½‚ç‚â‚è’¼‚µ
	auto &p = ps.at(id);
	while (true) {
		auto frame_back = p.frame;
		Point d(rd() % (fieldSize * 2) - fieldSize, rd() % (fieldSize * 2) - fieldSize);

		for (int i = 0; i < p.frame.size(); i++) {
			p.frame.at(i).x += d.x;
			p.frame.at(i).y += d.y;
		}

		if (isIn(id)) {
			break;
		}

		/// –ß‚·
		p.frame = frame_back;
	}
}

void Yakinamasi::rotPiece(unsigned int id)
{
	auto & p = ps.at(id);

	float r = rd_f(rd);			// ‰ñ“]—Ê‚ÌŒˆ’è

	Point c(0, 0);					// ‰ñ“]’†S
	for (int i = 0; i < p.frame.size(); i++) {
		c.x += p.frame.at(i).x;
		c.y += p.frame.at(i).y;
	}
	c.x /= p.frame.size();
	c.y /= p.frame.size();

	/// ‰ñ“]‚³‚¹‚é
	for (int i = 0; i < p.frame.size(); i++) {
		rotPoint(p.frame.at(i), c, r);
	}
}

void Yakinamasi::initState()
{
	for (int i = 0; i < ps.size(); i++) {
		auto &p = ps.at(i);
		while (true) {
			auto frame_back = p.frame;
			Point d(rd() % fieldSize, rd() %  fieldSize);

			for (int i = 0; i < p.frame.size(); i++) {
				p.frame.at(i).x += d.x;
				p.frame.at(i).y += d.y;
			}

			if (isIn(i)) {
				break;
			}

			/// –ß‚·
			p.frame = frame_back;
		}
		initPutPiece(i);
	}
}

void Yakinamasi::initPutPiece(unsigned int id)
{
	ps.at(id).fill_pts = ps.at(id).fill();
	for (int i = 0; i < ps.at(id).fill_pts.size(); i++) {
		const auto &p = ps.at(id).fill_pts.at(i);
		field.at(p.y).at(p.x)++;
	}
}

void Yakinamasi::nextState(unsigned int id)
{
	auto oldPiece = ps.at(id).fill_pts;


	/// ‰ñ“]
	rotPiece(id);

	/// ˆÚ“®
	movePiece(id);

	putPiece(id, oldPiece);
}

void Yakinamasi::putPiece(unsigned int id, const std::vector<Point>& oldPiece)
{
	/// ŒÃ‚¢ƒs[ƒX‚ğæ‚èœ‚­
	for (int i = 0; i < oldPiece.size(); i++) {
		auto &p = oldPiece.at(i);
		field.at(p.y).at(p.x)--;
	}

	/// V‚µ‚¢êŠ‚É’u‚«’¼‚µ
	ps.at(id).fill_pts = ps.at(id).fill();

	for (int i = 0; i < ps.at(id).fill_pts.size(); i++) {
		auto &p = ps.at(id).fill_pts.at(i);
		field.at(p.y).at(p.x)++;
	}
}

bool Yakinamasi::isIn(const Piece::BoundingBox & b)
{
	if (b.top < 0 ||
		b.bottom >= fieldSize ||
		b.left < 0 ||
		b.right >= fieldSize) {
		return false;
	}
	return true;
}

bool Yakinamasi::isIn(unsigned int id)
{
	ps.at(id).boundingBox = Piece::BoundingBox::Create(ps.at(id).frame);
	return isIn(ps.at(id).boundingBox);
}

void Yakinamasi::dumpField()
{
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field.at(y).size(); x++) {
			std::cerr << field.at(y).at(x);
		}
		std::cerr << std::endl;
	}
}


void Yakinamasi::rotPoint(Point& p, const  Point& c, float r) {
	/// ‰ñ“]’†S‚ğŒ´“_‚É‚Á‚Ä‚­‚é
	Point buf;
	buf.x = p.x - c.x;
	buf.y = p.y - c.y;

	/// ‰ñ“]‚·‚é
	p.x = buf.x * std::cos(r) - buf.y * std::sin(r);
	p.y = buf.x * std::sin(r) + buf.y * std::cos(r);

	/// ‰ñ“]’†S‚ğ–ß‚·
	p.x += c.x;
	p.y += c.x;
}

unsigned int Yakinamasi::elapsed(time_t t)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(t - startT).count();
}

unsigned int Yakinamasi::elapsed(time_t to, time_t from)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
}

std::ostream& Yakinamasi::logHead()
{
	return std::cerr << "[ " << elapsed(sysclock::now()) << " ms] ";
}

void Yakinamasi::log(const std::string & msg)
{
	logHead() << msg << std::endl;
}
