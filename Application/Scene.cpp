#include "main.h"
#include "Scene.h"
#include"Utility.h"

void Scene::Draw2D()
{

	switch (sceneType)
	{
	case eSceneTitle:
		//�^�C�g��
		TitleDraw();
		break;

	case eSceneGame:
		//�Q�[���{��
		GameDraw();
		break;
	
	case eSceneExplanation:
		//����
		ExplanationDraw();
		break;
	}

}

void Scene::Update()
{
	switch (sceneType)
	{
	case 0:
		//�^�C�g��
		TitleUpdate();
		break;

	case 1:
		//�Q�[���{��
		GameUpdate();
		break;

	case 2:
		//����
		ExplanationUpdate();
		break;
	}

}

void Scene::Init()
{
	//�R���\�[���쐬
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

	// �f�o�b�O�E�B���h�E
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

//���������Z�b�g
void Scene::Reset()
{
	//�e�N�X�`��
	m_playerTex.Load("Texture/Player/player.png");
	m_bulletTex.Load("Texture/Player/shuriken.png");
	m_slashTex.Load("Texture/Player/slash.png");
	m_bombTex.Load("Texture/Player/bomb.png");
	m_mapTex.Load("Texture/Map/Map1.png");
	m_enemyTex.Load("Texture/Enemy/enemy.png");
	titleTex.Load("Texture/Title/Op.png");
	ExpTex.Load("Texture/Title/ex.png");

	//�}�b�v
	m_map.Init();
	m_map.SetTexture(&m_mapTex);
	m_map.SetOwner(this);

	//�v���C���[
	m_player.Init();
	m_player.SetTexture(&m_playerTex);
	m_player.SetOwner(this);
	m_player.SetBulletTexture(&m_bulletTex);
	m_player.SetSlashTexture(&m_slashTex);
	m_player.SetBombTexture(&m_bombTex);


	//�G
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Init();					// �G�F������
		m_enemy[i].SetTexture(&m_enemyTex);// �摜�̃|�C���^��enemy�N���X�ɓn��
		m_enemy[i].SetOwner(this);
	}

	m_enemy[0].SetConfiguration(
		Math::Vector2(-300.0f, -1144.0f),	// X,Y���W
		Math::Vector2(5.0f, 1.0f));	// X,Y�̈ړ���

	m_enemy[1].SetConfiguration(
		Math::Vector2(200.0f, 250.0f),
		Math::Vector2(100.0f, 1.0f));

	m_enemy[2].SetConfiguration(
		Math::Vector2(-300.0f, -150.0f),
		Math::Vector2(5.0f, 1.0f));


	//�����V�[���@�^�C�g��
	sceneType = eSceneGame;	//0:�^�C�g���@1:�Q�[���{��

	//�L�[����
	keyFlg = 0;		//0:�����ĂȂ�1:�����Ă���
	clickFlg = false;

	//�Q�[���X�^�[�g�{�^��
	gameStartPos = { 9, -10 };
	gameStartFlg = false;

	//�����X�^�[�g�{�^��
	ExplanationStartPos = { 9, -137 };
	ExplanationStartFlg = false;

	//�^�C�g���X�^�[�g�{�^��
	TitleStartPos = { 488, -243 };
	TitleStartFlg = false;
}

//�}�b�v�N���X�擾
CMap* Scene::GetMap()
{
	return &m_map;
}

//�G�N���X�擾(�z��A�h���X�擾)
CEnemy* Scene::GetEnemy()
{
	return m_enemy;
}

//�v���C���[�N���X�擾
CPlayer Scene::GetPlayer()
{
	return m_player;
}


//�^�C�g��:�X�V
void Scene::TitleUpdate()
{
	GetMousePos();

	////////////////////////////////////////////////////////////
	//	�^�C�g������Q�[���{��								///
	////////////////////////////////////////////////////////////

	float Left = gameStartPos.x - 190;
	float Right = gameStartPos.x + 190;
	float Top = gameStartPos.y + 50;
	float Bottom = gameStartPos.y - 50;

	gameStartFlg = false;

	//�����g���̔���
	if (mouse.cur.x > Left && mouse.cur.x < Right)
	{
		if (mouse.cur.y > Bottom && mouse.cur.y < Top)
		{
			gameStartFlg = true;	//�g��\��

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				sceneType = eSceneGame;	//�Q�[����ʈڍs
				clickFlg = true;
			}
			else
			{
				clickFlg = false;
			}
		}
	}

	///////////////////////////////////////////////////////
	//	�^�C�g���������								///
	///////////////////////////////////////////////////////

	float exLeft = ExplanationStartPos.x - 150;
	float exRight = ExplanationStartPos.x + 150;
	float exTop = ExplanationStartPos.y + 43;
	float exBottom = ExplanationStartPos.y - 43;

	ExplanationStartFlg = false;

	//�����g���̔���
	if (mouse.cur.x > exLeft && mouse.cur.x < exRight)
	{
		if (mouse.cur.y > exBottom && mouse.cur.y < exTop)
		{
			ExplanationStartFlg = true;	//�����g�\��

			if (clickFlg == false)
			{
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					sceneType = eSceneExplanation;	//������ʂɈڍs
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

//�^�C�g��:�`��
void Scene::TitleDraw()
{
	Titlemat = DirectX::XMMatrixTranslation(0, 0, 0);//�����͍��W
	SHADER.m_spriteShader.SetMatrix(Titlemat);
	SHADER.m_spriteShader.DrawTex(&titleTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	//�����g�`��
	if (gameStartFlg)	//�Q�[��
	{
		color = { 1,1,0,1 };
		SHADER.m_spriteShader.DrawBox(gameStartPos.x, gameStartPos.y, 190, 50, &color, false);
	}
	if (ExplanationStartFlg)	//����
	{
		color = { 1,0,0,1 };
		SHADER.m_spriteShader.DrawBox(ExplanationStartPos.x, ExplanationStartPos.y, 150, 43, &color, false);
	}

}


//�Q�[��:�X�V
void Scene::GameUpdate()
{
	//�}�b�v
	m_map.Update();

	//�v���C���[
	m_player.Updata();

	//�v���C���[���S�Ń��U���g��ʂ�
	/*if (!m_player.IsAlive())
	{
		sceneType = eSceneResult
	}*/

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Update();	// �G�F�X�V
	}

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		if (keyFlg == 0)
		{
			Reset();
			keyFlg = 1;
			sceneType = eSceneTitle;//�^�C�g����
		}
	}
	else
	{
		keyFlg = 0;
	}

}

//�Q�[��:�`��
void Scene::GameDraw()
{
	m_map.Draw();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_enemy[i].Draw();		// �G�F�`��
	}
	//�v���C���[
	m_player.Draw();

}

//����:�X�V
void Scene::ExplanationUpdate()
{
	GetMousePos();

	////////////////////////////////////////////////////////
	//	��������^�C�g��								///
	//////////////////////////////////////////////////////

	float eLeft = TitleStartPos.x - 80;
	float eRight = TitleStartPos.x + 80;
	float eTop = TitleStartPos.y + 80;
	float eBottom = TitleStartPos.y - 80;

	TitleStartFlg = false;

	//�����g���̔���
	if (mouse.cur.x > eLeft && mouse.cur.x < eRight)
	{
		if (mouse.cur.y > eBottom && mouse.cur.y < eTop)
		{
			TitleStartFlg = true;	//�����g�\��

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (clickFlg == false)
				{
					sceneType = eSceneTitle;	//�^�C�g���ڍs
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

//����:�`��
void Scene::ExplanationDraw()
{
	Titlemat = DirectX::XMMatrixTranslation(0, 0, 0);//�����͍��W
	SHADER.m_spriteShader.SetMatrix(Titlemat);
	SHADER.m_spriteShader.DrawTex(&ExpTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	if (TitleStartFlg)
	{
		//�����g
		color = { 0,0,1,1 };
		SHADER.m_spriteShader.DrawBox(TitleStartPos.x, TitleStartPos.y, 80, 80, &color, false);
	}
}


//�}�E�X���W�擾�֐�
void Scene::GetMousePos()
{
	GetCursorPos(&mouse.cur);
	ScreenToClient(APP.m_window.GetWndHandle(), &mouse.cur);

	mouse.cur.x -= Screen::HalfWidth;
	mouse.cur.y -= Screen::HalfHeight;
	mouse.cur.y *= -1;
}
