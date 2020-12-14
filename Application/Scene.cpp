#include "main.h"
#include "Scene.h"
#include"Utility.h"

void Scene::Draw2D()
{

	switch (sceneType)
	{
	case eSceneTitle:
		//タイトル
		TitleDraw();
		break;

	case eSceneGame:
		//ゲーム本編
		GameDraw();
		break;
	
	case eSceneExplanation:
		//説明
		ExplanationDraw();
		break;
	}

}

void Scene::Update()
{
	switch (sceneType)
	{
	case 0:
		//タイトル
		TitleUpdate();
		break;

	case 1:
		//ゲーム本編
		GameUpdate();
		break;

	case 2:
		//説明
		ExplanationUpdate();
		break;
	}

}

void Scene::Init()
{
	//コンソール作成
	//m_player.CreateConsole();

	Reset();

}

void Scene::Release()
{
	//m_player.DestroyConsole();

	m_playerTex.Release();
	m_slashTex.Release();
	m_bulletTex.Release();
	m_bombTex.Release();
	m_mapTex.Release();
	m_enemyTex.Release();
	titleTex.Release();
	ExpTex.Release();

	delete spriteFont;
}

void Scene::ImGuiUpdate()
{
	//return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
		ImGui::Text("playerX : %.2f", m_player.GetPos().x);
		ImGui::Text("playerY : %.2f", m_player.GetPos().y);
		ImGui::Text("HP : %d", m_player.GetHp());
		ImGui::Text("slashPosX : %f", m_player.GetSword().x);
		ImGui::Text("slashPosY : %f", m_player.GetSword().y);
	}
	ImGui::End();
}

//初期化リセット
void Scene::Reset()
{
	//テクスチャ
	m_playerTex.Load("Texture/Player/player.png");
	m_bulletTex.Load("Texture/Player/shuriken.png");
	m_slashTex.Load("Texture/Player/slash.png");
	m_bombTex.Load("Texture/Player/bomb.png");
	m_mapTex.Load("Texture/Map/Map1.png");
	m_enemyTex.Load("Texture/Enemy/enemy.png");
	titleTex.Load("Texture/Title/Op.png");
	ExpTex.Load("Texture/Title/ex.png");

	//マップ
	m_map.Init();
	m_map.SetTexture(&m_mapTex);
	m_map.SetOwner(this);

	//プレイヤー
	m_player.Init();
	m_player.SetTexture(&m_playerTex);
	m_player.SetOwner(this);
	m_player.SetBulletTexture(&m_bulletTex);
	m_player.SetSlashTexture(&m_slashTex);
	m_player.SetBombTexture(&m_bombTex);


	//敵
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Init();					// 敵：初期化
		m_enemy[i].SetTexture(&m_enemyTex);// 画像のポインタをenemyクラスに渡す
		m_enemy[i].SetOwner(this);
	}

	m_enemy[0].SetConfiguration(
		Math::Vector2(0.0f, 0.0f),	// X,Y座標
		Math::Vector2(5.0f, 1.0f));	// X,Yの移動量

	m_enemy[1].SetConfiguration(
		Math::Vector2(200.0f, 250.0f),
		Math::Vector2(100.0f, 1.0f));

	m_enemy[2].SetConfiguration(
		Math::Vector2(-300.0f, -150.0f),
		Math::Vector2(5.0f, 1.0f));


	//初期シーン　タイトル
	sceneType = eSceneGame;	//0:タイトル　1:ゲーム本編

	//キー制御
	keyFlg = 0;		//0:押してない1:押している
	clickFlg = false;

	//ゲームスタートボタン
	gameStartPos = { 9, -10 };
	gameStartFlg = false;

	//説明スタートボタン
	ExplanationStartPos = { 9, -137 };
	ExplanationStartFlg = false;

	//タイトルスタートボタン
	TitleStartPos = { 488, -243 };
	TitleStartFlg = false;
}

//マップクラス取得
CMap* Scene::GetMap()
{
	return &m_map;
}

//敵クラス取得(配列アドレス取得)
CEnemy* Scene::GetEnemy()
{
	return m_enemy;
}

//プレイヤークラス取得
CPlayer Scene::GetPlayer()
{
	return m_player;
}


//タイトル:更新
void Scene::TitleUpdate()
{
	GetMousePos();

	////////////////////////////////////////////////////////////
	//	タイトルからゲーム本編								///
	////////////////////////////////////////////////////////////

	float Left = gameStartPos.x - 190;
	float Right = gameStartPos.x + 190;
	float Top = gameStartPos.y + 50;
	float Bottom = gameStartPos.y - 50;

	gameStartFlg = false;

	//強調枠内の判定
	if (mouse.cur.x > Left && mouse.cur.x < Right)
	{
		if (mouse.cur.y > Bottom && mouse.cur.y < Top)
		{
			gameStartFlg = true;	//枠を表示

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				sceneType = eSceneGame;	//ゲーム画面移行
				clickFlg = true;
			}
			else
			{
				clickFlg = false;
			}
		}
	}

	///////////////////////////////////////////////////////
	//	タイトルから説明								///
	///////////////////////////////////////////////////////

	float exLeft = ExplanationStartPos.x - 150;
	float exRight = ExplanationStartPos.x + 150;
	float exTop = ExplanationStartPos.y + 43;
	float exBottom = ExplanationStartPos.y - 43;

	ExplanationStartFlg = false;

	//強調枠内の判定
	if (mouse.cur.x > exLeft && mouse.cur.x < exRight)
	{
		if (mouse.cur.y > exBottom && mouse.cur.y < exTop)
		{
			ExplanationStartFlg = true;	//強調枠表示

			if (clickFlg == false)
			{
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					sceneType = eSceneExplanation;	//説明画面に移行
					clickFlg = true;
				}
				else
				{
					clickFlg = false;
				}
			}
		}
	}

}

//タイトル:描画
void Scene::TitleDraw()
{
	Titlemat = DirectX::XMMatrixTranslation(0, 0, 0);//ここは座標
	SHADER.m_spriteShader.SetMatrix(Titlemat);
	SHADER.m_spriteShader.DrawTex(&titleTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	//強調枠描画
	if (gameStartFlg)	//ゲーム
	{
		color = { 1,1,0,1 };
		SHADER.m_spriteShader.DrawBox(gameStartPos.x, gameStartPos.y, 190, 50, &color, false);
	}
	if (ExplanationStartFlg)	//説明
	{
		color = { 1,0,0,1 };
		SHADER.m_spriteShader.DrawBox(ExplanationStartPos.x, ExplanationStartPos.y, 150, 43, &color, false);
	}

}


//ゲーム:更新
void Scene::GameUpdate()
{
	//マップ
	m_map.Update();

	//プレイヤー
	m_player.Updata();

	//プレイヤー死亡でリザルト画面へ
	/*if (!m_player.IsAlive())
	{
		sceneType = eSceneResult
	}*/

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Update();	// 敵：更新
	}

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		if (keyFlg == 0)
		{
			Reset();
			keyFlg = 1;
			sceneType = eSceneTitle;//タイトルへ
		}
	}
	else
	{
		keyFlg = 0;
	}

}

//ゲーム:描画
void Scene::GameDraw()
{
	m_map.Draw();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Draw();		// 敵：描画
	}
	//プレイヤー
	m_player.Draw();

}

//説明:更新
void Scene::ExplanationUpdate()
{
	GetMousePos();

	////////////////////////////////////////////////////////
	//	説明からタイトル								///
	//////////////////////////////////////////////////////

	float eLeft = TitleStartPos.x - 80;
	float eRight = TitleStartPos.x + 80;
	float eTop = TitleStartPos.y + 80;
	float eBottom = TitleStartPos.y - 80;

	TitleStartFlg = false;

	//強調枠内の判定
	if (mouse.cur.x > eLeft && mouse.cur.x < eRight)
	{
		if (mouse.cur.y > eBottom && mouse.cur.y < eTop)
		{
			TitleStartFlg = true;	//強調枠表示

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (clickFlg == false)
				{
					sceneType = eSceneTitle;	//タイトル移行
					clickFlg = true;
				}
			}
			else
			{
				clickFlg = false;
			}
		}
	}

}

//説明:描画
void Scene::ExplanationDraw()
{
	Titlemat = DirectX::XMMatrixTranslation(0, 0, 0);//ここは座標
	SHADER.m_spriteShader.SetMatrix(Titlemat);
	SHADER.m_spriteShader.DrawTex(&ExpTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	if (TitleStartFlg)
	{
		//強調枠
		color = { 0,0,1,1 };
		SHADER.m_spriteShader.DrawBox(TitleStartPos.x, TitleStartPos.y, 80, 80, &color, false);
	}
}


//マウス座標取得関数
void Scene::GetMousePos()
{
	GetCursorPos(&mouse.cur);
	ScreenToClient(APP.m_window.GetWndHandle(), &mouse.cur);

	mouse.cur.x -= Screen::HalfWidth;
	mouse.cur.y -= Screen::HalfHeight;
	mouse.cur.y *= -1;
}
