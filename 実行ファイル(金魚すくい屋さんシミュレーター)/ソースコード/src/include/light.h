//************************************
//
// ライト処理 [light.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _LIGHT_H_//このマクロ定義がされていなかったら
#define _LIGHT_H_//二重インクルード防止のマクロ定義

//*****************
//	インクルード
//*****************
#include "main.h"

//******************
// ライトクラス
//******************
class CLight
{
public:
	CLight();	// コンストラクタ
	~CLight();	// デストラクタ
	void Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
private:
	static constexpr int nMaxNum = 3;
	// 座標
	D3DXVECTOR3 m_aVec[nMaxNum];// ベクトル
	// 構造体
	D3DLIGHT9 m_aLight[nMaxNum];// ライト

};

#endif