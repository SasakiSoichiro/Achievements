//**************************************
//
// ボウル処理 [bowl.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _BOWL_H_ // このマクロ定義がされていなかったら
#define _BOWL_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"
#include <vector>

//********************
// 前方宣言
//********************
class CGoldFish;

//********************
// ボウルクラス
//********************
class CBowl :public CObjectX
{
public:

	CBowl(int nPriority = 4);	// コンストラクタ
	~CBowl();					// デストラクタ
	static CBowl* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool Area(D3DXVECTOR3 pos);		// 注文範囲
	void AddFish(void) { m_nFishCnt++; }// 魚
	void Death(void) { m_bDeath = true; }
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	int GetFishCnt(void) { return m_nFishCnt; }// 魚の数
	bool GetDeath(void) { return m_bDeath; }
	D3DXVECTOR3 GetMove(void);
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
private:

	static constexpr float fAreaRadius = 8.0f;
	// 座標
	D3DXVECTOR3 m_posOld;	// 前フレームの位置
	// フラグ
	bool m_bDeath;
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号
	int m_nFishCnt;			// 魚のカウント

};
#endif