//******************************************
//
// スペックパターン処理 [specBase.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _SPECBASE_H_ // このマクロ定義がされていなかったら
#define _SPECBASE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "player.h"

//*************************
// スペックパターンクラス
//*************************
template<typename T>
class CSpecBase
{
public:
	CSpecBase() {}			// コンストラクタ
	virtual~CSpecBase() {}	// デストラクタ
	virtual bool IsCheck(const T& obj)const = 0;
};

class CPlayDisOrigin:public CSpecBase<CPlayer>
{
public:
	CPlayDisOrigin(){}
	~CPlayDisOrigin(){}
	bool IsCheck(const CPlayer& player) const
	{
		const D3DXVECTOR3 pos = player.GetPos();
		D3DXVECTOR3 Origin=VECTOR3_NULL;
		D3DXVECTOR3 toOrigin = Origin - pos;
		float fDist = D3DXVec3Length(&toOrigin);

		return fDist<3003.0f;
	}
};
#endif