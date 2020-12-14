#include "main.h"

#include "TestCamera.h"

void TestCamera::Update()
{
	//============================
	// ビュー行列
	//============================
	POINT nowPos;
	GetCursorPos(&nowPos);

	// Shiftで大きく
	float ratio = 1.0f;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		ratio = 5;
	}
	// Controlで小さく
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
		ratio = 0.2f;
	}

	// マウスの右ボタン
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		// マウスの前回からの移動量
		float deltaX = (float)(nowPos.x - m_PrevMousePos.x) * 0.5f * ratio;
		float deltaY = (float)(nowPos.y - m_PrevMousePos.y) * 0.5f * ratio;

		// カメラ回転 Y軸回転
		Math::Matrix mR;
		mR = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(deltaX));
		m_mCam *= mR;

		// カメラ回転 X軸回転

		m_mCam._31;
		mR = Math::Matrix::CreateFromAxisAngle(m_mCam.Right(), DirectX::XMConvertToRadians(deltaY));
		m_mCam *= mR;
	}

	// ホイールドラッグ
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		float deltaX = (float)(nowPos.x - m_PrevMousePos.x) * 0.05f * ratio;
		float deltaY = (float)(nowPos.y - m_PrevMousePos.y) * 0.05f * ratio;

		m_vCamBasePos -= m_mCam.Right() * deltaX;
		m_vCamBasePos += m_mCam.Up() * deltaY;
	}

	// ホイール
	if (APP.m_window.GetMouseWheelVal()) {
		Math::Matrix mT;
		mT = Math::Matrix::CreateTranslation(0, 0, (float)APP.m_window.GetMouseWheelVal() * 0.01f * ratio);

		m_mCam = mT * m_mCam;
	}

	m_PrevMousePos = nowPos;
}

void TestCamera::SetToShader()
{
	// カメラ行列からビュー行列へ変換
	Math::Matrix mCam = m_mCam;
	mCam *= Math::Matrix::CreateTranslation(m_vCamBasePos);

	SHADER.m_cb7_Camera.Work().CamPos = mCam.Translation();
	SHADER.m_cb7_Camera.Work().mV = mCam.Invert();
	SHADER.m_cb7_Camera.Work().mP = m_mProj;

	// カメラ情報(ビュー行列、射影行列)を、各シェーダの定数バッファへセット
	SHADER.m_cb7_Camera.Write();
}
