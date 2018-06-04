#include "Game/Enemy/Bear.h"
#include "Game/Player.h"
#include "Game/GameParent.h"
#include "Image.h"
#include "Vector2.h"
#include "Matrix23.h"
#include "Game/Field.h"
#include "Font.h"
#include <sstream>
#include "GameLib/Framework.h"
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
		Bear::Body::Body(int x, int y, Field* field) : mx(x), my(y), mtx(x), mty(y), HP(80),
			nextmdx(0), nextmdy(0), moveDistanceX(0), moveDistanceY(0), dx(0), dy(0),
			mImageStatus(0), mDirectionX(NONEX), mDirectionY(FRONT), mImageStatusCount(0), mFirstFrame(true),
			mIntervalCount(0), mAttack(false), afterAttack(false), isAttackFrame(false), mImage(0) {

			field->setCharactor(x, y, Field::Trout::ENEMY_LAMA);

			Vector2* troutCoordinate = field->getTroutCoordinate(mtx, mty);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(mty));
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
			double troutWidth = field->getLineX(mtx + 1, ty) - field->getLineX(mtx, ty);
			mdx = field->getLineX(mtx, ty) + troutWidth / 2.0;
			mdy = ty - displayHeight / 2.0;
			SAFE_DELETE(mImage);
			mImage = new Image("data/image/bearBody.dds");
		}
		Bear::Body::~Body() {
			SAFE_DELETE(mImage);
		}
		void Bear::Body::update(const Player& player, Field* field) {

			if (afterAttack) {
				++mIntervalCount;
				if (mIntervalCount == 50) {
					afterAttack = false;
					mIntervalCount = 0;
				}
				return;
			}

			if (!mAttack && mIntervalCount < moveAreaFrame) { //最初と攻撃直後は縦方向を向いてインターバルがある
				if (mIntervalCount == 0) {
					if (mFirstFrame) { //一番最初は方向変更しない
						mFirstFrame = false;
					}
					else {
						mDirectionX = NONEX;
						if (mty == 0) {
							mDirectionY = FRONT;
						}
						else if (mty == gLayerNumber - 1) {
							mDirectionY = BACK;
						}
						else {
							Framework f = Framework::instance();
							mDirectionY = (f.getRandom(2) == 0) ? BACK : FRONT;
						}
					}

					Vector2* troutCoordinate = field->getTroutCoordinate(mtx, mty + mDirectionY);
					double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
					double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
					double troutWidth = field->getLineX(mtx + 1, ty) - field->getLineX(mtx, ty);
					nextmdx = field->getLineX(mtx, ty) + troutWidth / 2.0;
					moveDistanceX = nextmdx - mdx; //正負つき
					dx = moveDistanceX / static_cast<double>(moveAreaFrame);

					double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(mty + mDirectionY));
					nextmdy = ty - displayHeight / 2.0;
					moveDistanceY = nextmdy - mdy; //正負つき
					dy = moveDistanceY / static_cast<double>(moveAreaFrame);
				}
				++mIntervalCount;

				return;
			}

			if (!mAttack) {
				if (dx == 0 && dy == 0) { //1マス移動ごとに
					if (percent(attackPercent)) { //インターバル直後はdx == 0 && dy == 0は成り立たない
						mAttack = true;
						return;
					}
					else if (mty == 0) {
						mDirectionX = NONEX;
						mDirectionY = FRONT;
					}
					else if (mty == gLayerNumber - 1) {
						mDirectionX = NONEX;
						mDirectionY = BACK;
					}
					else {
						//それ以外は進行方向そのまま
					}

					Vector2* troutCoordinate = field->getTroutCoordinate(mtx, mty + mDirectionY);
					double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
					double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
					double troutWidth = field->getLineX(mtx + 1, ty) - field->getLineX(mtx, ty);
					nextmdx = field->getLineX(mtx, ty) + troutWidth / 2.0;
					moveDistanceX = nextmdx - mdx; //正負つき
					dx = moveDistanceX / static_cast<double>(moveAreaFrame);

					double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(mty + mDirectionY));
					nextmdy = ty - displayHeight / 2.0;
					moveDistanceY = nextmdy - mdy; //正負つき
					dy = moveDistanceY / static_cast<double>(moveAreaFrame);
				}

				++mImageStatusCount;
				if (mImageStatusCount % 10 == 0) {
					++mImageStatusCount;
					mImageStatus = (mImageStatusCount % 3) + 1;
				}
				mdx += dx;
				mdy += dy;

				if ((nextmdy - mdy) * mDirectionY > moveDistanceY * mDirectionY * 4.0 / 6.0) {
					if (field->getTroutState(mtx, mty + mDirectionY) & Game::Field::Trout::PLAYER_AREA) {
						//この時点で突然前がplayerのエリアになったら
						mAttack = true;
						mdy = nextmdy - moveDistanceY;
						mdx = nextmdx - moveDistanceX;
						nextmdx = nextmdy = 0;
						moveDistanceX = moveDistanceY = 0;
						dx = dy = 0;
					}
				}
				else if (moveDistanceY * mDirectionY * 4.0 / 6.0 - dy * mDirectionY < (nextmdy - mdy) * mDirectionY &&
					(nextmdy - mdy) * mDirectionY <= moveDistanceY * mDirectionY * 4.0 / 6.0) {
					if (field->getCharactor(mtx, mty + mDirectionY) == Field::Trout::NONE) {
						field->setCharactor(mtx, mty + mDirectionY, Field::Trout::ENEMY_BEAR);
						my += mDirectionY;
					}
					else {
						mAttack = true;
						mdy = nextmdy - moveDistanceY;
						mdx = nextmdx - moveDistanceX;
						nextmdx = nextmdy = 0;
						moveDistanceX = moveDistanceY = 0;
						dx = dy = 0;
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
			if (mAttack && (mIntervalCount != 0)) {
				mIntervalCount = 0;
				isAttackFrame = true;
			}
		}
		void Bear::Body::draw(const Field& field) const {

			Vector2* troutCoordinate = field.getTroutCoordinate(mtx, my); //mtyにすると大きさが変わる瞬間が変
			unsigned direction = 0;
			if (mDirectionY == BACK) {
				direction = 3;
			}
			else if (mDirectionY == FRONT) {
				direction = 0;
			}
			else if (mDirectionX == LEFT) {
				direction = 1;
			}
			double displayWidth = mWidth * pow(gEnemyScale, static_cast<double>(my));
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(my));
			mImage->draw(field.getOrigin().x + mdx - displayWidth / 2.0, field.getOrigin().y + mdy - displayHeight / 2.0,
				displayWidth, displayHeight,
				mImageStatus * mWidth, direction * mHeight, mWidth, mHeight);

			ostringstream oss;
			oss << ((HP > 0) ? HP : 0);
			Font::instance()->drawString(field.getOrigin().x + mdx - 4, field.getOrigin().y + mdy - displayHeight / 2.0 - 8, 4, 9, oss.str().c_str());
		}
		void Bear::Body::changeAttackFrame() {
			isAttackFrame = !isAttackFrame;
		}
		bool Bear::Body::getAttackFrame() const {
			return isAttackFrame;
		}
		void Bear::Body::AttackEnd() {
			mAttack = false;
			afterAttack = true;
		}
		int Bear::Body::mtX() const {
			return mtx;
		}
		int Bear::Body::mtY() const {
			return mty;
		}
		int Bear::Body::mX() const {
			return mx;
		}
		int Bear::Body::mY() const {
			return my;
		}
		double Bear::Body::mdX() const {
			return mdx;
		}
		double Bear::Body::mdY() const {
			return mdy;
		}
		void Bear::Body::setDirectionX(DirectionX directionX) {
			mDirectionX = directionX;
		}
		void Bear::Body::setDirectionY(DirectionY directionY) {
			mDirectionY = directionY;
		}
		void Bear::Body::setImageStatus(int status) {
			mImageStatus = status;
		}
		void Bear::Body::damage(int d) {
			HP -= d;
		}
		int Bear::Body::getHP() const {
			return HP;
		}



		Bear::Sensor::Sensor(int x, int y, const Field& field) : mx(x), my(y), vanish(false), sensor(false), mSensorCount(0), mCount(0) {
			SAFE_DELETE(mImage);
			mImage = new Image("data/image/sensor1.dds");
			
			mdx = field.getTroutCoordinate(mx, my)[0].x;
			mdy = field.getTroutCoordinate(mx, my)[2].y;
		}
		Bear::Sensor::~Sensor() {
			SAFE_DELETE(mImage);
		}
		int Bear::Sensor::mX() const {
			return mx;
		}
		int Bear::Sensor::mY() const {
			return my;
		}
		bool Bear::Sensor::isVanished() const {
			return vanish;
		}
		void Bear::Sensor::Vanish() { //playerとの衝突判定でぶつかったときvanish
			vanish = true;
		}
		bool Bear::Sensor::isSensorOn() const {
			return sensor;
		}
		void Bear::Sensor::update() {

			++mSensorCount;
			if (mSensorCount < 35) {
				sensor = false;
			}
			else if (mSensorCount == 35) { 
				//sensorがtrueになるのはここだけ(sensorにplayerがぶつかったらfalseになるのでupdateでsensorをtrueにしない
				sensor = true;
			}
			else if (35 < mSensorCount && mSensorCount < 55) {

			}
			else if (mSensorCount == 55) {
				sensor = false;

				mSensorCount = 0;
				++mCount;
				if (mCount >= 3) {
					mCount = 0;
					vanish = true;
				}
			}
		}
		void Bear::Sensor::draw(const Field& field) const {

			if (sensor) {
				double minx = field.getTroutCoordinate(0, my)[2].x;
				double x = mdx;
				while (x >= minx) {
					mImage->draw((x - mWidth / 2.0) + field.getOrigin().x, (mdy - mHeight * 3.0 / 2.0 * pow(mDepth, static_cast<double>(my))) + field.getOrigin().y,
									0, 0, mWidth, mHeight);
					x -= mWidth;
				}
			}
		}


		Bear::Laser::Laser(int x, int y, const Field& field) : mx(x), my(y), mLaserCount(0), laser(false), vanish(false) {
			SAFE_DELETE(mImage);
			mImage = new Image("data/image/laser.dds");

			mdx = field.getTroutCoordinate(mx, my)[0].x;
			mdy = (field.getTroutCoordinate(mx, my)[0].y + field.getTroutCoordinate(mx, my)[2].y) / 2.0;
		}
		Bear::Laser::~Laser() {
			SAFE_DELETE(mImage);
		}
		int Bear::Laser::mX() const {
			return mx;
		}
		int Bear::Laser::mY() const {
			return my;
		}
		bool Bear::Laser::isLaserOn() const {
			return laser;
		}
		bool Bear::Laser::isVanished() const {
			return vanish;
		}
		void Bear::Laser::Vanish() {
			vanish = true;
		}
		void Bear::Laser::update() {
			++mLaserCount;
			if (mLaserCount < 25) {

			}
			else if (mLaserCount == 25) {
				laser = true;
			}
			else if (25 < mLaserCount && mLaserCount < 70) {

			}
			else if (mLaserCount == 70) {
				mLaserCount = 0;
				laser = false;
				vanish = true;
			}
		}
		void Bear::Laser::draw(const Field& field) const {
			if (laser) {
				double minx = field.getTroutCoordinate(0, my)[2].x;
				double x = mdx;
				while (x >= minx) {
					mImage->draw((x - mWidth / 2.0) + field.getOrigin().x, (mdy - mHeight * 3.0 / 5.0) + field.getOrigin().y,
						0, 0, mWidth, mHeight);
					x -= mWidth;
				}
			}
		}



		Bear::Bear(int x, int y, int n, Field* field) : mBody(0), mSensor(0), mLaser(0), mSensorCount(0),
			laserVanishing(false), laserVanishingCount(0), index(n), deleted(false) {
			SAFE_DELETE(mBody);
			mBody = new Body(x, y, field);
		}
		Bear::~Bear() {
			SAFE_DELETE(mBody);
			SAFE_DELETE(mSensor);
			SAFE_DELETE(mLaser);
		}
		int Bear::bodyX() const {
			return mBody->mX();
		}
		int Bear::bodyY() const {
			return mBody->mY();
		}
		int Bear::attackY() const {
			if (mSensor) {
				return mSensor->mY();
			}
			if (mLaser) {
				return mLaser->mY();
			}
			return -1;
		}
		double Bear::mdX(const Field&) const {
			return mBody->mdX();
		}
		double Bear::mdY(const Field&) const {
			return mBody->mdY();
		}
		double Bear::getDisplayWidth() const {
			return mWidth * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		double Bear::getDisplayHeight() const {
			return mHeight * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		void Bear::update(const Player& player, Field* field) {

			if (mBody && !deleted) {
				mBody->update(player, field); //Bear::Body::attack = trueなのでupdateは大丈夫
				if (mBody->getAttackFrame()) { //mAttackFrameがtrueでセンサーもレーザーもまだない
					mBody->changeAttackFrame();
					mBody->setDirectionX(Body::LEFT);
					mBody->setDirectionY(Body::NONEY);
					SAFE_DELETE(mSensor);
					mSensor = new Sensor(mBody->mX(), mBody->mY(), *field);
				}
				if (mBody->getHP() <= 0) {
					enemyDelete(field);
				}
			}

			//熊がデリートしたからといって以下が消えるわけではない
			if (mSensor) {
				if (mSensor->isVanished()) {
					SAFE_DELETE(mSensor);
					if (!mLaser) { //mLaserがnewされてない、つまりplayerに当たらず3回センサーを発射したとき
						mBody->AttackEnd();
					}
				}
				else {
					mBody->setImageStatus(0);
					mSensor->update();
				}
			}
			if (mLaser) {
				if (mLaser->isVanished()) {
					SAFE_DELETE(mLaser);
					mBody->setImageStatus(0);
					mBody->AttackEnd();
				}
				else {
					if (mLaser->isLaserOn()) {
						mBody->setImageStatus(1);
					}
					mLaser->update();
				}
			}
		}
		void Bear::drawBody(const Field& field) const {
			if (mBody && !deleted) {
				mBody->draw(field);
			}
		}
		void Bear::drawAttack(Game::GameParent::DrawAttackTiming timing, const Field& field) const {
			if (timing == Game::GameParent::BEFORE) {
				if (mSensor) {
					mSensor->draw(field);
				}
			}
			if (timing == Game::GameParent::BEFORE) {
				if (mLaser) {
					mLaser->draw(field);
				}
			}
		}
		void Bear::attackCollision(Player* player, Field* field) {

			//bodyの有無は関係ない
			if (mSensor && !mLaser) {
				if (mSensor->isSensorOn()) {
					if (player->mY() == mSensor->mY()) {
						SAFE_DELETE(mLaser);
						mLaser = new Laser(mSensor->mX(), mSensor->mY(), *field);
						mSensor->Vanish();
					}
				}
			}
			if (mLaser) {
				if (mLaser->isLaserOn()) {
					if (player->mY() == mLaser->mY() && !laserVanishing) {
						player->decreaseHP(mDamage);
						laserVanishing = true;
					}
					if (laserVanishing) {
						++laserVanishingCount;
						if (laserVanishingCount == 15) {
							mLaser->Vanish();
							laserVanishing = false;
							laserVanishingCount = 0;
						}
					}
				}
			}
		}
		void Bear::damage(int d) {
			mBody->damage(d);
		}
		int Bear::getIndex() const {
			return index;
		}

		void Bear::enemyDelete(Field* field) {
			if (!deleted) {
				field->setCharactor(mBody->mX(), mBody->mY(), Field::Trout::NONE);
				deleted = true;
			}
		}
		bool Bear::isDeleted() const {
			return deleted;
		}
	}
}