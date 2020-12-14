#include"CMap.h"
#include"Scene.h"
#include"Utility.h"

CMap::CMap()
	:m_pTexture(nullptr)	//�摜(�e�N�X�`��)
	, m_ChipData()			//�}�b�v�f�[�^
	, m_chipX()				//���WX
	, m_chipY()				//���WY
	, m_mat()				//�s��
	, m_scroll(0,0)			//�X�N���[����
{
}

CMap::~CMap()
{
}

//������
void CMap::Init()
{
	
	mapData = OutSide;

	//�}�b�v��񃍁[�h
	LoadMapFile();

	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			m_chipX[h][w] = (64.0f * w) - 640;
			m_chipY[h][w] = (-64.0f * h) + 360;
		}
	}
}

//�X�V
void CMap::Update()
{
	/*�f�o�b�O�L�[*/
	//���X�N���[������
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_scroll.x -= 10.0f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_scroll.x += 10.0f;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_scroll.y += 10.0f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_scroll.y -= 10.0f;
	
	//�}�b�v�X�V
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			//�X�N���[������
			Scroll();

			//��ʒ[�̃��~�b�g��`
			ScrollRemit();

		}
	}
	
	//�s��쐬
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			m_mat[h][w] = DirectX::XMMatrixTranslation(
				m_chipX[h][w] - m_scroll.x, m_chipY[h][w] - m_scroll.y, 0.0f);
		}
	}
}

//�`��
void CMap::Draw()
{
	Math::Rectangle chipRect;

	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			if (m_ChipData[h][w] == 0)		//��
			{
				chipRect = { 0,0,64,64 };
			}
			else if (m_ChipData[h][w] == 1)
			{
				chipRect = { 64,0,64,64 };
			}
			else if (m_ChipData[h][w] == 2)
			{
				chipRect = { 128,0,64,64 };
			}
			else if (m_ChipData[h][w] == 3)
			{
				chipRect = { 192,0,64,64 };
			}
			else if (m_ChipData[h][w] == 4)
			{
				chipRect = { 256,0,64,64 };
			}
			else if (m_ChipData[h][w] == 5)
			{
				chipRect = { 320,0,64,64 };
			}
			else if (m_ChipData[h][w] == 7)
			{
				chipRect = { 384,0,64,64 };
			}
			else if (m_ChipData[h][w] == 6)
			{
				chipRect = {448 ,0,64,64 };
			}
			else if (m_ChipData[h][w] == 9)
			{
				chipRect = {512 ,0,64,64 };
			}
			else if (m_ChipData[h][w] == 8)
			{
				chipRect = { 576,0,64,64 };
			}
			SHADER.m_spriteShader.SetMatrix(m_mat[h][w]);
			SHADER.m_spriteShader.DrawTex(m_pTexture, chipRect, 1.0f);
		}
	}
}

//�e�N�X�`���ݒ�
void CMap::SetTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr) return;

	m_pTexture = apTexture;
}

//�}�b�vX���W�擾
float(*CMap::GetPosX())[MAP_CHIP_W]
{
	return m_chipX;
}

//�}�b�vY���W�擾
float(*CMap::GetPosY())[MAP_CHIP_W]
{
	return m_chipY;
}

//�}�b�v�f�[�^�擾
int(*CMap::GetChipData())[MAP_CHIP_W]
{
	return m_ChipData;
}

//�X�N���[���ʎ擾
Math::Vector2 CMap::GetscrollPos()
{
	return m_scroll;
}

//�I�[�i�[�ݒ�
void CMap::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}

void CMap::SetMapData()
{
	if (mapData == OutSide)
	{
		mapData = OneFloor;
	}
	else if (mapData == OneFloor)
	{
		mapData = TwoFloor;
	}


}

//�X�N���[������
void CMap::Scroll()
{
	//�v���C���[���W�擾
	CPlayer player = m_pOwner->GetPlayer();
	Math::Vector2 playerPos = player.GetPos();

	//�X�N���[�����W
	m_scroll = playerPos;
}

//��ʒ[���~�b�g��`
void CMap::ScrollRemit()
{
	static const float SCR_X_MIN = m_chipX[0][0] + 640;
	static const float SCR_X_MAX = m_chipX[0][MAP_CHIP_W - 1] - 640;

	static const float SCR_Y_MIN = m_chipY[MAP_CHIP_H - 1][0] + 360;
	static const float SCR_Y_MAX = m_chipY[0][0] - 360;

	//���[����
	if (m_scroll.x < SCR_X_MIN)
		m_scroll.x = SCR_X_MIN; //�[�ɓ��B���Œ�
	//�E�[����
	else if (m_scroll.x > SCR_X_MAX)
		m_scroll.x = SCR_X_MAX; //�[�ɓ��B���Œ�
	//���[����
	if (m_scroll.y < SCR_Y_MIN)
		m_scroll.y = SCR_Y_MIN; //�[�ɓ��B���Œ�
	//��[����
	else if (m_scroll.y > SCR_Y_MAX)
		m_scroll.y = SCR_Y_MAX; //�[�ɓ��B���Œ�
}


//�}�b�v�ǂݍ���
void CMap::LoadMapFile()
{
	FILE* fp; // �t�@�C���̏����i�[����\����

	

	// �t�@�C���ǂݍ���
	if (mapData == OutSide)	//�}�b�v�P
	{
		if ((fp = fopen("Data/Map/map1-40.csv", "r")) != nullptr)
		{
			int h;
			int w;

			for (h = 0; h < 40; h++)
			{
				for (w = 0; w < 40; w++)
				{
					fscanf(fp, "%d,", &m_ChipData[h][w]);
				}
			}
			for (h = 0; h < 40; h++)
			{
				for (w = 0; w < 40; w++)
				{
					printf("%d ", &m_ChipData[h][w]);
				}
				printf("\n");
			}
			fclose(fp);
		}
	}
	else if (mapData == OneFloor)	//�}�b�v�Q
	{
		//constexpr int MAP_CHIP_H = 26;					//�c�T�C�Y
		//constexpr int MAP_CHIP_W = 28;					//���T�C�Y

		if ((fp = fopen("Data/Map/map40.txt", "r")) != nullptr)
		{
			char ch; // �������ꕨ
			int h = 0;
			int w = 0;

			while ((ch = fgetc(fp)) != EOF)
			{

				if (ch == '\n') // ���s�̏ꍇ
				{
					h++;
					w = 0;
				}
				else //���̑�
				{
					//1�����𐔎��Ƃ��Ċi�[ �@���������琔���ɕϊ�
					m_ChipData[h][w] = ch - 0x30;
					w++;
				}
			}
			fclose(fp);
		}
	}
	


	// �R���\�[���o�̓f�[�^���m�F
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			printf("%d", m_ChipData[h][w]);
		}
		printf("\n");
	}
}


//�R���\�[���쐬
void CMap::CreateConsole()
{
	// �R���\�[�������蓖�Ă�
	AllocConsole();

	//�W�����o�͂����蓖�Ă�
	freopen_s(&fp_c, "CONOUT$", "w", stdout);
	//	freopen_s(&fp_c, "CONIN$", "r", stdin);
}

// �R���\�[���j��
void CMap::DestroyConsole()
{
	fclose(fp_c);

	// �R���\�[���j��
	FreeConsole();
}
