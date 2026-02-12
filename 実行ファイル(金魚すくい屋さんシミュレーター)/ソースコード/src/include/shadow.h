//*****************************************
// 
// シェーダ読み込み処理 [shaderManager.h]
// Author:Sasaki Soichiro
//
//*****************************************

#ifndef _SHADOW_H_ // このマクロ定義がされてなかったら
#define _SHADOW_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object3D.h"

//*****************************
// 影クラス
//*****************************
class CShadow : public CObject3D
{
public:
	// コンストラクタ・デストラクタ
	CShadow(int nPriority = 7);
	~CShadow();

	// メンバ関数
	static CShadow* Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetScreen(bool bScreen) { m_bScreen = bScreen; }
private:
	// 定数
	static constexpr float fSize = 40.0f;
	static constexpr float fAlpha = 0.25f;
	// 数値
	int m_nIdxTexture;
	bool m_bScreen;
};

#endif
