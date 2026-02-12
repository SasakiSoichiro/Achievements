//**************************************
//
// 水槽処理 [aquarium.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _AQUARIUM_H_ // このマクロ定義がされていなかったら
#define _AQUARIUM_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

//********************
// 水槽クラス
//********************
class CAquarium :public CObjectX
{
public:
	CAquarium(int nPriority = 4);	// コンストラクタ
	~CAquarium();					// デストラクタ
	static CAquarium* Create(D3DXVECTOR3 pos);// インスタンス生成
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
	int m_nIdxXFile;	// 使うモデルの番号
};
#endif