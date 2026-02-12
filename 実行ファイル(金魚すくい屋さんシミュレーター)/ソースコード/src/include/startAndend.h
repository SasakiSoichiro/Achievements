//***************************************
// 
// 開始と終わりのUI処理 [startAndend.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _STARTANDEND_H_ // このマクロ定義がされていなかったら
#define _STARTANDEND_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object2D.h"

//************************
// 開始と終わりのUIクラス
//************************
class CStartAndEnd :public CObject2D
{
public:
	typedef enum
	{
		TEXTUREIDX_START=0,
		TEXTUREIDX_END,
		TEXTUREIDX_MAX
	}TEXTUREIDX;
	CStartAndEnd(int nPriority = 5);	// コンストラクタ
	~CStartAndEnd();					// デストラクタ
	static CStartAndEnd* Create(D3DXVECTOR3 pos);// インスタンス生成
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
	static constexpr int nStartScreenTime = 100;
	// フラグ
	bool m_bStart;
	bool m_bDeath;
	// 数値
	int m_nIdxTexture[TEXTUREIDX_MAX];	// 使っているテクスチャの番号
	int m_nCnt;
};
#endif