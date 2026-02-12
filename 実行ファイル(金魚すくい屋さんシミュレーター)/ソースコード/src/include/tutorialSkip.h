//**************************************************
// 
// チュートリアルスキップUI処理 [tutorialSkip.h]
// Author:Sasaki Soichiro
//
//**************************************************
#ifndef _TUTORIALSKIP_H_ // このマクロ定義がされていなかったら
#define _TUTORIALSKIP_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object2D.h"

//****************************************
// チュートリアルスキップUIクラス
//****************************************
class CTutorialSkip :public CObject2D
{
public:
	CTutorialSkip(int nPriority = 5);
	~CTutorialSkip();
	static CTutorialSkip* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置設定
	void SetRot(D3DXVECTOR3 rot);	// 向き設定
	void Death(void) { m_bDeath = true; }
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
private:
	// 定数
	static constexpr int nAlphaCnt = 60;
	// フラグ
	bool m_bDeath;
	// 数値
	int m_nIdxTexture;			// 使っているテクスチャの番号
	int m_nCnt;
};

#endif