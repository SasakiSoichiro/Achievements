//**************************************
//
// アイテムが入っている箱の処理 [itemBox.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _ITEMBOX_H_ // このマクロ定義がされていなかったら
#define _ITEMBOX_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"
#include "item.h"

class CBlock;

//********************
// アイテム箱クラス
//********************
class CItemBox :public CObjectX
{
public:
	CItemBox(int nPriority = 4);	// コンストラクタ
	~CItemBox();					// デストラクタ
	static CItemBox* Create(D3DXVECTOR3 pos, CItem::ITEM type);
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool Area(D3DXVECTOR3 pos);				// 範囲
	CItem* Pick(void);						// 拾う
	CItem* Put(CItem* pItem);				// 置く
	void SetPos(D3DXVECTOR3 pos);			// 位置
	void SetRot(D3DXVECTOR3 rot);			// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得

private:
	// 定数
	static constexpr float distance = 5.0f;
	static constexpr float radius = 20.0f;
	static constexpr float minHeight = -10.0f;
	static constexpr float maxHeight = -24.0f;
	// 座標
	D3DXVECTOR3 m_areaPos;	// 範囲の中心
	// enum
	CItem::ITEM m_type;
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号
};

#endif