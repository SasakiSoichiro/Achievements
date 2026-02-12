//******************************************
//
// タイマー処理 [timer.h]
// AuthorSasaki Soichiro
//
//******************************************
#ifndef _TIMER_H_ // このマクロ定義がされていなかったら
#define _TIMER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "number.h"

//********************
// タイマークラス
//********************
class CTimer :public CObject
{
public:
	typedef enum
	{
		TEXTUREIDX_NUMBER = 0,
		TEXTUREIDX_COLON,
		TEXTUREIDX_MAX
	}TEXTUREIDX;
	CTimer(int nPriority = 5);	// コンストラクタ
	~CTimer();					// デストラクタ
	static CTimer* Create(D3DXVECTOR3 pos,int nTime);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);// 位置の設定
	void SetRot(D3DXVECTOR3 rot);// 向きの設定
	void Death(void) { m_bDeath = true; }
	static void SetTime(int nTime);			// タイマーの設定
	static void Add(int nTime);				// タイム加算
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; };		// 位置の設定
	D3DXVECTOR2 GetSize(void) { return m_size; };	// 大きさの設定
	static int GetTime(void) { return m_nTime; }	// タイム情報の取得

private:
	// 定数
	static constexpr int nMaxTexture = 2;
	static constexpr int nMaxDigit = 2;
	static constexpr int nSecond = 60;
	static constexpr float fSizeX = 40.0f;
	static constexpr float fSizeY = 60.0f;
	static constexpr float fOffsetLeft = -40.0f;
	static constexpr float fOffsetRight = 80.0f;
	static constexpr float fSizeOffset = -20.0f;
	// ポインタ
	static CNumber* m_apMin[nMaxDigit];	// ナンバーへのポインタ
	static CNumber* m_apSec[nMaxDigit];	// ナンバーへのポインタ
	static CSymbol* m_pSymbol;	// コロンへのポインタ
	// 定数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR2 m_size;	// 大きさ
	// フラグ
	bool m_bDeath;
	// 数値
	int  m_nIdxTexture[nMaxTexture];		// テクスチャのインデックス
	static int m_nTime;	// タイム
	static int m_nFps;	// FPSカウント
};
#endif