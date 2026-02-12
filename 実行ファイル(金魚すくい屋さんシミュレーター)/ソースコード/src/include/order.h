//**************************************
//
// 注文表示処理 [order.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _ORDER_H_ // このマクロ定義がされていなかったら
#define _ORDER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectBillboard.h"
#include "item.h"

//************************
// 注文クラス
//************************
class COrder :public CObjectBillboard
{
public:
	typedef enum
	{
		TEXTUREIDX_BOARD = 0,
		TEXTUREIDX_ITEM0,
		TEXTUREIDX_ITEM1,
	}TEXTUREIDX;
	typedef enum
	{
		ITEM0 = 0,
		ITEM1,
		ITEM_MAX
	}ITEM;
	COrder(int nPriority = 4);	// コンストラクタ
	~COrder();					// デストラクタ
	static COrder* Create(D3DXVECTOR3 pos);	// インスタンス生成
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Screen(void);						// 表示
	// セッター
	void SetPos(D3DXVECTOR3 pos);			// 位置
	void SetRot(D3DXVECTOR3 rot);			// 向き
	void SetItem(CItem::ITEM type0, CItem::ITEM type1);// 注文する内容
	void SetScreen(bool bScreen) { m_bScreen = bScreen; }
	void Death(void) { m_bDeath = true; }
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報の取得
	D3DXVECTOR3 GetRot(void);		// 向き情報の取得
	int GetItemTextureIdx(CItem::ITEM type);	// アイテムのテクスチャ番号を返す
private:
	// 定数
	static constexpr int nMaxTexture = 3;
	static constexpr int nMaxOrder = 2;
	static constexpr float fOrderSizeX = 18.0f;
	static constexpr float fOrderSizeY = 12.0f;
	static constexpr float fItemSize = 8.0f;
	static constexpr float fItemOffset = 8.0f;
	// ポインタ
	CObjectBillboard* m_pItemBillboard[nMaxOrder];
	// フラグ
	bool m_bScreen;	// 表示するかしないか
	bool m_bDeath;
	// 数値
	int m_nIdxTexture[nMaxTexture];		// 使うテクスチャの番号
};
#endif