//**************************************
//
// アイテム処理 [item.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _ITEM_H_ // このマクロ定義がされていなかったら
#define _ITEM_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

class CBlock;

//********************
// アイテムクラス
//********************
class CItem :public CObjectX
{
public:
	typedef enum
	{
		ITEM_SOFTPOI=0,
		ITEM_MIDIUMPOI,
		ITEM_HARDPOI,
		ITEM_BOWL,
		ITEM_BAG_S,
		ITEM_BAG_L,
		ITEM_MAX
	}ITEM;
	CItem(int nPriority = 4);	// コンストラクタ
	~CItem();					// デストラクタ
	static CItem* Create(D3DXVECTOR3 pos, ITEM type);
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos);			// 位置
	void SetRot(D3DXVECTOR3 rot);			// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
	ITEM GetType(void) { return m_type; }	// 自分のタイプを返す

private:
	// enum
	ITEM m_type;			// アイテムの種類
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号
};
#endif