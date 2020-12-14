#include "Utility.h"

//四辺の当たり判定チェック
//当たったらtrueかfalseを返す
bool Utility::bHitCheck(Math::Vector2 aScrPos, Math::Vector2 aScrMove, Math::Vector2 aDestPos, float aScrRad, float aDesRad)
{
	//キャラ1の現在座標の四辺
	const float aScrLeft = aScrPos.x - Infor::RADIUS_32;		//左辺
	const float aScrRight = aScrPos.x + Infor::RADIUS_32;		//右辺
	const float aScrTop = aScrPos.y + Infor::RADIUS_32;		//上辺
	const float aScrBottom = aScrPos.y - Infor::RADIUS_32;		//下辺

	//キャラ1の未来座標の四辺
	const float nextLeft = aScrLeft + aScrMove.x;				//左辺
	const float nextRight = aScrRight + aScrMove.x;			//右辺
	const float nextTop = aScrTop + aScrMove.y;				//上辺
	const float nextBottom = aScrBottom + aScrMove.y;			//下辺

	//キャラ2の現在座標の四辺
	const float aDestLeft = aDestPos.x - Infor::RADIUS_32;	//左辺
	const float aDestRight = aDestPos.x + Infor::RADIUS_32;	//右辺
	const float aDestTop = aDestPos.y + Infor::RADIUS_32;	//上辺
	const float aDestBottom = aDestPos.y - Infor::RADIUS_32;	//下辺

	/* 現在座標 :左右当たり判定*/
	if (aScrRight > aDestLeft && aScrLeft < aDestRight)
	{
		//未来座標 :上からあたったか判定
		if (nextBottom < aDestTop && nextTop > aDestTop)
		{
			return false;
		}
		//未来座標 :下からあたったか判定
		else if (aScrTop > aDestBottom && aScrBottom < aDestBottom)
		{
			return false;
		}
	}

	/* 現在座標 :上下当たり判定*/
	if (aScrTop > aDestBottom && aScrBottom < aDestTop)
	{
		//未来座標 :左からあたったか判定
		if (nextRight > aDestLeft && nextLeft < aDestLeft)
		{
			return false;
		}
		//未来座標 :右からあたったか判定
		else if (nextLeft < aDestRight && nextRight > aDestRight)
		{
			return false;
		}
	}
}

//四辺の当たり判定を個々に判定
//1:上判定 2:下判定 3:右 4:左
int Utility::iHitCheck(Math::Vector2 aScrPos, Math::Vector2 aScrMove, float aDestPosX, float aDestPosY, float aScrRad, float aDesRad)
{
	//キャラ1の現在座標の四辺
	const float aScrLeft = aScrPos.x - Infor::RADIUS_32;		//左辺
	const float aScrRight = aScrPos.x + Infor::RADIUS_32;		//右辺
	const float aScrTop = aScrPos.y + Infor::RADIUS_32;		//上辺
	const float aScrBottom = aScrPos.y - Infor::RADIUS_32;		//下辺

	//キャラ1の未来座標の四辺
	const float nextLeft = aScrLeft + aScrMove.x;				//左辺
	const float nextRight = aScrRight + aScrMove.x;			//右辺
	const float nextTop = aScrTop + aScrMove.y;				//上辺
	const float nextBottom = aScrBottom + aScrMove.y;			//下辺

	//キャラ2の現在座標の四辺
	const float aDestLeft = aDestPosX - Infor::RADIUS_32;	//左辺
	const float aDestRight = aDestPosX + Infor::RADIUS_32;	//右辺
	const float aDestTop = aDestPosY + Infor::RADIUS_32;	//上辺
	const float aDestBottom = aDestPosY - Infor::RADIUS_32;	//下辺

	/* 現在座標 :左右当たり判定*/
	if (aScrRight > aDestLeft && aScrLeft < aDestRight)
	{
		//未来座標 :上からあたったか判定
		if (nextBottom < aDestTop && nextTop > aDestTop)
		{
			return 1;	//上に押し戻し
		}
		//未来座標 :下からあたったか判定
		else if (aScrTop > aDestBottom && aScrBottom < aDestBottom)
		{
			return 2;	//下に押し戻し
		}
	}

	/* 現在座標 :上下当たり判定*/
	if (aScrTop > aDestBottom && aScrBottom < aDestTop)
	{
		//未来座標 :左からあたったか判定
		if (nextRight > aDestLeft && nextLeft < aDestLeft)
		{
			return 3;	//左に押し戻し
		}
		//未来座標 :右からあたったか判定
		else if (nextLeft < aDestRight && nextRight > aDestRight)
		{
			return 4;	//右に押し戻し
		}
	}

	return 0;
}

