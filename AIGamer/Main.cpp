//Main.cpp

#include <Siv3D.hpp>
#include "Enemy.h"
#include "BulletPool.h"
#include "player.h"
#include "config.h"
#include "speed.h"
#include "sound.h"
#include "ScoreItemPool.h"
#include "Boss.h"
#include "GameData.h"
//================================================================================
//進行管理
// ===============================================================================




//ゲームの進行状況
enum class GameState {
	SpawingWave1,
	FightingWave1,
	SpawingWave2,
	FightingWave2,
	SpawingBoss,
	FightingBoss,
	Clear,
};

//シーン管理
enum class GameScene {
	Explain,
	Title,
	Game,
	Result,
};

//プレイ中/エンディング中
enum class GamePhase {
	Playing,
	BossExploding,
	Enging,
};

//================================================================================
//ヘルパー関数
//================================================================================


//エネミーを出現させるヘルパー関数
void SpawnWave1(Array<Enemy>& enemies) {
	const Enemy::EnemyParam GruntParam{ .recognitionRange = 1000, .attackRange = 300,.size = 60,.fireRate = 0.1,.color = Palette::Red,.hp = 1 };
	const Enemy::EnemyParam SniperParam{ .recognitionRange = 1000, .attackRange = 500,.size = 30, .fireRate = 0.5,.color = Palette::Green,.hp = 2 };
	for (size_t i = 0; i < Config::wave1EnemyNum * GameData::gameLevel; i++) {
		if (i < Config::wave1SpawnThreshold * GameData::gameLevel)	enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), GruntParam);
		if (i >= Config::wave1SpawnThreshold * GameData::gameLevel) enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), SniperParam);
	}

}

void SpawnWave2(Array<Enemy>& enemies) {
	const Enemy::EnemyParam GruntParam{ .recognitionRange = 1000, .attackRange = 300,.size = 60,.fireRate = 0.1,.color = Palette::Red,.hp = 1 };
	const Enemy::EnemyParam SniperParam{ .recognitionRange = 1000, .attackRange = 500,.size = 30, .fireRate = 0.5,.color = Palette::Green,.hp = 2 };
	for (size_t i = 0; i < Config::wave2EnemyNum * GameData::gameLevel; i++) {
		if (i < Config::wave2SpawnThreshold * GameData::gameLevel)	enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), GruntParam);
		if (i >= Config::wave2SpawnThreshold * GameData::gameLevel) enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), SniperParam);
	}
}

void SpawnWave3(Array<Enemy>& enemies) {
	const Enemy::EnemyParam GruntParam{ .recognitionRange = 1000, .attackRange = 300,.size = 60,.fireRate = 0.1,.color = Palette::Red,.hp = 1 };
	const Enemy::EnemyParam SniperParam{ .recognitionRange = 1000, .attackRange = 500,.size = 30, .fireRate = 0.5,.color = Palette::Green,.hp = 2 };
	for (size_t i = 0; i < Config::wave3EnemyNum * GameData::gameLevel; i++) {
		if (i < Config::wave3SpawnThreshold * GameData::gameLevel)	enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), GruntParam);
		if (i >= Config::wave3SpawnThreshold * GameData::gameLevel) enemies.emplace_back(RandomVec2(Config::movableArea.stretched(-100)), SniperParam);
	}
}

// GameStateの値をStringに変換する関数
String ToString(const GameState state)
{
	switch (state)
	{
	case GameState::SpawingWave1:
		return U"Spawn1";
	case GameState::FightingWave1:
		return U"Fight1";
	case GameState::SpawingWave2:
		return U"Spawn2";
	case GameState::FightingWave2:
		return U"Fight2";
	case GameState::SpawingBoss:
		return U"SpawnBoss";
	case GameState::FightingBoss:
		return U"FightBoss";
	case GameState::Clear:
		return U"Clear";

	default:
		return U"Unknown";
	}
}

//数秒に1度自動でtrueを返すヘルパー関数
bool fewSecondTrue(double intervalSeconds) {
	static double lastTime = 0.0;

	double currentTime = Scene::Time();

	if (currentTime - lastTime >= intervalSeconds) {
		lastTime = currentTime;
		return true;
	}

	return false;
}

//スペースを押した時、trueを返した後指定秒のクールダウンを作るヘルパー関数
class CooldownShooter {

public:
	CooldownShooter(double intervalSec)
		: m_intervalSec(intervalSec)
		, m_timer(StartImmediately::Yes){ }

	bool tryShoot() {
		if ((KeySpace.pressed() || MouseL.pressed())&& m_timer.sF() >= m_intervalSec) {
			m_timer.restart();
			return true;
		}

		return false;
	}
private:
	double m_intervalSec;
	Stopwatch m_timer;

};



// 変化する関数
std::function<Optional<double>()> CreateValueLerp(double from, double to, double duration)
{
	const double startTime = Scene::Time();
	const double halfDuration = duration / 2.0;

	return [=]() mutable -> Optional<double>
		{
			double t = Scene::Time() - startTime;

			if (t > duration) {
				return none; // 時間経過で終了
			}

			if (t < halfDuration) {
				// A → C に変化
				double rate = t / halfDuration;
				return Math::Lerp(from, to, rate);
			}
			else {
				// C → A に戻る
				double rate = (t - halfDuration) / halfDuration;
				return Math::Lerp(to, from, rate);
			}
		};
}

//初期化関数
void Init(
	Player& player,
	Array<Enemy>& enemies,
	BulletPool& enemyBulletPool,
	BulletPool& playerBulletPool,
	ScoreItemPool& scoreItemPool,
	Camera2D& camera,
	CooldownShooter& shooter,
	Speed& sceneSpeed,
	GamePhase& nowPhase,
	Stopwatch& inputDelayStopWatch,
	GameState& currentState,
	int& score
) {
	player.reset({0,0});
	enemies.clear();
	enemyBulletPool.reset();
	playerBulletPool.reset();
	scoreItemPool.reset();
	camera = Camera2D(player.getShape().center, 1.0);
	shooter = CooldownShooter(0.5);
	sceneSpeed = Speed();
	nowPhase = GamePhase::Playing;
	inputDelayStopWatch.reset();
	currentState = GameState::SpawingWave1;
	score = 0;
}


void Main()
{
	

	//===================ゲームオブジェクト====================
	Player player{ {0,0},15 };		//プレイヤーキャラクター
	Array<Enemy>enemies;			//エネミーArray
	Optional<Boss> boss;
	BulletPool enemyBulletPool{ 200 };//敵弾
	BulletPool playerBulletPool{ 50 };//自弾
	ScoreItemPool scoreItemPool{ 100 }; //スコアアイテム
	//===================カメラとシューター====================
	Camera2D camera{ player.getShape().center, 1.0 };// 2Dカメラを作成
	CooldownShooter shooter(0.5);
	Speed sceneSpeed;
	int score = 0;

	//===================アセット==============================
	InitSounds();
	titleBGM->playOneShot();

	const Texture titleBG(U"assets/Images/TitleBG.png");
	const Texture explainBG(U"assets/Images/ExplainBG.png");

	//===================状態遷移==============================
	GameScene nowScene = GameScene::Explain;		
	GamePhase nowPhase = GamePhase::Playing;	
	Stopwatch inputDelayStopWatch;	//シーン遷移用キー入力のインターバル
	GameState currentState = GameState::SpawingWave1;
	inputDelayStopWatch.start();//シーン遷移を正しく処理するためのタイマー
	//===================UI====================================
	bool showText = true;
	//==================画面設定===============================
	Window::Resize(1280, 720);
	Window::SetTitle(U"スフィアシューター");
	Scene::Resize(1920, 1080);
	Scene::SetLetterbox(Palette::Black);
	Scene::SetBackground(Palette::Black);

	//==================ゲームを管理するスピーダー=============
	Optional<std::function<Optional<double>()>> effect;
	double sceneSpeeder = 1.0;


	//==================処理開始===============================
	while (System::Update())
	{

		if (KeyC.down()) {
			effect = CreateValueLerp(1.0, 0.5,1.0);
		}

		// 変化処理がある場合は更新
		if (effect) {
			if (auto val = (*effect)()) {
				sceneSpeeder = *val;
				sceneSpeed.setScale(sceneSpeeder);
			}
			else {
				effect = none; // 終了したら消す
				sceneSpeed.setScale(sceneSpeeder);
			}
		}
		

		switch (nowScene)
		{
			//=============================nowScene::Explain============================
		case GameScene::Explain:
			explainBG.resized(Scene::Size()).draw(0, 0);

			if (fewSecondTrue(0.5)) {
				if (showText == true) showText = false;
				else if (showText == false) showText = true;
			}

			if (showText == true) {
				SimpleGUI::GetFont()(U"PUSH SPACE BUTTON").drawAt({Scene::Center().x,Scene::Center().y + 150}, Palette::Black);
			}
			if (KeySpace.pressed() && inputDelayStopWatch > 0.5s) {
				Init(player, enemies, enemyBulletPool, playerBulletPool, scoreItemPool, camera, shooter, sceneSpeed, nowPhase, inputDelayStopWatch, currentState, score);
				nowScene = GameScene::Title;
				inputDelayStopWatch.restart();
			}

			break;
			//=============================nowScene::Title==============================
		case GameScene::Title:
			titleBG.resized(Scene::Size()).draw(0, 0);

			if (fewSecondTrue(0.5)) {
				if (showText == true) showText = false;
				else if (showText == false) showText = true;
			}

			if (showText == true) {
				SimpleGUI::GetFont()(U"PUSH SPACE BUTTON").drawAt(Scene::Center(), Palette::White);
			}
			if (KeySpace.pressed() && inputDelayStopWatch > 0.5s) {
				Init(player, enemies, enemyBulletPool, playerBulletPool,scoreItemPool, camera, shooter, sceneSpeed, nowPhase, inputDelayStopWatch,currentState,score);
				nowScene = GameScene::Game;
			}
			break;
			//============================nowScene::Game===============================
		case GameScene::Game:
		{
			// --- 描画処理 ---

			//ホワイトアウト用変数
			static double whiteoutAlpha = 0.0;
			static Stopwatch whiteoutTimer;



			{
				// このブロックの中だけ、描画座標系がワールド座標になる
				const auto t = camera.createTransformer();

				for (int32 y = -2; y <= 2; ++y)
				{
					for (int32 x = -2; x <= 2; ++x)
					{
						RectF{ Arg::center(x * 400, y * 400), 380 }.draw(ColorF{ 0.4, 0.5, 0.6 });
					}
				}


				for (auto& enemy : enemies)
				{
					enemy.draw();
				}

				if (boss) {
					boss->draw();
				}

				player.draw();

				//drawAll()
				enemyBulletPool.drawAll();
				playerBulletPool.drawAll();
				scoreItemPool.drawAll();



				String now = ToString(currentState);
				SimpleGUI::GetFont()(Format(now)).drawAt({ player.getShape().center.x, player.getShape().center.y + 20 }, Palette::Black);

			}

			//-------UI表示------------
			{
				const Vec2 hpBarPos = { 20,20 };
				const Vec2 hpBarSize = { 300,30 };
				const double barRounding = 5.0;
				const double borderThickness = 2.0;

				const double healthRatio = static_cast<double>(player.getHp()) / player.getMaxHp();


				RoundRect{ hpBarPos,hpBarSize,barRounding }.draw(Palette::Black);

				if (player.getHp() > 0)
				{
					// Arg::topLeft() を削除した正しいコンストラクタ呼び出し
					RoundRect{ hpBarPos + Vec2{ borderThickness, borderThickness },
							   Vec2{ (hpBarSize.x - borderThickness * 2) * healthRatio, hpBarSize.y - borderThickness * 2 },
							   barRounding }
					.draw(HSV{ 120, 0.7, 0.9 }); // 緑色をHSVで指定
				}

				const String hpText = U"HP: {}/{}"_fmt(player.getHp(), player.getMaxHp());
				SimpleGUI::GetFont()(hpText).draw(Arg::leftCenter(hpBarPos.x + 10, hpBarPos.y / 2), Palette::White);


				const String scoreText = U"SCORE: {}"_fmt(score);

				SimpleGUI::GetFont()(scoreText).draw(Arg::topRight(Scene::Width() - 20, 20), Palette::White);

			}

			if (whiteoutAlpha > 0.0) {
				Scene::Rect().draw(ColorF{ 1.0,whiteoutAlpha });
			}



			//===================================nowPhase::Playing====================
			switch (nowPhase)
			{


			case GamePhase::Playing:
				if (shooter.tryShoot()) {
					player.shoot(camera, playerBulletPool);

				}



				player.update(sceneSpeed);


				//スポーンウェーブ管理
				switch (currentState)
				{
				case GameState::SpawingWave1:
					SpawnWave1(enemies);
					currentState = GameState::FightingWave1;
					break;

				case GameState::FightingWave1:
					if (enemies.isEmpty()) {
						currentState = GameState::SpawingWave2;
					}
					break;

				case GameState::SpawingWave2:
					SpawnWave2(enemies);
					currentState = GameState::FightingWave2;
					break;
				case GameState::FightingWave2:
					if (enemies.isEmpty()) {
						currentState = GameState::SpawingBoss;
					}
					break;
				case GameState::SpawingBoss:
					//ボス召喚
					boss.emplace(Vec2{ 0,-500 }, enemyBulletPool);
					SpawnWave3(enemies);
					currentState = GameState::FightingBoss;

					break;

				case GameState::FightingBoss:

					if (enemies.isEmpty() && !boss) {
						currentState = GameState::Clear;
						nowPhase = GamePhase::BossExploding;
					}

					break;


				}

				for (auto& enemy : enemies) {
					enemy.update(player.getShape().center, enemyBulletPool, sceneSpeed);
				}

				// カメラをプレイヤーの中心に滑らかに追従させる
				camera.setTargetCenter(player.getShape().center);
				camera.update();


				//updateAll()
				enemyBulletPool.updateAll(sceneSpeed);
				playerBulletPool.updateAll(sceneSpeed);
				scoreItemPool.updateAll();

				//敵弾 : プレイヤーとの衝突判定
				for (auto& bullet : enemyBulletPool.getBullets()) {
					if (not bullet.isActive()) {
						continue;
					}

					if (player.getShape().intersects(Circle{ bullet.getPos(),Config::enemyBulletSize })) {
						if ((not player.isInvincible()) && player.getShape().intersects(Circle{ bullet.getPos(), Config::enemyBulletSize })) {
							player.onHit();
							bullet.deactivate();

							if (player.getHp() <= 0) {
								nowScene = GameScene::Result;
								inputDelayStopWatch.restart();
							}
						}
					}
				}

				//自弾 : 敵との衝突判定
				for (auto& enemy : enemies)
				{
					for (auto& bullet : playerBulletPool.getBullets()) {
						if (not bullet.isActive()) {
							continue;
						}

						if (enemy.getShape().intersects(Circle{ bullet.getPos(),5 })) {
							enemy.onHit();
							bullet.deactivate();
						}
					}
				}

				//プレイヤーとボス本体との衝突判定
				if (boss && boss->hasCollisionDamage() && player.getShape().intersects(boss->getShape())) {
					if (boss && (not player.isInvincible()) && boss->hasCollisionDamage() && player.getShape().intersects(boss->getShape())) {
						player.onHit();
						if (player.getHp() <= 0) {
							inputDelayStopWatch.restart();
							nowPhase = GamePhase::BossExploding;
						}
					}
				}

				// HPが0になった敵を配列から削除
				enemies.remove_if([&](const Enemy& e) {
					if (e.isDead()) {
						scoreItemPool.create(e.getShape().center, 100);
						return true;
					}
					return false;
				});


				if (boss)
				{
					for (auto& bullet : playerBulletPool.getBullets())
					{
						if (bullet.isActive() && boss->getShape().intersects(Circle{ bullet.getPos(), 5 }))
						{
							boss->onHit();
							bullet.deactivate();
						}
					}

					if (boss->isDead())
					{
						boss.reset();
					}
					else
					{
						boss->update(player.getShape().center, sceneSpeed);

						if (boss->hasCollisionDamage() && player.getShape().intersects(boss->getShape()))
						{
							player.onHit();
						}
					}
				}
				for (auto& item : scoreItemPool.getItems()) {
					if (item.isActive() && player.getShape().intersects(item.getShape())) {
						score += item.getValue();
						item.deactivate();
						soundScore->playOneShot();
					}
				}

				break;



			case GamePhase::BossExploding:
			{

				//ボスを倒した後、画面をゆっくりホワイトアウトさせる
				if (not whiteoutTimer.isStarted())	whiteoutTimer.start();


				const double fadeDuration = 2.0;

				whiteoutAlpha = Min(whiteoutTimer.sF() / fadeDuration, 1.0);


				if (whiteoutAlpha >= 1.0) {
					nowPhase = GamePhase::Enging;
				}

				break;
			}
			//==================================nowPhase::Ending==============================
			case GamePhase::Enging:

				if (fewSecondTrue(0.5)) {
					if (showText == true) showText = false;
					else if (showText == false) showText = true;
				}

				if (showText == true) {
					SimpleGUI::GetFont()(U"Clear! : PUSH TO SPACE").drawAt(Scene::Center(), Palette::Black);
				}

				if (KeySpace.pressed()) {
					inputDelayStopWatch.restart();

					nowScene = GameScene::Result;
					nowPhase = GamePhase::Playing;

					GameData::OnGameClear();
					whiteoutAlpha = 0.0;
					whiteoutTimer.reset();

				}
				break;
			}


			break;
		}
			//========================nowScene::Result=====================================
		case GameScene::Result:

			Scene::SetBackground(Palette::Black);

			if (fewSecondTrue(0.5)) {
				if (showText == true) showText = false;
				else if (showText == false) showText = true;
			}

			if (nowPhase != GamePhase::Enging) {
				if (showText == true) {
					SimpleGUI::GetFont()(U"Result : PUSH TO SPACE").drawAt(Scene::Center(), Palette::White);
					SimpleGUI::GetFont()(U"Score: " + Format(score) ).drawAt({Scene::Center().x,Scene::Center().y - 20}, Palette::White);

				}


				if (KeySpace.pressed() && inputDelayStopWatch > 0.5s) {
					inputDelayStopWatch.restart();
					nowScene = GameScene::Title;
				}
			}

			break;

		}







	}
}
