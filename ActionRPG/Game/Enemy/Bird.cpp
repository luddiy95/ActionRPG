#include "Game/Enemy/Bird.h"
#include "Game/Player.h"
#include "Game/GameParent.h"
#include "Image.h"
#include "Game/Field.h"
#include "Font.h"
#include <sstream>
#include "GameLib/Framework.h"

using namespace GameLib;

namespace {
	int Round(double a) {
		return (a > 0) ? static_cast<int>(a + 0.5) : static_cast<int>(a - 0.5);
	}
}

namespace Game {
	namespace Enemy {
		Bird::Body::Body(int x, int y, Field* field) : mx(x), my(y), mImageStatus(0), mImageStatusCount(0),
			mMoveCount(0), mAttackCount(0), mBeamCount(0), isAttackFrame(false), HP(40){
			mImage = new Image("data/image/birdBody.dds");
			field->setCharactor(x, y, Field::Trout::ENEMY_BIRD);
		}
		Bird::Body::~Body() {
			SAFE_DELETE(mImage);
		}
		void Bird::Body::update(const Player& player, Field* field) {

			//羽ばたき
			if (++mImageStatusCount == 15) {
				mImageStatusCount = 0;
				mImageStatus = (++mImageStatus % 4);
			}

			//マス移動&攻撃
			int py = player.mY();
			if (my == py) {
				++mAttackCount;
			}
			else { //正面にplayerがいない
				if (mAttackCount == 0 && mMoveCount == 0) {
					//動くにはmAttackCount = 0(攻撃しようとしていない(攻撃終わった直後など))じゃないといけないし
						//カウント開始するのはmMoveCount == 0のときのみ
						//mMoveCountが正になるにはmAttackCountが0でないといけないので
						//mMoveCountが正かつmAttackCount == 0かつ正面にplayerがいないとき
						//mMoveCountが0より大きくなることはない
					mMoveCount = 1; //移動カウント開始
				}
				else if ((0 < mAttackCount && mAttackCount < 30) || (120 < mAttackCount && mAttackCount < 170)) {
					//攻撃しようとしてすぐにplayerが正面から移ったときはmAttackCountが0
					mAttackCount = 0;
				}
				else if ((30 <= mAttackCount && mAttackCount <= 120) || (170 <= mAttackCount && mAttackCount < 260)) {
					//攻撃しようとしてplayerがしばらくその場から動かなかったときはそのまま攻撃
					++mAttackCount;
				}
			}

			//playerが正面にいようがいなかろうが関係ない
			if (mBeamCount > 0) {
				++mBeamCount;
			}
			if ((60 <= mAttackCount && mAttackCount <= 120) || (200 <= mAttackCount && mAttackCount <= 260)) {
				if (mBeamCount == 0) {
					if (mAttackCount == 60 || mAttackCount == 200) { //発射
						mBeamCount = 1; //光線のカウント開始
						isAttackFrame = true;
					}
				}
				else if (0 < mBeamCount && mBeamCount <= 60) { //光線のカウントが始まってる
					mImageStatus = 1;
					if (mBeamCount == 60) {
						mBeamCount = 0;
					}
				}

				if (mAttackCount == 120) {
					if (my != py) {
						mAttackCount = 0;
					}
				}
				else if (mAttackCount == 260) {
					if (my != py) {
						mAttackCount = 0;
					}
					else {
						mAttackCount = 120;
					}
				}
			}

			if (mMoveCount > 0) {
				if (++mMoveCount == 40) {
					mMoveCount = 0;
					if (my < py && my + 1 <= gLayerNumber) {
						if (field->getTroutState(mx, my + 1) & Field::Trout::ENEMY_AREA) {
							if (field->getCharactor(mx, my + 1) == Field::Trout::NONE) {
								field->setCharactor(mx, my, Field::Trout::NONE);
								++my;
								field->setCharactor(mx, my, Field::Trout::ENEMY_BIRD);
							}
						}
					}
					else if (my > py && my - 1 >= 0) {
						if (field->getTroutState(mx, my - 1) & Field::Trout::ENEMY_AREA) {
							if (field->getCharactor(mx, my - 1) == Field::Trout::NONE) {
								field->setCharactor(mx, my, Field::Trout::NONE);
								--my;
								field->setCharactor(mx, my, Field::Trout::ENEMY_BIRD);
							}
							
						}
					}
					else {
						//playerが正面にきたら(mAttackCountが増加する)動かない→いずれ0になる
					}
				}
			}
		}
		void Bird::Body::draw(const Field& field) const {

			Vector2* troutCoordinate = field.getTroutCoordinate(mx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
			double troutWidth = field.getLineX(mx + 1, ty) - field.getLineX(mx, ty);
			double tx = field.getLineX(mx, ty) + troutWidth / 2.0;
			double displayWidth = mWidth * pow(gEnemyScale, static_cast<double>(my));
			double displayHeight = mHeight * pow(gEnemyScale, static_cast<double>(my));
			mImage->draw(field.getOrigin().x + tx - displayWidth / 2.0, field.getOrigin().y + ty - displayHeight,
				displayWidth, displayHeight,
				mImageStatus * mWidth, 1 * mHeight, mWidth, mHeight);

			ostringstream oss;
			oss << HP;
			Font::instance()->drawString(field.getOrigin().x + tx, field.getOrigin().y + ty - displayHeight + 4, 4, 9, oss.str().c_str());
		}
		void Bird::Body::changeAttackFrame() {
			if (isAttackFrame) {
				isAttackFrame = false;
			}
		}
		bool Bird::Body::attackFrame() const {
			return isAttackFrame;
		}
		int Bird::Body::mX() const {
			return mx;
		}
		int Bird::Body::mY() const {
			return my;
		}
		void Bird::Body::damage(int d) {
			HP -= d;
		}
		int Bird::Body::getHP() const {
			return HP;
		}



		Bird::Beam::Beam(const Body& body, const Field& field) : mImage(0), mVanish(false), mdx(0), mdy(0), my(body.mY()) {

			int x = body.mX(), y = body.mY();
			Vector2* troutCoordinate = field.getTroutCoordinate(x, y);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			mdy = troutHeight * gEnemyDepth / 2.0 + troutCoordinate[0].y;
			double troutWidth = field.getLineX(x + 1, mdy) - field.getLineX(x, mdy);
			mdx = field.getLineX(x, mdy);

			mImage = new Image("data/image/birdAttack.dds");
		}
		Bird::Beam::~Beam() {
			SAFE_DELETE(mImage);
		}
		void Bird::Beam::update(const Player& player, const Field& field) {

			if (mdx <= field.getLineX(0, mdy)) { //光線の軌道上の左端のx座標
				mVanish = true;
			}
			else {
				mdx -= mV;
			}
		}
		void Bird::Beam::draw(const Field& field) const {

			mImage->draw(field.getOrigin().x + (mdx - mWidth / 2.0), 
				field.getOrigin().y + Round(mdy - mHeight / 2.0), 0, 0,
				static_cast<int>(mWidth), static_cast<int>(mHeight));
		}
		bool Bird::Beam::isVanished() const {
			return mVanish;
		}
		void Bird::Beam::vanish() {
			mVanish = true;
		}
		int Bird::Beam::mY() const {
			return my;
		}
		double Bird::Beam::mdX() const {
			return mdx;
		}
		double Bird::Beam::mdY() const {
			return mdy;
		}
		void Bird::Beam::getInformation(const Bird::Beam& in) {
			this->mdx = in.mdX();
			this->mdy = in.mdY();
			this->mVanish = in.isVanished();
		}



		Bird::Bird(int x, int y, int n, Field* field) : mBody(0), index(n) {
			mBody = new Body(x, y, field);
		}
		Bird::~Bird() {
			SAFE_DELETE(mBody);
			SAFE_DELETE(mBeam);
		}
		int Bird::bodyX() const {
			return mBody->mX();
		}
		int Bird::bodyY() const {
			return mBody->mY();
		}
		int Bird::attackY() const {
			return mBody->mY();
		}
		double Bird::mdX(const Field& field) const {

			int mx = mBody->mX(), my = mBody->mY();
			Vector2* troutCoordinate = field.getTroutCoordinate(mx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
			double ty = troutHeight * gEnemyDepth + troutCoordinate[0].y;
			double troutWidth = field.getLineX(mx + 1, ty) - field.getLineX(mx, ty);

			return field.getLineX(mx, ty) + troutWidth / 2.0;
		}
		double Bird::mdY(const Field& field) const {

			int mx = mBody->mX(), my = mBody->mY();
			Vector2* troutCoordinate = field.getTroutCoordinate(mx, my);
			double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;

			return troutHeight * gEnemyDepth + troutCoordinate[0].y;
		}
		double Bird::getDisplayWidth() const {
			return mWidth * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		double Bird::getDisplayHeight() const {
			return mHeight * pow(gEnemyScale, static_cast<double>(mBody->mY()));
		}
		void Bird::update(const Player& player, Field* field) {
			if (mBody && !deleted) {
				mBody->update(player, field);
				if (mBody->attackFrame()) {
					if (!mBeam) {
						mBody->changeAttackFrame();
						SAFE_DELETE(mBeam);
						mBeam = new Beam(*mBody, *field);
					}
				}
				if (mBody->getHP() <= 0) {
					enemyDelete(field);
				}
			}
			if (mBeam) {
				mBeam->update(player, *field);
				if (mBeam->isVanished()) {
					SAFE_DELETE(mBeam);
				}
			}
		}
		void Bird::drawBody(const Field& field) const {
			if (mBody && !deleted) {
				mBody->draw(field);
			}
		}
		void Bird::drawAttack(Game::GameParent::DrawAttackTiming timing, const Field& field) const {
			if (timing == mAttackDrawTiming) {
				if (mBeam) {
					mBeam->draw(field);
				}
			}
		}
		void Bird::attackCollision(Player* player, Field*) {
			//衝突判定はx方向はplayerの幅でいいがy方向はマスの高さにする
			if (mBeam) {
				double beamX = mBeam->mdX();
				if (mBeam->mY() == player->mY()) {
					int lb = Round(beamX - mBeamWidth / 2.0);
					int rb = Round(beamX + mBeamWidth / 2.0);
					int lp = Round(player->mdX() - player->getDisplayWidth() / 2.0);
					int rp = Round(player->mdX() + player->getDisplayWidth() / 2.0);
					if ((lb < rp) && (rb > lp)) {
						mBeam->vanish();
						player->decreaseHP(mDamage);
					}
				}
			}
		}

		void Bird::damage(int d) {
			mBody->damage(d);
		}
		int Bird::getIndex() const {
			return index;
		}
		void Bird::enemyDelete(Field* field) {
			if (!deleted) {
				field->setCharactor(mBody->mX(), mBody->mY(), Field::Trout::NONE);
				deleted = true;
			}
		}
		bool Bird::isDeleted() const {
			return deleted;
		}
	}
}