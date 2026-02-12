//**********************************
//
// 注文処理 [order.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "order.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "texture.h"

//*****************
// コンストラクタ
//*****************
COrder::COrder(int nPriority) :CObjectBillboard(nPriority)
{
	for (int nCnt = 0; nCnt < nMaxTexture; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;
	}
	for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
	{
		m_pItemBillboard[nCnt] = nullptr;
	}
	m_bScreen = false;
	m_bDeath = false;
}

//*****************
// デストラクタ
//*****************
COrder::~COrder()
{
}

//******************
// インスタンス生成
//******************
COrder* COrder::Create(D3DXVECTOR3 pos)
{
	COrder* pOrder;
	// テクスチャ情報の取得
	CTexture* pTexture = CManager::GetCTexture();

	int nNum = CObject::GetNumAll();// 現在のオブジェクト総数取得
	// インスタンス生成
	pOrder = new COrder;
	// テクスチャ
	pOrder->m_nIdxTexture[TEXTUREIDX_BOARD] = pTexture->Register("data\\TEXTURE\\order.png");
	// 初期化
	pOrder->Init();
	pOrder->SetPos(pos);

	return pOrder;
}

//*****************
// 初期化処理
//*****************
HRESULT COrder::Init(void)
{
	// テクスチャ情報の取得
	CTexture* pTexture = CManager::GetCTexture();

	// 吹き出しのテクスチャ設定
	CObjectBillboard::Init();
	CObjectBillboard::BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_BOARD]));
	// テクスチャ座標の設定
	CObjectBillboard::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObjectBillboard::SetSize(D3DXVECTOR2(fOrderSizeX, fOrderSizeY));

	// 注文内容のテクスチャ設定
	for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
	{// 共通部分の初期化
		m_pItemBillboard[nCnt] = new CObjectBillboard;
		m_pItemBillboard[nCnt]->Init();
		m_pItemBillboard[nCnt]->SetTex(D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f),
			D3DXVECTOR2(1.0f, 1.0f));
		m_pItemBillboard[nCnt]->SetSize(D3DXVECTOR2(fItemSize, fItemSize));
	}

	return S_OK;
}

//*****************
// 終了処理
//*****************
void COrder::Uninit(void)
{
	for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
	{
		m_pItemBillboard[nCnt]->Uninit();
		m_pItemBillboard[nCnt] = nullptr;
	}
	// 終了処理
	CObjectBillboard::Uninit();
}

//*****************
// 更新処理
//*****************
void COrder::Update(void)
{
	for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
	{
		m_pItemBillboard[nCnt]->SetPos(GetPos());
	}

	Screen();
	if (m_bDeath)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void COrder::Draw(void)
{
	CObjectBillboard::Draw();
}

//*****************
// 表示処理
//*****************
void COrder::Screen(void)
{
	if (m_bScreen)
	{
		// 大きさの設定
		CObjectBillboard::SetSize(D3DXVECTOR2(fOrderSizeX, fOrderSizeY));
		for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
		{
			if (m_nIdxTexture[nCnt+1] > -1)
			{
				m_pItemBillboard[nCnt]->SetSize(D3DXVECTOR2(fItemSize, fItemSize));
			}
		}
	}
	else
	{
		// 大きさの設定
		CObjectBillboard::SetSize(VECTOR2_NULL);
		for (int nCnt = 0; nCnt < nMaxOrder; nCnt++)
		{
			m_pItemBillboard[nCnt]->SetSize(VECTOR2_NULL);
		}
	}
}

//*****************
// 位置設定
//*****************
void COrder::SetPos(D3DXVECTOR3 pos)
{
	CObjectBillboard::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void COrder::SetRot(D3DXVECTOR3 rot)
{
	CObjectBillboard::SetRot(rot);
}

//************************
// 注文するアイテムの設定
//************************
void COrder::SetItem(CItem::ITEM type0, CItem::ITEM type1)
{
	// テクスチャ情報の取得
	CTexture* pTexture = CManager::GetCTexture();

	m_nIdxTexture[TEXTUREIDX_ITEM0] = GetItemTextureIdx(type0);
	m_nIdxTexture[TEXTUREIDX_ITEM1] = GetItemTextureIdx(type1);
	if (m_nIdxTexture[TEXTUREIDX_ITEM0] <= -1)return;

	if (m_nIdxTexture[TEXTUREIDX_ITEM1] <= -1)
	{// 注文のアイテムが一個だった場合
		m_pItemBillboard[ITEM0]->BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_ITEM0]));
		m_pItemBillboard[ITEM1]->BindTexture(nullptr);
		m_pItemBillboard[ITEM0]->SetOffset(VECTOR2_NULL);
		m_pItemBillboard[ITEM1]->SetOffset(VECTOR2_NULL);
	}
	else
	{// 注文のアイテムが二個だった場合
		m_pItemBillboard[ITEM0]->BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_ITEM0]));
		m_pItemBillboard[ITEM1]->BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_ITEM1]));
		m_pItemBillboard[ITEM0]->SetOffset(D3DXVECTOR2(-fItemOffset,0.0f));
		m_pItemBillboard[ITEM1]->SetOffset(D3DXVECTOR2(fItemOffset,0.0f));
	}
}

//*********************************
// アイテムテクスチャの番号を返す
//*********************************
int COrder::GetItemTextureIdx(CItem::ITEM type)
{
	// テクスチャ情報の取得
	CTexture* pTexture = CManager::GetCTexture();
	int nIdxTexture = -1;

	switch (type)
	{
	case CItem::ITEM_SOFTPOI:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\poi_red.png");
		break;
	case CItem::ITEM_MIDIUMPOI:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\poi_yellow.png");
		break;
	case CItem::ITEM_HARDPOI:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\poi_white.png");
		break;
	case CItem::ITEM_BOWL:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\bowl.jpg");
		break;
	case CItem::ITEM_BAG_S:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\bag_S.png");
		break;
	case CItem::ITEM_BAG_L:
		nIdxTexture = pTexture->Register("data\\TEXTURE\\bag_L.png");
		break;
	case CItem::ITEM_MAX:
		nIdxTexture = -1;
		break;

	}

	return nIdxTexture;
}
//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 COrder::GetPos(void)
{
	return CObjectBillboard::GetPos();
}