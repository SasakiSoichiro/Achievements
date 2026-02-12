//*******************************************
// 
// アイテム箱マネージャー処理 [itemBoxManager.h]
// Author:Sasaki Soichiro
//
//*******************************************
#ifndef _ITEMBOXMANAGER_H_ // このマクロ定義がされていなかったら
#define _ITEMBOXMANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include <vector>
#include "itemBox.h"

//*****************************
// アイテム箱マネージャークラス
//*****************************
class CItemBoxManager
{
public:
	CItemBoxManager();	// コンストラクタ
	~CItemBoxManager();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void PickUp(D3DXVECTOR3 pos);
	CItem* Pick(D3DXVECTOR3 pos);
	CItem* Put(D3DXVECTOR3 pos, CItem* pItem);
	D3DXVECTOR3 GetItemLineLeftPos(void) {return m_ItemLineLeftPos;}
	D3DXVECTOR3 GetItemLineRightPos(void) { return m_ItemLineRightPos; }
private:
	// 定数
	static constexpr int ItemBoxNum = 6;
	static constexpr float ItemLineX = -130.0f;
	static constexpr float ItemLineLeftZ = 100.0f;
	static constexpr float ItemLineRightZ = -100.0f;
	static constexpr float ItemLineY = 10.0f;
	// ポインタ
	std::vector<CItemBox*> m_ItemBox;// ブロックのポインタ
	// 座標
	D3DXVECTOR3 m_ItemLineLeftPos;
	D3DXVECTOR3 m_ItemLineRightPos;
};
#endif