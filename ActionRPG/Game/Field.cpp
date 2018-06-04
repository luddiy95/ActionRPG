#include "Game/Field.h"
#include "Game/GameParent.h"
#include "Vector2.h"
#include "Matrix23.h"
#include "Image.h"
#include "File.h"
#include "GameLib/Framework.h"
#include "GameLib/Math.h"
using namespace GameLib;

namespace Game {
	Field::Line::Line() : mSlope(0), mInterceptX(0){}
	Field::Line::~Line(){}
	void Field::Line::setSlope(double s) {
		mSlope = s;
	}
	void Field::Line::setInterceptX(double ix) {
		mInterceptX = ix;
	}
	double Field::Line::getX(int y) const {
		return mInterceptX - GameLib::tan(mSlope) * getY(y);
	}
	double Field::Line::getX(double y) const {
		return mInterceptX - GameLib::tan(mSlope) * y;
	}
	double Field::Line::getY(int y) const {
		return (y == 0) ? 0.0 : gFieldHeight *
								(1.0 - pow(gFieldScale, static_cast<double>(y))) / (1.0 - gFieldScale);
	}



	Field::Trout::Trout() : 
		mCoordinate(0),
		mCharactor(NONE),
		mState(PLAYER_AREA){
		mCoordinate = new Vector2[4];
	}
	Field::Trout::~Trout(){
		SAFE_DELETE(mCoordinate);
	}
	void Field::Trout::setCoordinate(double x0, double x1, double x2, double x3, double y0, double y1) {
		mCoordinate[0].setVector(x0, y0);
		mCoordinate[1].setVector(x1, y0);
		mCoordinate[2].setVector(x2, y1);
		mCoordinate[3].setVector(x3, y1);
	}
	void Field::Trout::draw(const Image& image, const Vector2& translation) const {
		if (mState == HOLE) {
			return;
		}
		Matrix23 translate;
		translate.setTranslation(translation); //ñ{óàÇÃâfÇµèoÇ≥ÇÍÇÈèÍèäÇ…à⁄ìÆ
		Vector2 C0, C1, C2, C3;
		translate.multiply(&C0, mCoordinate[0]);
		translate.multiply(&C1, mCoordinate[1]);
		translate.multiply(&C2, mCoordinate[2]);
		translate.multiply(&C3, mCoordinate[3]);

		int x = 0, y = 0;
		if (mState & PLAYER_AREA) {
			x = 0; y = 0;
			if (mState & ATTACKED) {
				x = 1;
			}
		}
		else if (mState & ENEMY_AREA) {
			x = 0; y = 1;
			if (mState & ATTACKED) {
				x = 1;
			}
		}
		image.draw(C0, C1, C2, C3, x * gFieldWidth, y * gFieldHeight, gFieldWidth, gFieldHeight);
	}
	Vector2* Field::Trout::getCoordinate() const {
		return mCoordinate;
	}
	void Field::Trout::initializeState(unsigned state) {
		mState = state;
	}
	unsigned Field::Trout::getState() const {
		return mState;
	}
	void Field::Trout::setFlag(unsigned f) {
		mState |= f;
	}
	void Field::Trout::resetFlag(unsigned f) {
		mState &= ~f;
	}
	void Field::Trout::setCharactor(Charactor ch) {
		mCharactor = ch;
	}
	Field::Trout::Charactor Field::Trout::getCharactor() const {
		return mCharactor;
	}



	Field::Field(double ox, double oy, const File& stageFile) : mLines(0), mOriginX(ox), mOriginY(oy), mImage(0){
		mLines = new Line[gLineNumber + 1];
		int maxN = gLineNumber / 2;
		double delta = gFieldTheta / static_cast<double>(gLineNumber);
		for (int x = 0; x < gLineNumber + 1; x++) {
			if (x <= maxN) {
				mLines[x].setSlope(gFieldTheta / 2.0 - delta * x);
			}
			else {
				mLines[x].setSlope(-1.0 * (gFieldTheta / 2.0 - delta * (gLineNumber - x)));
			}
			mLines[x].setInterceptX(static_cast<double>(x * gFieldWidth));
		}

		mTrouts.setSize(gLineNumber, gLayerNumber);
		mCharactors.setSize(gLineNumber, gLayerNumber);
		for (int y = 0; y < gLayerNumber; y++) {
			for (int x = 0; x < gLineNumber; x++) {
				mTrouts(x, y).setCoordinate(
					mLines[x].getX(y),
					mLines[x + 1].getX(y),
					mLines[x].getX(y + 1),
					mLines[x + 1].getX(y + 1),
					mLines[x].getY(y),
					mLines[x].getY(y + 1));
				mCharactors(x, y).setCharactor(Trout::NONE);
			}
		}

		int x = 0, y = 0;
		int i = 0;
		while ('e' != stageFile.data()[i]) {
			switch (stageFile.data()[i]) {
			case '2': mTrouts(x, y).initializeState(Trout::HOLE); ++x; break;
			case '0': case 'p': mTrouts(x, y).initializeState(Trout::PLAYER_AREA); ++x; break;
			case '1': case 'b': case 't': case 'l': case 'r': mTrouts(x, y).initializeState(Trout::ENEMY_AREA); ++x; break;
			case 'n': ++y; x = 0; break;
			}
			++i;
		}

		mImage = new Image("data/image/trout.dds");
	}
	Field::~Field() {
		SAFE_DELETE(mImage);
		SAFE_DELETE(mLines);
	}
	void Field::moveOrigin(double dx) {
		double left = mOriginX + mLines[0].getX(gLayerNumber) + dx;
		double right = mOriginX + mLines[gLineNumber].getX(gLayerNumber) + dx;
		
		if (left > 100.0 || right + 100.0 < Framework::instance().width()) {
			return;
		}

		mOriginX += dx;
	}
	Vector2 Field::getOrigin() const {
		return Vector2(mOriginX, mOriginY);
	}
	void Field::setAttackFlag(int x, int y) {
		mTrouts(x, y).setFlag(Trout::ATTACKED);
	}
	void Field::resetAttackFlag(int x, int y) {
		mTrouts(x, y).resetFlag(Trout::ATTACKED);
	}
	int Field::getTroutState(int x, int y) const {
		return mTrouts(x, y).getState();
	}
	void Field::enemyChangeField(int x, int y) {
		if (mTrouts(x, y).getState() & Trout::PLAYER_AREA) {
			mTrouts(x, y).resetFlag(Trout::PLAYER_AREA);
			mTrouts(x, y).setFlag(Trout::ENEMY_AREA);
		}
	}
	void Field::playerChangeField(int x, int y) {
		if (mTrouts(x, y).getState() & Trout::ENEMY_AREA) {
			mTrouts(x, y).resetFlag(Trout::ENEMY_AREA);
			mTrouts(x, y).setFlag(Trout::PLAYER_AREA);
		}
	}
	Field::Trout::Charactor Field::getCharactor(int x, int y) const {
		return mCharactors(x, y).getCharactor();
	}
	void Field::setCharactor(int x, int y, Trout::Charactor ch) {
		mCharactors(x, y).setCharactor(ch);
	}
	void Field::draw() const {
		for (int y = 0; y < gLayerNumber; y++) {
			for (int x = 0; x < gLineNumber; x++) {
				mTrouts(x, y).draw(*mImage, Vector2(mOriginX, mOriginY));
			}
		}
	}
	Vector2* Field::getTroutCoordinate(int x, int y) const {
		return mTrouts(x, y).getCoordinate();
	}
	double Field::getLineX(int x, double y) const {
		return mLines[x].getX(y);
	}
}