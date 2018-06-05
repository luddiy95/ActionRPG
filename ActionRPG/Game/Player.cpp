#include "Game/Player.h"
#include "Game/GameParent.h"
#include "Game/Field.h"
#include "GameLib/Framework.h"
#include "Pad.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Status.h"
#include "Game/Tarot.h"
#include "Image.h"
#include "Matrix23.h"
#include "Vector2.h"
using namespace GameLib;

namespace {
	int Round(double a) {
		return (a > 0) ? static_cast<int>(a + 0.5) : static_cast<int>(a - 0.5);
	}
}

namespace Game {
	Player::Player(int x, int y, Field* field) : mx(x), my(y), mdx(0), mdy(0), mImage(0),
		mDisplayWidth(0), mDisplayHeight(0), buttonCount(0), HP(100), maxHP(100),
		selectTarotNum(0), headIndex(0), attackTime(false), attackInterval(0),
		mAttack(0){

		Vector2* troutCoordinate = field->getTroutCoordinate(mx, my);
		double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
		double ty = troutHeight * gPlayerDepth + troutCoordinate[0].y;
		mDisplayHeight = gPlayerHeight * pow(gPlayerScale, static_cast<double>(my));
		mdy = ty - mDisplayHeight / 2.0;

		double troutWidth = field->getLineX(mx + 1, ty) - field->getLineX(mx, ty);
		double tx = field->getLineX(mx, ty) + troutWidth / 2.0;
		mDisplayWidth = gPlayerWidth * pow(gPlayerScale, static_cast<double>(my));
		mdx = tx;

		SAFE_DELETE(mImage);
		mImage = new Image("data/image/player1.dds");
		mDirection = RIGHT;
		field->setCharactor(x, y, Field::Trout::PLAYER);

		SAFE_DELETE(mAttack);

		SAFE_DELETE_ARRAY(mDeck);
		//デッキ初期化(いずれ探索シーケンスのplayerに移動)
		mDeck = new Tarot[maxTarotNum];
		
		for (int i = 0; i < 3; i++) {
			mDeck[i].setTarot(Tarot::Tarots::CANON);
		}
		for (int i = 3; i < 10; i++) {
			mDeck[i].setTarot(Tarot::Tarots::LASER);
		}
		for (int i = 10; i < 15; i++) {
			mDeck[i].setTarot(Tarot::Tarots::CANON);
		}
		for (int i = 15; i < 20; i++) {
			mDeck[i].setTarot(Tarot::Tarots::LASER2);
		}
		for (int i = 20; i < 25; i++) {
			mDeck[i].setTarot(Tarot::Tarots::ROB);
		}
		for (int i = 25; i < 30; i++) {
			mDeck[i].setTarot(Tarot::Tarots::RECOVERY);
		}
		for (int i = 0; i < maxTarotNum; i++) {
			int n = Framework::instance().getRandom(100);
			if (0 <= n && n < 8) {
				mDeck[i].setColor(Tarot::BLACK);
			}
			else if (8 <= n && n < 16) {
				mDeck[i].setColor(Tarot::WHITE);
			}
			else if (16 <= n && n < 44) {
				mDeck[i].setColor(Tarot::RED);
			}
			else if (44 <= n && n < 72) {
				mDeck[i].setColor(Tarot::GREEN);
			}
			else if (72 <= n && n < 100) {
				mDeck[i].setColor(Tarot::BLUE);
			}
		}
		
		for (int i = 0; i < maxTarotNum - 1; i++) {
			int n = maxTarotNum - (i + 1);
			n = Framework::instance().getRandom(n) + 1;
			n = n + i;
			mDeck[i].switchTarot(&mDeck[n]);
		}
	}
	Player::~Player() {
		SAFE_DELETE(mImage);
	}
	int Player::mX() const {
		return mx;
	}
	int Player::mY() const {
		return my;
	}
	double Player::mdX() const {
		return mdx;
	}
	double Player::mdY() const {
		return mdy;
	}
	double Player::getDisplayWidth() const {
		return mDisplayWidth;
	}
	double Player::getDisplayHeight() const {
		return mDisplayHeight;
	}
	Player::Direction Player::getDirection() const {
		return mDirection;
	}
	void Player::draw(const Field& field) const {
		mImage->draw(field.getOrigin().x + mdx - mDisplayWidth / 2.0, field.getOrigin().y + mdy - mDisplayHeight / 2.0,
							mDisplayWidth, mDisplayHeight, 
							0, mDirection * gPlayerHeight, gPlayerWidth, gPlayerHeight);
	}
	void Player::update(Field* field) {

		if (!attackTime) { //攻撃している間は動けないしタロットの使用もできない
			++buttonCount;

			//移動
			int dx = 0, dy = 0;
			if (Pad::isOn(Pad::A)) {
				if (Pad::isTriggered(Pad::U)) {
					mDirection = BACK;
				}
				else if (Pad::isTriggered(Pad::DW)) {
					mDirection = FRONT;
				}
				else if (Pad::isTriggered(Pad::L)) {
					mDirection = LEFT;
				}
				else if (Pad::isTriggered(Pad::R)) {
					mDirection = RIGHT;
				}
			}
			else if (Pad::isOn(Pad::B)) {
				//視点移動
				int dx = 0;
				if (Pad::isOn(Pad::L)) {
					dx = -2.2;
				}
				else if (Pad::isOn(Pad::R)) {
					dx += 2.2;
				}
				field->moveOrigin(static_cast<double>(dx));
			}
			else {
				if (buttonCount > 10) {
					if (Pad::isOn(Pad::U)) {
						dy = -1;
						buttonCount = 0;
					}
					else if (Pad::isOn(Pad::DW)) {
						dy = +1;
						buttonCount = 0;
					}
					else if (Pad::isOn(Pad::L)) {
						dx = -1;
						buttonCount = 0;
					}
					else if (Pad::isOn(Pad::R)) {
						dx = +1;
						buttonCount = 0;
					}
				}
			}

			if (mx + dx >= 0 && mx + dx < gLineNumber && my + dy >= 0 && my + dy < gLayerNumber) {
				if (field->getTroutState(mx + dx, my + dy) & Field::Trout::PLAYER_AREA) {
					if (field->getCharactor(mx + dx, my + dy) == Field::Trout::NONE) {
						field->setCharactor(mx, my, Field::Trout::NONE);
						mx += dx;
						my += dy;
						field->setCharactor(mx, my, Field::Trout::PLAYER);
						Vector2* troutCoordinate = field->getTroutCoordinate(mx, my);
						double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
						double ty = troutHeight * gPlayerDepth + troutCoordinate[0].y;
						mDisplayHeight = gPlayerHeight * pow(gPlayerScale, static_cast<double>(my));
						mdy = ty - mDisplayHeight / 2.0;

						double troutWidth = field->getLineX(mx + 1, ty) - field->getLineX(mx, ty);
						double tx = field->getLineX(mx, ty) + troutWidth / 2.0;
						mDisplayWidth = gPlayerWidth * pow(gPlayerScale, static_cast<double>(my));
						mdx = tx;
					}
				}
			}

			if (attackInterval > 0) {
				++attackInterval;
				if (attackInterval == 40) {
					attackInterval = 0;
				}
			}
			//タロット使用
			if (Pad::isTriggered(Pad::D) && attackInterval == 0) {
				if (headIndex >= 0 && headIndex < maxTarotNum && selectTarotNum > 0) {
					attackTime = true;
					if (!mAttack) {
						SAFE_DELETE(mAttack);
						switch (mDeck[headIndex].getTarot()) {
						case Tarot::LASER: mAttack = new attackLaser(*this); break;
						case Tarot::LASER2: mAttack = new attackLaser2(*this); break;
						case Tarot::CANON: mAttack = new Canon(*this, *field); break;
						case Tarot::RECOVERY: mAttack = new Recovery(*this); break;
						case Tarot::ROB: mAttack = new Rob(*this, *field); break;
						}
					}
					++headIndex;
					--selectTarotNum;
				}
			}
		}
		
		if (headIndex >= 30) {
			Framework::instance().drawDebugString(2, 3, "ﾊﾞﾂﾃﾞﾘﾀｲｱ");
			if (Pad::isTriggered(Pad::C)) {
				HP = 0;
			}
		}
	}
	
	int Player::hp() const {
		return HP;
	}
	void Player::decreaseHP(int damage) {
		HP -= damage;
	}
	void Player::increaseHP(int up) {
		HP += up;
		if (HP >= maxHP) {
			HP = maxHP;
		}
	}
	Tarot* Player::deck() const {
		return mDeck;
	}
	void Player::setSelectTarotNum(int n) {
		selectTarotNum = n;
	}
	int Player::getSelectTarotNum() const {
		return selectTarotNum;
	}
	int Player::getHeadIndex() const {
		return headIndex;
	}
	void Player::setHeadIndex(int n) {
		headIndex = n;
	}

	bool Player::getAttackTime() const {
		return attackTime;
	}
	Player::playerAttack* Player::attack() const {
		return mAttack;
	}
	void Player::changeAttackTime() {
		if (attackTime) {
			attackTime = false;
			attackInterval = 1;
		}
	}
	void Player::endAttack() {
		if (mAttack) {
			SAFE_DELETE(mAttack);
		}
	}


	Player::attackLaser::attackLaser(const Player& player) : mImage(0), firstAttackInterval(0),
		playerX(player.mX()), playerY(player.mY()),
		mLaserCount(0), laserCollisionCount(0), laserCollision(false), collisionEnemyNum(0){
		collisionIndex = new int[gLayerNumber * gLineNumber];
		for (int i = 0; i < gLayerNumber * gLineNumber; i++) {
			collisionIndex[i] = -1;
		}
		SAFE_DELETE(mImage);
		mImage = new Image("data/image/playerLaser.dds");
	}
	Player::attackLaser::~attackLaser() {
		SAFE_DELETE(mImage);
	}
	void Player::attackLaser::attackUpdate(Player* player, Field*) {

		if (firstAttackInterval < firstAttackIntervalMax) {
			++firstAttackInterval;
			return;
		}

		++mLaserCount;

		//攻撃終了
		if (mLaserCount == 45 || (laserCollision && (mLaserCount == laserCollisionCount + 15))) {
			mLaserCount = 0;
			laserCollisionCount = 0;
			laserCollision = false;
			firstAttackInterval = 0;
			player->changeAttackTime();
		}
	}
	void Player::attackLaser::attackCollision(Enemy::Enemy* enemy, Player* player, const Field&) {
		//enemyとの衝突判定
		if (firstAttackInterval < firstAttackIntervalMax) {

			return;
		}
		if (player->mX() == enemy->bodyX()) {
			//最初の一体に当たったらlaserは消えていき、最後の方は当たっても攻撃にならない
			bool yet = true;
			for (int i = 0; i < collisionEnemyNum; i++) {
				if (collisionIndex[i] == enemy->getIndex()) {
					yet = false;
				}
			}
			if (yet) {
				if (!laserCollision) {
					enemy->damage(laserDamage);
					collisionIndex[collisionEnemyNum] = enemy->getIndex();
					++collisionEnemyNum;
					laserCollisionCount = mLaserCount;
					laserCollision = true;
				}
				else if (laserCollision && mLaserCount <= laserCollision + 10) {
					enemy->damage(laserDamage);
					collisionIndex[collisionEnemyNum] = enemy->getIndex();
					++collisionEnemyNum;
				}
			}
		}
	}
	void Player::attackLaser::drawAttack(GameParent::DrawAttackTiming timing, const Field& field) const {

		if (firstAttackInterval < firstAttackIntervalMax) {
			return;
		}

		if (timing == mDrawAttackTiming) {
			double bottom = field.getTroutCoordinate(0, gLayerNumber - 1)[2].y;
			double top1 = field.getLineX(playerX, -8.0);
			double bottom1 = field.getLineX(playerX, bottom + 8.0);
			double top2 = field.getLineX(playerX + 1, -8.0);
			double bottom2 = field.getLineX(playerX + 1, bottom + 8.0);
			double topWidth = top2 - top1;
			double bottomWidth = bottom2 - bottom1;
			double leftTop = top1 + topWidth * 1. / 4.;
			double rightTop = top2 - topWidth * 1. / 8.;
			double leftBottom = bottom1 + bottomWidth * 1. / 4.;
			double rightBottom = bottom2 - bottomWidth * 1. / 8.;

			Vector2* mCoordinate = new Vector2[4];
			mCoordinate[0].setVector(leftTop, -8.0);
			mCoordinate[1].setVector(rightTop, -8.0);
			mCoordinate[2].setVector(leftBottom, bottom + 8.0);
			mCoordinate[3].setVector(rightBottom, bottom + 8.0);

			Matrix23 translate;
			translate.setTranslation(Vector2(field.getOrigin().x, field.getOrigin().y));
			Vector2 C0, C1, C2, C3;
			translate.multiply(&C0, mCoordinate[0]);
			translate.multiply(&C1, mCoordinate[1]);
			translate.multiply(&C2, mCoordinate[2]);
			translate.multiply(&C3, mCoordinate[3]);

			mImage->draw(C0, C1, C2, C3, 0, 0, mWidth, mHeight);
		}
	}

	Player::attackLaser2::attackLaser2(const Player& player) : mImage1(0), mImage2(0),
		playerX(player.mX()), playerY(player.mY()), firstAttackInterval(0),
		mLaser2Count(0), laser2CollisionCount(0), laser2Collision(false), collisionEnemyNum2(0) {
		collisionIndex2 = new int[gLayerNumber * gLineNumber];
		for (int i = 0; i < gLayerNumber * gLineNumber; i++) {
			collisionIndex2[i] = -1;
		}
		SAFE_DELETE(mImage1);
		SAFE_DELETE(mImage2);
		mImage1 = new Image("data/image/playerLaser.dds");
		mImage2 = new Image("data/image/playerLaserVertical.dds");
	}
	Player::attackLaser2::~attackLaser2() {
		SAFE_DELETE(mImage1);
		SAFE_DELETE(mImage2);
	}
	void Player::attackLaser2::attackUpdate(Player* player, Field*) {

		if (firstAttackInterval < firstAttackIntervalMax) {
			++firstAttackInterval;
			return;
		}

		++mLaser2Count;

		//攻撃終了
		if (mLaser2Count == 45 || (laser2Collision && (mLaser2Count == laser2CollisionCount + 15))) {
			mLaser2Count = 0;
			laser2CollisionCount = 0;
			laser2Collision = false;
			player->changeAttackTime();
		}
	}
	void Player::attackLaser2::attackCollision(Enemy::Enemy* enemy, Player* player, const Field&) {
		//enemyとの衝突判定
		if (firstAttackInterval < firstAttackIntervalMax) {
			return;
		}
		if (player->mX() == enemy->bodyX() || player->mY() == enemy->bodyY()) {
			//最初の一体に当たったらlaserは消えていき、最後の方は当たっても攻撃にならない
			bool yet = true;
			for (int i = 0; i < collisionEnemyNum2; i++) {
				if (collisionIndex2[i] == enemy->getIndex()) {
					yet = false;
				}
			}
			if (yet) {
				if (!laser2Collision) {
					enemy->damage(laser2Damage);
					collisionIndex2[collisionEnemyNum2] = enemy->getIndex();
					++collisionEnemyNum2;
					laser2CollisionCount = mLaser2Count;
					laser2Collision = true;
				}
				else if (laser2Collision && mLaser2Count <= laser2Collision + 10) {
					enemy->damage(laser2Damage);
					collisionIndex2[collisionEnemyNum2] = enemy->getIndex();
					++collisionEnemyNum2;
				}
			}
		}
	}
	void Player::attackLaser2::drawAttack(GameParent::DrawAttackTiming timing, const Field& field) const {

		if (firstAttackInterval < firstAttackIntervalMax) {
			return;
		}

		if (timing == mDrawAttackTiming) {
			double bottom = field.getTroutCoordinate(0, gLayerNumber - 1)[2].y;
			double top1 = field.getLineX(playerX, -8.0);
			double bottom1 = field.getLineX(playerX, bottom + 8.0);
			double top2 = field.getLineX(playerX + 1, -8.0);
			double bottom2 = field.getLineX(playerX + 1, bottom + 8.0);
			double topWidth = top2 - top1;
			double bottomWidth = bottom2 - bottom1;
			double leftTop = top1 + topWidth * 1. / 4.;
			double rightTop = top2 - topWidth * 1. / 8.;
			double leftBottom = bottom1 + bottomWidth * 1. / 4.;
			double rightBottom = bottom2 - bottomWidth * 1. / 8.;

			Vector2* mCoordinate = new Vector2[4];
			mCoordinate[0].setVector(leftTop, -8.0);
			mCoordinate[1].setVector(rightTop, -8.0);
			mCoordinate[2].setVector(leftBottom, bottom + 8.0);
			mCoordinate[3].setVector(rightBottom, bottom + 8.0);

			Matrix23 translate;
			translate.setTranslation(Vector2(field.getOrigin().x, field.getOrigin().y));
			Vector2 C0, C1, C2, C3;
			translate.multiply(&C0, mCoordinate[0]);
			translate.multiply(&C1, mCoordinate[1]);
			translate.multiply(&C2, mCoordinate[2]);
			translate.multiply(&C3, mCoordinate[3]);

			mImage1->draw(C0, C1, C2, C3, 0, 0, mWidth, mHeight);

			double right = field.getTroutCoordinate(gLineNumber - 1, playerY)[3].x;
			double left1 = field.getTroutCoordinate(0, playerY)[0].y;
			double right1 = field.getTroutCoordinate(gLineNumber - 1, playerY)[1].y;
			double left2 = field.getTroutCoordinate(0, playerY)[2].y;
			double right2 = field.getTroutCoordinate(gLineNumber - 1, playerY)[3].y;
			double leftHeight = left2 - left1;
			double rightHeight = right2 - right1;
			double topleft = left1 + leftHeight * 1. / 4.;
			double bottomleft = left2 - leftHeight * 1. / 8.;
			double topright = right1 + rightHeight * 1. / 4.;
			double bottomright = right2 - rightHeight * 1. / 8.;

			mCoordinate[0].setVector(-60.0, topleft);
			mCoordinate[1].setVector(right + 8.0, topright);
			mCoordinate[2].setVector(-60.0, bottomleft);
			mCoordinate[3].setVector(right + 8.0, bottomright);

			translate.multiply(&C0, mCoordinate[0]);
			translate.multiply(&C1, mCoordinate[1]);
			translate.multiply(&C2, mCoordinate[2]);
			translate.multiply(&C3, mCoordinate[3]);

			mImage2->draw(C0, C1, C2, C3, 0, 0, mWidth, mHeight);
		}
	}


	Player::Canon::Canon(const Player& player, const Field& field) : mImage(0), mVanish(false), mdx(0), mdy(0),
		playerX(player.mX()), playerY(player.mY()), xmV(0),
		firstAttackInterval(0), playerDirection(player.getDirection()) {

		int x = player.mX(), y = player.mY();
		Vector2* troutCoordinate = field.getTroutCoordinate(x, y);
		double troutHeight = troutCoordinate[2].y - troutCoordinate[0].y;
		mdy = troutHeight * gEnemyDepth / 2.0 + troutCoordinate[0].y;
		double troutWidth = field.getLineX(x + 1, mdy) - field.getLineX(x, mdy);
		mdx = field.getLineX(x, mdy) + troutWidth / 2.0;

		double troutWidth1 = 0, goalx = 0;

		switch (playerDirection) {
		case Player::Direction::FRONT:
			troutWidth1 = field.getTroutCoordinate(x + 1, gLayerNumber - 1)[2].x
				- field.getTroutCoordinate(x, gLayerNumber - 1)[2].x;
			goalx = field.getTroutCoordinate(x, gLayerNumber - 1)[2].x +
				troutWidth1 / 2.0;
			xmV = (goalx - mdx) / (field.getTroutCoordinate(x, gLayerNumber - 1)[2].y - mdy) * mV;
			break;
		case Player::Direction::BACK:
			troutWidth1 = field.getTroutCoordinate(x + 1, 0)[0].x
				- field.getTroutCoordinate(x, 0)[0].x;
			goalx = field.getTroutCoordinate(x, 0)[0].x +
				troutWidth1 / 2.0;
			xmV = (goalx - mdx) / (mdy - field.getTroutCoordinate(x, 0)[0].y) * mV;
			break;
		}

		SAFE_DELETE(mImage);
		mImage = new Image("data/image/playerCanon.dds");
	}
	Player::Canon::~Canon() {
		SAFE_DELETE(mImage);
	}
	void Player::Canon::attackUpdate(Player* player, Field* field) {

		if (firstAttackInterval < firstAttackIntervalMax) {
			++firstAttackInterval;
			return;
		}

		if (!mVanish) {

			switch (playerDirection) {
			case Player::Direction::RIGHT:
				if (mdx >= field->getLineX(gLineNumber, mdy)) {
					mVanish = true;
					player->changeAttackTime();
				}
				else {
					mdx += mV;
				}
				break;
			case Player::Direction::LEFT:
				if (mdx <= field->getLineX(0, mdy)) {
					mVanish = true;
					player->changeAttackTime();
				}
				else {
					mdx -= mV;
				}
				break;
			case Player::Direction::FRONT:
				if (mdy >= field->getTroutCoordinate(playerX, gLayerNumber - 1)[2].y) {
					mVanish = true;
					player->changeAttackTime();
				}
				else {
					mdy += mV;
					mdx += xmV;
				}
				break;
			case Player::Direction::BACK:
				if (mdy <= field->getTroutCoordinate(playerX, 0)[0].y) {
					mVanish = true;
					player->changeAttackTime();
				}
				else {
					mdy -= mV;
					mdx += xmV;
				}
				break;
			}
			
		}
	}
	void Player::Canon::attackCollision(Enemy::Enemy* enemy, Player* player, const Field& field) {
		switch (playerDirection) {
		case Player::Direction::RIGHT:
			if (playerY == enemy->bodyY() && playerX < enemy->bodyX()) {
				int lp = Round(mdx - mWidth / 2.0);
				int rp = Round(mdx + mWidth / 2.0);
				int le = Round(enemy->mdX(field) - enemy->getDisplayWidth() / 4.0);
				int re = Round(enemy->mdX(field) + enemy->getDisplayWidth() / 2.0);
				if ((lp < re) && (rp > le)) {
					mVanish = true; //一体にでも当たったら消える
					player->changeAttackTime();
					enemy->damage(mCanonDamage);
				}
			}
			break;
		case Player::Direction::LEFT:
			if (playerY == enemy->bodyY() && playerX > enemy->bodyX()) {
				int lp = Round(mdx - mWidth / 2.0);
				int rp = Round(mdx + mWidth / 2.0);
				int lb = Round(enemy->mdX(field) - enemy->getDisplayWidth() / 2.0);
				int rb = Round(enemy->mdX(field) + enemy->getDisplayWidth() / 4.0);
				if ((lp < rb) && (rp > lb)) {
					mVanish = true; //一体にでも当たったら消える
					player->changeAttackTime();
					enemy->damage(mCanonDamage);
				}
			}
			break;
		case Player::Direction::FRONT:
			if (playerX == enemy->bodyX() && playerY < enemy->bodyY()) {
				//enemyの真ん中の座標とcanonの真ん中の座標の関係でplayerY < enemy->body()
				int tp = Round(mdy - mWidth / 2.0);
				int bp = Round(mdy + mWidth / 2.0);
				int tb = Round(enemy->mdY(field) - enemy->getDisplayHeight() / 4.0);
				int bb = Round(enemy->mdY(field) + enemy->getDisplayHeight() / 2.0);
				if ((tp < bb) && (bp > tb)) {
					mVanish = true; //一体にでも当たったら消える
					player->changeAttackTime();
					enemy->damage(mCanonDamage);
				}
			}
			break;
		case Player::Direction::BACK:
			if (playerX == enemy->bodyX() && playerY > enemy->bodyY()) { //playerの方がenemyより下
				int tp = Round(mdy - mWidth / 2.0);
				int bp = Round(mdy + mWidth / 2.0);
				int tb = Round(enemy->mdY(field) - enemy->getDisplayHeight() / 2.0);
				int bb = Round(enemy->mdY(field) + enemy->getDisplayHeight() / 4.0);
				if ((tp < bb) && (bp > tb)) {
					mVanish = true; //一体にでも当たったら消える
					player->changeAttackTime();
					enemy->damage(mCanonDamage);
				}
			}
			break;
		}
	}
	void Player::Canon::drawAttack(GameParent::DrawAttackTiming timing, const Field& field) const {

		if (firstAttackInterval < firstAttackIntervalMax) {
			return;
		}

		if (timing == mDrawAttackTiming && !mVanish) {
			mImage->draw(field.getOrigin().x + (mdx - mWidth / 2.0),
				field.getOrigin().y + Round(mdy - mHeight / 2.0), 0, 0,
				static_cast<int>(mWidth), static_cast<int>(mHeight));
		}
	}


	Player::Recovery::Recovery(const Player& player) {
		
	}
	Player::Recovery::~Recovery() {

	}
	void Player::Recovery::attackUpdate(Player* player, Field*) {
		player->increaseHP(30);
		player->changeAttackTime();
	}
	void Player::Recovery::drawAttack(GameParent::DrawAttackTiming timing, const Field&) const {

	}
	void Player::Recovery::attackCollision(Enemy::Enemy* enemy, Player* player, const Field&) {

	}


	Player::Rob::Rob(const Player& player, const Field& field) : playerX(player.mX()), playerY(player.mY()),
		playerDirection(player.getDirection()), robX(-1), robY(-1), firstAttackInterval(0), mAttackTroutCount(0),
		isChangeArea(false){
		switch (playerDirection) {
		case Player::Direction::LEFT:
			if (playerX > 0) {
				if (field.getTroutState(playerX - 1, playerY) == Field::Trout::ENEMY_AREA) {
					robX = playerX - 1;
					robY = playerY;
				}
			}
			break;
		case Player::Direction::RIGHT:
			if (playerX < gLineNumber - 1) {
				if (field.getTroutState(playerX + 1, playerY) == Field::Trout::ENEMY_AREA) {
					robX = playerX + 1;
					robY = playerY;
				}
			}
			break;
		case Player::Direction::FRONT:
			if (playerY < gLayerNumber - 1) {
				if (field.getTroutState(playerX, playerY + 1) == Field::Trout::ENEMY_AREA) {
					robX = playerX;
					robY = playerY + 1;
				}
			}
			break;
		case Player::Direction::BACK:
			if (playerY > 0) {
				if (field.getTroutState(playerX, playerY - 1) == Field::Trout::ENEMY_AREA) {
					robX = playerX;
					robY = playerY - 1;
				}
			}
			break;
		}

		if (robX != -1 && robY != -1) {
			m0 = field.getTroutCoordinate(robX, robY)[0];
			m1 = field.getTroutCoordinate(robX, robY)[1];
			m2 = field.getTroutCoordinate(robX, robY)[2];
			m3 = field.getTroutCoordinate(robX, robY)[3];
			mTranslation.setVector(0.0, -mFirstDistance);
		}

		SAFE_DELETE(mImage);
		mImage = new Image("data/image/attackTrout.dds");
	}
	Player::Rob::~Rob() {
		SAFE_DELETE(mImage);
	}
	void Player::Rob::attackUpdate(Player* player, Field* field) {
		if (robX == -1 || robY == -1) {
			player->changeAttackTime();
			return;
		}
		if (firstAttackInterval < firstAttackIntervalMax) {
			++firstAttackInterval;
			return;
		}


		if (mAttackTroutCount < robAreaFrame) {
			++mAttackTroutCount;
			mTranslation.setVector(0.0,
				-1.0 * (mFirstDistance - mFirstDistance * static_cast<double>(mAttackTroutCount) / static_cast<double>(robAreaFrame)));
		}
		else if (mAttackTroutCount == robAreaFrame) {
			if (!isChangeArea) {
				field->playerChangeField(robX, robY);
			}
			mAttackTroutCount = 0;
			player->changeAttackTime();
		}
	}
	void Player::Rob::attackCollision(Enemy::Enemy* enemy, Player* player, const Field&) {

		if (mAttackTroutCount == robAreaFrame) {
			if (robX == enemy->bodyX() && robY == enemy->bodyY()) {
				enemy->damage(robDamage);
				isChangeArea = true; //enemy一体にでも当たればarea変えますよ
			}
		}
	}
	void Player::Rob::drawAttack(GameParent::DrawAttackTiming timing, const Field& field) const {
		if (robX == -1 || robY == -1) {
			return;
		}

		if (timing == mDrawAttackTiming) {

			Matrix23 translate;
			translate.setTranslation(Vector2(mTranslation.x + field.getOrigin().x, mTranslation.y + field.getOrigin().y));
			Vector2 c0, c1, c2, c3;
			translate.multiply(&c0, m0);
			translate.multiply(&c1, m1);
			translate.multiply(&c2, m2);
			translate.multiply(&c3, m3);
			mImage->draw(c0, c1, c2, c3, 0 * gFieldWidth, 0 * gFieldHeight, gFieldWidth, gFieldHeight);
		}
	}
}