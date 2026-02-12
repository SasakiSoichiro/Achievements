//**************************************
//
// お金処理 [money.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _MONEY_H_ // このマクロ定義がされていなかったら
#define _MONEY_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "scene.h"

class CNumber;
class CObject2D;

//*****************
// マクロ定義
//*****************
#define MAX_DIGIT (5)	// 最大桁数

//********************
// お金クラス
//********************
class CMoney :public CObject
{
public:
	CMoney(int nPriority = 5);	// コンストラクタ
	~CMoney();					// デストラクタ
	static CMoney* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void GameInit(D3DXVECTOR3 pos);	// 初期化
	// セッター
	void SetPos(D3DXVECTOR3 pos);		// 位置
	void SetRot(D3DXVECTOR3 rot);		// 向き
	static void SetScore(int nScore);	// スコアの設定
	static void Add(int nValue);		// スコア加算
	int Digit(int nScore);				// 桁数情報の取得
	void Death(void) { m_bDeath = true; }
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }		// 位置情報の取得
	D3DXVECTOR2 GetSize(void) { return m_size; }	// 大きさ情報の取得
	static int GetScore(void) { return m_nScore; }	// スコア取得
private:
	// 定数
	static constexpr int nTextureNum = 2;	// 使うテクスチャの数
	static constexpr float fSizeX = 40.0f;
	static constexpr float fSizeY = 60.0f;
	static constexpr float fOffsetX00 = 200.0f;
	static constexpr float fOffsetX01 = 50.0f;
	static constexpr float fTextureSize_X = 60.0f;
	static constexpr float fTextureSize_Y = 60.0f;
	// ポインタ
	CObject2D* m_pObject2D[2];				// オブジェクト2Dのポインタ
	static CNumber* m_apNumber[MAX_DIGIT];	// ナンバーへのポインタ
	// 座標
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR2 m_size;						// 大きさ
	// フラグ
	bool m_bDeath;
	int  m_nIdxTexture;						// テクスチャのインデックス
	// 数値
	static int m_nScore;					// スコア
};
#endif