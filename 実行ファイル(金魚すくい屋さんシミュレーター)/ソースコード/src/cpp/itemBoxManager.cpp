//*******************************************
//
// アイテム箱管理処理 [itemBoxManager.cpp]
// Author:Sasaki Soichiro
//
//*******************************************

//*******************
// インクルード
//*******************
#include "itemBoxManager.h"
#include "itemBox.h"
#include "player.h"
#include "game.h"

//*****************
// コンストラクタ
//*****************
CItemBoxManager::CItemBoxManager()
{

}

//*****************
// デストラクタ
//*****************
CItemBoxManager::~CItemBoxManager()
{

}

//*****************
// 初期化処理
//*****************
HRESULT CItemBoxManager::Init(void)
{
	m_ItemLineLeftPos = D3DXVECTOR3(ItemLineX, ItemLineY, ItemLineLeftZ);
	m_ItemLineRightPos = D3DXVECTOR3(ItemLineX, ItemLineY, ItemLineRightZ);

	float fZ[ItemBoxNum];
	// きれいに横に並べるためのfor文
	for (int nCnt = 0; nCnt < ItemBoxNum; nCnt++)
	{
		fZ[nCnt] = ItemLineLeftZ + ((ItemLineRightZ - ItemLineLeftZ)/(ItemBoxNum+1)) * (nCnt+1);
	}
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[0]), CItem::ITEM_SOFTPOI));
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[1]), CItem::ITEM_MIDIUMPOI));
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[2]), CItem::ITEM_HARDPOI));
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[3]), CItem::ITEM_BOWL));
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[4]), CItem::ITEM_BAG_S));
	m_ItemBox.push_back(CItemBox::Create(D3DXVECTOR3(ItemLineX, 0.0f, fZ[5]), CItem::ITEM_BAG_L));

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CItemBoxManager::Uninit(void)
{

}

//*****************
// 更新処理
//*****************
void CItemBoxManager::Update(void)
{

}

//***********************************
// 拾えるアイテムを分かりやすく表示
//***********************************
void CItemBoxManager::PickUp(D3DXVECTOR3 pos)
{
	for (const auto& itemBox : m_ItemBox)
	{
		itemBox->Area(pos);
	}
}

//*****************
// 拾う
//*****************
CItem* CItemBoxManager::Pick(D3DXVECTOR3 pos)
{
	for (const auto& itemBox : m_ItemBox)
	{
		if (itemBox->Area(pos))
		{// エリアに入ってたら拾える
			return itemBox->Pick();
		}
	}

	return nullptr;
}

//*****************
// 置く
//*****************
CItem* CItemBoxManager::Put(D3DXVECTOR3 pos,CItem*pItem)
{
	for (const auto& itemBox : m_ItemBox)
	{
		if (itemBox->Area(pos))
		{// エリアに入ってたら拾える
			return itemBox->Put(pItem);
		}
	}

	return nullptr;

}