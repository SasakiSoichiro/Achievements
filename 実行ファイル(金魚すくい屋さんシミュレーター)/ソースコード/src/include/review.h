//***************************************
// 
// レビュー処理 [review.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _REVIEW_H_ // このマクロ定義がされていなかったら
#define _REVIEW_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "object3DMulti.h"

//********************
// レビュークラス
//********************
class CReview :public CObject3DMulti
{
public:
	typedef enum
	{
		TEXTUREIDX_STAR=0,
		TEXTUREIDX_COLOR,
		TEXTUREIDX_MAX
	}TEXTUREIDX;
	CReview(int nPriority = 5);	// コンストラクタ
	~CReview();					// デストラクタ
	static CReview* Create(D3DXVECTOR3 pos);
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置設定
	void SetRot(D3DXVECTOR3 rot);	// 向き設定
	static void Write(float fScore);// 書き込み
	static void Reset(void) { m_fScore = 0.0f; }
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
private:
	// 定数
	static constexpr int nMaxMulti = 2;
	static constexpr float fMaxReview = 5.0f;
	static constexpr float fTexHalf = 0.5f;
	static constexpr float fSizeX = 50.0f;
	static constexpr float fSizeY = 10.0f;
	// 座標
	D3DXVECTOR2 m_size;				// 大きさ
	// 数値
	int  m_nIdxTexture[nMaxMulti];	// テクスチャのインデックス
	static float m_fScore;
};

#endif