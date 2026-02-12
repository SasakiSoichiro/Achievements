//**************************************
//
// 矢印処理 [arrow.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _ARROW_H_ // このマクロ定義がされていなかったら
#define _ARROW_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

//********************
// 矢印クラス
//********************
class CArrow :public CObjectX
{
public:

	CArrow(int nPriority = 4);	// コンストラクタ
	~CArrow();					// デストラクタ
	static CArrow* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
private:
	// 定数
	static constexpr int maxLife = 240;
	static constexpr float fRatioSin = 0.1f;
	static constexpr float fCross = 5.0f;
	static constexpr float fRotYDelta = 0.1f;
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号
	int m_nLife;
	float m_fHeightOffset;
};

#endif