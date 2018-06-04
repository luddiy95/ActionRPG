#include "Game/Enemy/Lama.h"
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
	bool percent(int a) {
		Framework f = Framework::instance();
		return (f.getRandom(100) <= a);
	}
}

namespace Game {
	namespace Enemy {
		Lama::Body::Body(int x, int y, Field* field) : mx(x), my(y), mtx(x), mty(y), 
			nextmdx(0), nextmdy(0), moveDistanceX(0), moveDistanceY(0), dx(0), dy(0),
			mImageStatus(0), mDirectionX(LEFT), mDirectionY(NONEY), mImageStatusCount(0), HP(80),
			mMoveCount(0), mAttackCount(0), mDirectionCount(0), isAttackFrame(false), mImage(0) {

			field->setCharactor(x, y, Field::Trout::ENEMY_LAMA);

			Vector2* troutCoordinate = field->getTroutCoordinate(mtx, mty);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(mty));
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
			double troutWidth = field->getLineX(mtx + 1, ty) - field->getLineX(mtx, ty);
			mdx = field->getLineX(mtx, ty) + troutWidth / 2.0;
			mdy = ty - displayHeight / 2.0;
			mImage = new Image("data/image/lamaBody.dds");
		}
		Lama::Body::~Body() {
			SAFE_DELETE(mImage);
		}
		void Lama::Body::update(const Player& player, Field* field) {

			if (mDirectionCount > 0) {
				++mDirectionCount;
				if (mDirectionCount == 30) {
					Framework f = Framework::instance();
					switch (f.getRandom(4)) {
					case 0: mDirectionX = LEFT; mDirectionY = NONEY; break;
					case 1: mDirectionX = RIGHT; mDirectionY = NONEY; break;
					case 2: mDirectionX = NONEX; mDirectionY = BACK; break;
					case 3: mDirectionX = NONEX; mDirectionY = FRONT; break;
					}
					mDirectionCount = 0;
				}
				return;
			}

			if (((mtx >= 2 && mDirectionX == LEFT &&
					(field->getTroutState(mtx + mDirectionX, mty) & Game::Field::Trout::PLAYER_AREA)) ||
				(mtx <= gLineNumber - 2 && mDirectionX == RIGHT &&
					(field->getTroutState(mtx + mDirectionX, mty) & Game::Field::Trout::PLAYER_AREA)) ||
				(mty >= 2 && mDirectionY == BACK &&
					(field->getTroutState(mtx, mty + mDirectionY) & Game::Field::Trout::PLAYER_AREA)) ||
				(mty <= gLayerNumber - 2 && mDirectionY == FRONT &&
						field->getTroutState(mtx, mty + mDirectionY) & Game::Field::Trout::PLAYER_AREA) ||
				(100 <= mAttackCount && mAttackCount < 100 + robAreaFrame))){//攻撃最中の50フレーム間に前のエリアがenemyに変わってしまう

				if (mMoveCount > 0) {
					mMoveCount = 0;
				}

				if (percent(robPercent) && mAttackCount == 0) {
					mAttackCount = 1;
				}
				if (mAttackCount > 0) {
					++mAttackCount;
					if (50 <= mAttackCount && mAttackCount < 100) {
						field->setAttackFlag(mtx + mDirectionX, mty + mDirectionY);
					}
					else if (100 <= mAttackCount && mAttackCount < 100 + robAreaFrame) {
						field->setAttackFlag(mtx + mDirectionX, mty + mDirectionY);
						if (mAttackCount == 100 &&
							(field->getTroutState(mtx + mDirectionX, mty + mDirectionY) & Game::Field::Trout::PLAYER_AREA)) {
							isAttackFrame = true;
						}
						mImageStatus = 1;
					}
					else if (mAttackCount == 100 + robAreaFrame) {
						field->setAttackFlag(mtx + mDirectionX, mty + mDirectionY);
						mAttackCount = 0;
						mImageStatus = 0;
					}
					else {
						;
					}
				}
				else {
					mAttackCount = 0;
					mMoveCount = 0;
					mDirectionCount = 1;
				}

			}else if (0 <= mtx + mDirectionX && mtx + mDirectionX <= gLineNumber - 1 &&
						0 <= mty + mDirectionY && mty + mDirectionY <= gLayerNumber - 1 &&
						(field->getTroutState(mtx + mDirectionX, mty + mDirectionY) & Field::Trout::ENEMY_AREA) &&
						!(100 <= mAttackCount && mAttackCount < 100 + robAreaFrame)) { //攻撃最中の50フレーム間(この間に前のマスがenemyに急に変わるかもしれない)

				mAttackCount = 0; //リャマ以外の力で前のエリアが突然playerからenemyに変わったら
				++mMoveCount;
				if (dx == 0 && dy == 0) {
					Vector2* troutCoordinate = field->getTroutCoordinate(mtx + mDirectionX, mty + mDirectionY);
					double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
					double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
					double troutWidth = field->getLineX(mtx + mDirectionX + 1, ty) - field->getLineX(mtx + mDirectionX, ty);
					nextmdx = field->getLineX(mtx + mDirectionX, ty) + troutWidth / 2.0;
					moveDistanceX = nextmdx - mdx; //正負つき
					dx = moveDistanceX / static_cast<double>(moveAreaFrame); //正負つき

					if (mDirectionY != NONEY) {
						Vector2* troutCoordinate = field->getTroutCoordinate(mtx, mty + mDirectionY);
						double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
						double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(mty + mDirectionY));
						double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
						nextmdy = ty - displayHeight / 2.0;
						moveDistanceY = nextmdy - mdy; //正負つき
						dy = moveDistanceY / static_cast<double>(moveAreaFrame);
					}
				}

				if (mMoveCount >= moveAreaFrame) {
					if (mMoveCount % 10 == 0) {
						++mImageStatusCount;
						mImageStatus = (mImageStatusCount % 3) + 1;
					}
					mdx += dx;
					mdy += dy;
				}
				
				if (mDirectionX != NONEX){
					if ((nextmdx - mdx) * mDirectionX > moveDistanceX * mDirectionX * 4.0 / 6.0) {
						if (field->getTroutState(mtx + mDirectionX, mty) & Game::Field::Trout::PLAYER_AREA) {
							//この時点で突然前がplayerのエリアになったら(ここをこえたらLamaがいるからエリアは奪えない)
							mdx = nextmdx - moveDistanceX;
							mMoveCount = 0;
							nextmdx = nextmdy = 0;
							moveDistanceX = moveDistanceY = 0;
							dx = dy = 0;
						}
					}
					else if (moveDistanceX * mDirectionX * 4.0 / 6.0 - dx * mDirectionX < (nextmdx - mdx) * mDirectionX &&
								(nextmdx - mdx) * mDirectionX <= moveDistanceX * mDirectionX * 4.0 / 6.0) {
						if (field->getCharactor(mtx + mDirectionX, mty) == Field::Trout::NONE) {
							field->setCharactor(mtx + mDirectionX, mty, Field::Trout::ENEMY_LAMA);
							mx += mDirectionX;
						}
						else {
							mdx = nextmdx - moveDistanceX;
							mMoveCount = 0;
							nextmdx = nextmdy = 0;
							moveDistanceX = moveDistanceY = 0;
							dx = dy = 0;

							mDirectionCount = 1;
						}
					}
					else if (moveDistanceX * mDirectionX * 1.0 / 6.0 - dx * mDirectionX < (nextmdx - mdx) * mDirectionX &&
								(nextmdx - mdx) * mDirectionX <= moveDistanceX * mDirectionX * 1.0 / 6.0) {
						field->setCharactor(mtx, mty, Field::Trout::NONE);
					}
					else if (-dx * mDirectionX < (nextmdx - mdx) * mDirectionX &&
								(nextmdx - mdx) * mDirectionX <= 0) { //目的のマスにたどり着いた瞬間
						mtx += mDirectionX;
						nextmdx = nextmdy = 0;
						moveDistanceX = moveDistanceY = 0;
						dx = dy = 0;
					}
				}
				else if (mDirectionY != NONEY) {
					if ((nextmdy - mdy) * mDirectionY > moveDistanceY * mDirectionY * 4.0 / 6.0) {
						if (field->getTroutState(mtx, mty + mDirectionY) & Game::Field::Trout::PLAYER_AREA) {
							//この時点で突然前がplayerのエリアになったら
							mdy = nextmdy - moveDistanceY;
							mdx = nextmdx - moveDistanceX;
							mMoveCount = 0;
							nextmdx = nextmdy = 0;
							moveDistanceX = moveDistanceY = 0;
							dx = dy = 0;
						}
					}
					else if (moveDistanceY * mDirectionY * 4.0 / 6.0 - dy * mDirectionY < (nextmdy - mdy) * mDirectionY &&
						(nextmdy - mdy) * mDirectionY <= moveDistanceY * mDirectionY * 4.0 / 6.0) {
						if (field->getCharactor(mtx, mty + mDirectionY) == Field::Trout::NONE) {
							field->setCharactor(mtx, mty + mDirectionY, Field::Trout::ENEMY_LAMA);
							my += mDirectionY;
						}
						else {
							mdy = nextmdy - moveDistanceY;
							mdx = nextmdx - moveDistanceX;
							mMoveCount = 0;
							nextmdx = nextmdy = 0;
							moveDistanceX = moveDistanceY = 0;
							dx = dy = 0;

							mDirectionCount = 1;
						}
					}
					else if (moveDistanceY * mDirectionY * 1.0 / 6.0 - dy * mDirectionY < (nextmdy - mdy) * mDirectionY &&
						(nextmdy - mdy) * mDirectionY <= moveDistanceY * mDirectionY * 1.0 / 6.0) {
						field->setCharactor(mtx, mty, Field::Trout::NONE);
					}
					else if (-dy * mDirectionY < (nextmdy - mdy) * mDirectionY &&
						(nextmdy - mdy) * mDirectionY <= 0) { //目的のマスにたどり着いた瞬間
						mty += mDirectionY;
						nextmdx = 0;
						nextmdy = 0;
						moveDistanceX = 0;
						moveDistanceY = 0;
						dx = 0;
						dy = 0;
					}
				}
			}
			else {
				mAttackCount = 0;
				mMoveCount = 0;
				mDirectionCount = 1;
			}
			if ((field->getTroutState(mtx + mDirectionX, mty + mDirectionY) & Field::Trout::PLAYER_AREA) &&
				((nextmdx > 0 && (nextmdx - mdx) * mDirectionX > moveDistanceX * mDirectionX * 4.0 / 6.0) ||
				(nextmdy > 0 && (nextmdy - mdy) * mDirectionY > moveDistanceY * mDirectionY * 4.0 / 6.0))) {
				if (mDirectionX != NONEX) {
					mdx = nextmdx - moveDistanceX;
				}
				else if (mDirectionY != NONEY) {
					mdx = nextmdx - moveDistanceX;
					mdy = nextmdy - moveDistanceY;
				}
				mAttackCount = 0;
				mMoveCount = 0;
				nextmdx = 0;
				nextmdy = 0;
				moveDistanceX = 0;
				moveDistanceY = 0;
				dx = 0;
				dy = 0;
			}
			
		}
		void Lama::Body::draw(const Field& field) const {

			Vector2* troutCoordinate = field.getTroutCoordinate(mtx, my); //mtyにすると大きさが変わる瞬間が変
			double displayWidth = mWidth * pow(gEnemyScale, static_cast<double>(my));
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(my));
			unsigned direction = 0;
			if (mDirectionX == NONEX) {
				if (mDirectionY == BACK) {
					direction = 3;
				}
				else if (mDirectionY == FRONT) {
					direction = 0;
				}
				else {
					direction = 1;
				}
			}
			else if (mDirectionX == LEFT) {
				direction = 1;
			}
			else if (mDirectionX == RIGHT) {
				direction = 2;
			}
			mImage->draw(field.getOrigin().x + mdx - displayWidth / 2.0, field.getOrigin().y + mdy - displayHeight / 2.0,
				displayWidth, displayHeight,
				mImageStatus * mWidth, direction * mHeight, mWidth, mHeight);

			ostringstream oss;
			oss << HP;
			Font::instance()->drawString(field.getOrigin().x + mdx, field.getOrigin().y + mdy - displayHeight / 2.0, 4, 9, oss.str().c_str());
		}
		void Lama::Body::changeAttackFrame() {
			isAttackFrame = !isAttackFrame;
		}
		bool Lama::Body::attackFrame() const {
			return isAttackFrame;
		}
		int Lama::Body::mtX() const {
			return mtx;
		}
		int Lama::Body::mtY() const {
			return mty;
		}
		int Lama::Body::mX() const {
			return mx;
		}
		int Lama::Body::mY() const {
			return my;
		}
		double Lama::Body::mdX() const {
			return mdx;
		}
		double Lama::Body::mdY() const {
			return mdy;
		}
		Lama::Body::DirectionX Lama::Body::getDirectionX() const {
			return mDirectionX;
		}
		Lama::Body::DirectionY Lama::Body::getDirectionY() const {
			return mDirectionY;
		}
		void Lama::Body::damage(int d) {
			HP -= d;
		}
		int Lama::Body::getHP() const {
			return HP;
		}



		Lama::AttackTrout::AttackTrout(int x, int y, const Field& field) : mx(x), my(y), mAttackTroutCount(0),
			vanish(false), mImage(0),
			m0(field.getTroutCoordinate(x, y)[0]),
			m1(field.getTroutCoordinate(x, y)[1]),
			m2(field.getTroutCoordinate(x, y)[2]),
			m3(field.getTroutCoordinate(x, y)[3]) {
			mTranslation.setVector(0.0, -mFirstDistance);
			mImage = new Image("data/image/attackTrout.dds");
		}
		Lama::AttackTrout::~AttackTrout() {
			SAFE_DELETE(mImage);
		}
		int Lama::AttackTrout::mX() const {
			return mx;
		}
		int Lama::AttackTrout::mY() const {
			return my;
		}
		void Lama::AttackTrout::update() {
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
		void Lama::AttackTrout::draw(const Field& field) {

			Matrix23 translate;
			translate.setTranslation(Vector2(mTranslation.x + field.getOrigin().x, mTranslation.y + field.getOrigin().y));
			Vector2 c0, c1, c2, c3;
			translate.multiply(&c0, m0);
			translate.multiply(&c1, m1);
			translate.multiply(&c2, m2);
			translate.multiply(&c3, m3);
			mImage->draw(c0, c1, c2, c3, 0 * gFieldWidth, 1 * gFieldHeight, gFieldWidth, gFieldHeight);
		}
		bool Lama::AttackTrout::isVanished() const {
			return vanish;
		}



		Lama::Lama(int x, int y, int n, Field* field) : mBody(0), mAttackTrout(0), index(n) {
			mBody = new Body(x, y, field);
		}
		Lama::~Lama() {
			SAFE_DELETE(mBody);
		}
		int Lama::bodyX() const {
			return mBody->mX();
		}
		int Lama::bodyY() const {
			return mBody->mY();
		}
		int Lama::attackY() const {
			if (mAttackTrout) {
				return mAttackTrout->mY();
			}
			else {
				return -1;
			}
		}
		double Lama::mdX(const Field&) const {
			return mBody->mdX();
		}
		double Lama::mdY(const Field&) const {
			return mBody->mdY();
		}
		double Lama::getDisplayWidth() const {
			return mWidth * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		double Lama::getDisplayHeight() const {
			return mHeight * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		void Lama::update(const Player& player, Field* field) {

			if (mBody && !deleted) {
				mBody->update(player, field);
				if (mBody->attackFrame() && !mAttackTrout) {
					mBody->changeAttackFrame();
					mAttackTrout = new AttackTrout(mBody->mtX() + mBody->getDirectionX(), mBody->mtY() + mBody->getDirectionY(), *field);
				}
				if (mAttackTrout) {
					mAttackTrout->update();
				}
				if (mBody->getHP() <= 0) {
					enemyDelete(field);
				}
			}
		}
		void Lama::drawBody(const Field& field) const {
			if (mBody && !deleted) {
				mBody->draw(field);
			}
		}
		void Lama::drawAttack(Game::GameParent::DrawAttackTiming timing, const Field& field) const {
			if (mAttackDrawTiming == timing) {
				if (mAttackTrout) {
					mAttackTrout->draw(field);
				}
			}
		}
		void Lama::attackCollision(Player* player, Field* field) {

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
		void Lama::damage(int d) {
			mBody->damage(d);
		}
		int Lama::getIndex() const {
			return index;
		}
		void Lama::enemyDelete(Field* field) {
			if (!deleted) {
				field->setCharactor(mBody->mX(), mBody->mY(), Field::Trout::NONE);
				deleted = true;
			}
		}
		bool Lama::isDeleted() const {
			return deleted;
		}
	}
}