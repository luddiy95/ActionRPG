#include "Game/Enemy/Turtle.h"
#include "Game/Player.h"
#include "Game/GameParent.h"
#include "Image.h"
#include "Vector2.h"
#include "Matrix23.h"
#include "Game/Field.h"
#include "GameLib/Framework.h"
#include "Font.h"
#include "sstream"
using namespace GameLib;

namespace {
	int Round(double a) {
		return (a > 0) ? static_cast<int>(a + 0.5) : static_cast<int>(a - 0.5);
	}
}

namespace Game {
	namespace Enemy {
		Turtle::Body::Body(int x, int y, Field* field) : mx(x), my(y), mtx(x), nextmdx(0), moveDistance(0), dx(0),
			mImageStatus(1), mImageStatusCount(0), HP(150),
			mMoveCount(0), mAttackCount(0), isAttackFrame(false), mImage(0){

			field->setCharactor(x, y, Field::Trout::ENEMY_TURTLE);

			Vector2* troutCoordinate = field->getTroutCoordinate(mtx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
			double troutWidth = field->getLineX(mtx + 1, ty) - field->getLineX(mtx, ty);
			mdx = field->getLineX(mtx, ty) + troutWidth / 2.0;
			mImage = new Image("data/image/turtleBody.dds");
		}
		Turtle::Body::~Body() {
			SAFE_DELETE(mImage);
		}
		void Turtle::Body::update(const Player& player, Field* field) {

			if ((mtx >= 2 && 
				(field->getTroutState(mtx - 1, my) & Game::Field::Trout::PLAYER_AREA)) || 
					(250 <= mAttackCount && mAttackCount < 250 + robAreaFrame)) { //攻撃最中の50フレーム間に前のエリアがenemyに変わってしまう
				if (mMoveCount > 0) {
					mMoveCount = 0;
				}
				++mAttackCount;
				if (200 <= mAttackCount && mAttackCount < 250) {
					field->setAttackFlag(mtx - 1, my);
				}
				else if (250 <= mAttackCount && mAttackCount < 250 + robAreaFrame) {
					field->setAttackFlag(mtx - 1, my);
					if (mAttackCount == 250 && (field->getTroutState(mtx - 1, my) & Game::Field::Trout::PLAYER_AREA)) {
						isAttackFrame = true;
					}
					mImageStatus = 0;
				}
				else if (mAttackCount == 250 + robAreaFrame) {
					field->setAttackFlag(mtx - 1, my);
					mAttackCount = 0;
					mImageStatus = 3;
				}
				else {
					mImageStatus = 3;
				}
			}
			else if(mtx >= 1 && (field->getTroutState(mtx - 1, my) & Game::Field::Trout::ENEMY_AREA) && 
						!(250 <= mAttackCount && mAttackCount < 250 + robAreaFrame)) { //攻撃最中の50フレーム間(この間に前のマスがenemyに急に変わるかもしれない)
				if (mAttackCount > 0) { //亀以外の力で前のエリアが突然playerからenemyに変わったら
					mAttackCount = 0;
				}
				++mMoveCount;
				if (mMoveCount >= 300) {
					if (mMoveCount == 300) {
						Vector2* troutCoordinate = field->getTroutCoordinate(mtx - 1, my);
						double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
						double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
						double troutWidth = field->getLineX(mtx + 1 - 1, ty) - field->getLineX(mtx - 1, ty);
						nextmdx = field->getLineX(mtx - 1, ty) + troutWidth / 2.0;
						moveDistance = mdx - nextmdx;
						dx = moveDistance / 200.0;
					}
					if (mMoveCount % 30 == 0) {
						++mImageStatusCount;
						mImageStatus = (mImageStatusCount % 3) + 1;
					}
					mdx -= dx;

					if (mdx > nextmdx + moveDistance * 4.0 / 6.0) {
						if (field->getTroutState(mtx - 1, my) & Game::Field::Trout::PLAYER_AREA) {
							//この時点で突然前がplayerのエリアになったら
							mdx = nextmdx + moveDistance;
							mMoveCount = 0;
						}
					}
					else if (nextmdx + moveDistance * 4.0 / 6.0 - dx < mdx && mdx <= nextmdx + moveDistance * 4.0 / 6.0) {
						if (field->getCharactor(mtx - 1, my) == Field::Trout::NONE) {
							//この時点では(mtx - 1, my)をplayerにすることはできない(亀がいるから)
							field->setCharactor(mtx - 1, my, Field::Trout::ENEMY_TURTLE);
							--mx; //turtleは左にしか進まないからこの更新だけでok
						}
						else {
							mdx = nextmdx + moveDistance;
							mMoveCount = 0;
						}
					}
					else if (nextmdx + moveDistance * 1.0 / 6.0 - dx < mdx && mdx <= nextmdx + moveDistance * 1.0 / 6.0) {
						field->setCharactor(mtx, my, Field::Trout::NONE);
					}
					
					if (nextmdx - dx < mdx && mdx <= nextmdx) { //目的のマスにたどり着いた瞬間
						--mtx;
						mMoveCount = 0;
						nextmdx = 0;
						moveDistance = 0;
						dx = 0;
						mImageStatus = 3;
					}
				}
				else {
					mImageStatus = 3;
				}
			}
			if ((field->getTroutState(mtx - 1, my) & Field::Trout::PLAYER_AREA) &&
				(nextmdx > 0 && (mdx - nextmdx) > moveDistance * 4.0 / 6.0)) {
				mdx = nextmdx + moveDistance;
				mAttackCount = 0;
				mMoveCount = 0;
				nextmdx = 0;
				dx = 0;
			}
		}
		void Turtle::Body::draw(const Field& field) const {

			Vector2* troutCoordinate = field.getTroutCoordinate(mtx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y; //敵が立っているy
			double displayWidth = mWidth * pow(gEnemyScale, static_cast<double>(my));
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(my));
			mImage->draw(field.getOrigin().x + mdx - displayWidth / 2.0, field.getOrigin().y + ty - displayHeight,
				displayWidth, displayHeight,
				mImageStatus * mWidth + 1.0, 1 * mHeight, mWidth - 3.0, mHeight); //画像の都合上微調整

			ostringstream oss;
			oss << HP;
			Font::instance()->drawString(field.getOrigin().x + mdx, field.getOrigin().y + ty - displayHeight - 8, 4, 9, oss.str().c_str());
			
		}
		void Turtle::Body::changeAttackFrame() {
			isAttackFrame = !isAttackFrame;
		}
		bool Turtle::Body::attackFrame() const {
			return isAttackFrame;
		}
		int Turtle::Body::mtX() const {
			return mtx;
		}
		int Turtle::Body::mX() const {
			return mx;
		}
		int Turtle::Body::mY() const {
			return my;
		}
		double Turtle::Body::mdX() const {
			return mdx;
		}
		void Turtle::Body::damage(int d) {
			HP -= d;
		}
		int Turtle::Body::getHP() const {
			return HP;
		}



		Turtle::AttackTrout::AttackTrout(int x, int y, const Field& field) : mx(x), my(y), mAttackTroutCount(0),
			vanish(false), mImage(0),
			m0(field.getTroutCoordinate(x, y)[0]),
			m1(field.getTroutCoordinate(x, y)[1]),
			m2(field.getTroutCoordinate(x, y)[2]),
			m3(field.getTroutCoordinate(x, y)[3]){
			mTranslation.setVector(0.0, -mFirstDistance);
			mImage = new Image("data/image/attackTrout.dds");
		}
		Turtle::AttackTrout::~AttackTrout() {
			SAFE_DELETE(mImage);
		}
		int Turtle::AttackTrout::mX() const {
			return mx;
		}
		int Turtle::AttackTrout::mY() const {
			return my;
		}
		void Turtle::AttackTrout::update() {
			++mAttackTroutCount;
			if (mAttackTroutCount <= robAreaFrame) {
				mTranslation.setVector(0.0,
					-1.0 * (mFirstDistance - mFirstDistance * static_cast<double>(mAttackTroutCount) / static_cast<double>(robAreaFrame)));
				if (mAttackTroutCount == robAreaFrame) {
					mAttackTroutCount = 0;
					this->vanish = true;
				}
			}
		}
		void Turtle::AttackTrout::draw(const Field& field) {
			Matrix23 translate;
			translate.setTranslation(Vector2(mTranslation.x + field.getOrigin().x, mTranslation.y + field.getOrigin().y));
			Vector2 c0, c1, c2, c3;
			translate.multiply(&c0, m0);
			translate.multiply(&c1, m1);
			translate.multiply(&c2, m2);
			translate.multiply(&c3, m3);
			mImage->draw(c0, c1, c2, c3, 0 * gFieldWidth, 1 * gFieldHeight, gFieldWidth, gFieldHeight);
		}
		bool Turtle::AttackTrout::isVanished() const {
			return vanish;
		}



		Turtle::Turtle(int x, int y, int n, Field* field) : mBody(0), mAttackTrout(0), index(n) {
			mBody = new Body(x, y, field);
		}
		Turtle::~Turtle() {
			SAFE_DELETE(mBody);
		}
		int Turtle::bodyX() const {
			return mBody->mX();
		}
		int Turtle::bodyY() const {
			return mBody->mY();
		}
		int Turtle::attackY() const {
			if (mAttackTrout) {
				return mAttackTrout->mY();
			}
			else {
				return -1;
			}
		}
		double Turtle::mdX(const Field&) const {
			return mBody->mdX();
		}
		double Turtle::mdY(const Field& field) const {

			int mtx = mBody->mtX(), my = mBody->mY();
			Vector2* troutCoordinate = field.getTroutCoordinate(mtx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			return troutHeight * gEnemyDepth + troutCoordinate[0].y;
		}
		double Turtle::getDisplayWidth() const {
			return mWidth * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		double Turtle::getDisplayHeight() const {
			return mHeight * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		void Turtle::update(const Player& player, Field* field) {

			if (mBody && !deleted) {
				mBody->update(player, field);
				if (mBody->attackFrame() && !mAttackTrout) {
					mBody->changeAttackFrame();
					mAttackTrout = new AttackTrout(mBody->mtX() - 1, mBody->mY(), *field);
				}
				if (mAttackTrout) {
					mAttackTrout->update();
				}
				if (mBody->getHP() <= 0) {
					enemyDelete(field);
				}
			}
		}
		void Turtle::drawBody(const Field& field) const {
			if (mBody && !deleted) {
				mBody->draw(field);
			}
		}
		void Turtle::drawAttack(Game::GameParent::DrawAttackTiming timing, const Field& field) const {
			if (mAttackDrawTiming == timing) {
				if (mAttackTrout) {
					mAttackTrout->draw(field);
				}
			}
		}
		void Turtle::attackCollision(Player* player, Field* field) {

			if (mAttackTrout) {
				if (mAttackTrout->isVanished()) { //目的のマスにたどり着いた
					if (mAttackTrout->mX() == player->mX() && mAttackTrout->mY() == player->mY()) {
						player->decreaseHP(mDamage);
					}
					else {
						field->enemyChangeField(mAttackTrout->mX(), mAttackTrout->mY());
					}
					SAFE_DELETE(mAttackTrout);
				}
			}
		}
		void Turtle::damage(int d) {
			mBody->damage(d);
		}
		int Turtle::getIndex() const {
			return index;
		}
		void Turtle::enemyDelete(Field* field) {
			if (!deleted) {
				field->setCharactor(mBody->mX(), mBody->mY(), Field::Trout::NONE);
				deleted = true;
			}
		}
		bool Turtle::isDeleted() const {
			return deleted;
		}
	}
}