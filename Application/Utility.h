#pragma once

//画面情報
namespace Screen
{
	const int Width = 1280;				//画面幅
	const int Height = 720;				//画面高
	const int HalfWidth = Width / 2;	//画面幅半分
	const int HalfHeight = Height / 2;	//画面高半分

}

//キャラ情報
namespace Infor
{
	//半径情報
	const float RADIUS_32 = 32.0f;
	const float RADIUS_16 = 16.0f;
	const float RADIUS_8 = 8.0f;

}

//汎用処理欄
namespace Utility
{
	/*当たり判定処理*/
	//0か1を返還（キャラ１座標、キャラ１移動量、キャラ２座標、キャラ１半径、キャラ２半径）
	bool bHitCheck(Math::Vector2 aScrPos,Math::Vector2 aScrMove, Math::Vector2 aDestPos, float aScrRad, float aDesRad);				
	//0または1～4を返還
	int  iHitCheck(Math::Vector2 aScrPos,Math::Vector2 aScrMove, float aDestPosX, float aDestPosY, float aScrRad, float aDesRad);

}