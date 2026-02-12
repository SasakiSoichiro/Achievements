//**************************************************
// 
// Rスティック処理 [rStick.h]
// Author:Sasaki Soichiro
//
//**************************************************
#ifndef _RSTICK_H_ // このマクロ定義がされていなかったら
#define _RSTICK_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object2D.h"

//************************
// Rスティッククラス
//************************
class CRStick :public CObject2D
{
public:
	CRStick(int nPriority = 7);	// コンストラクタ
	~CRStick();					// デストラクタ
	static CRStick* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置設定
	void SetRot(D3DXVECTOR3 rot);	// 向き設定
	void Death(void) { m_bDeath = true; }
	static void SetScreen(bool bScreen) { m_bScreen = bScreen; }
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
	static D3DXVECTOR2 GetUIPos(void) { return m_TouchPos; }
private:
	// 定数
	static constexpr int nAlphaCnt = 60;
	static constexpr int nDeadZone = 10000;
	static constexpr float fSpeed = 10.0f;
	static constexpr float fSizeX = 40.0f;
	static constexpr float fSizeY = 40.0f;
	// 座標
	D3DXVECTOR2 m_move;
	D3DXVECTOR2 m_RstickPos;
	D3DXVECTOR2 m_RstickOldPos;
	D3DXVECTOR2 m_MouseOldPos;
	static D3DXVECTOR2 m_TouchPos;
	// フラグ
	bool m_bDeath;
	static bool m_bScreen;
	// 数値
	int m_nIdxTexture;			// 使っているテクスチャの番号
	float m_fSpeed;
};

#endif