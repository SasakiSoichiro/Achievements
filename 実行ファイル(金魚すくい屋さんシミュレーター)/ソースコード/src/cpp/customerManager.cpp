//**********************************
//
// 客管理処理 [customerManager.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "customerManager.h"
#include "customer.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "manager.h"
#include "orderDesk.h"
#include "baggingDesk.h"
#include "item.h"
#include "money.h"
#include "fullCustomerUI.h"

namespace Position
{
	const D3DXVECTOR3 GRID = { 70.0f,0.0f,120.0f };
	const D3DXVECTOR3 PLAYGRID1 = { 0.0f,0.0f,90.0f };
	const D3DXVECTOR3 PLAYGRID2 = { 50.0f,0.0f,90.0f };
	const D3DXVECTOR3 PLAYGRID3 = { 70.0f,0.0f,30.0f };
	const D3DXVECTOR3 PLAYGRID4 = { 70.0f,0.0f,-30.0f };
	const D3DXVECTOR3 PLAYGRID5 = { 50.0f,0.0f,-90.0f };
	const D3DXVECTOR3 PLAYGRID6 = { 0.0f,0.0f,-90.0f };
	const D3DXVECTOR3 TURNGRID = { 70.0f,0.0f,-120.0f };
	const D3DXVECTOR3 CUSTOMER = { 150.0f, 0.0f, 150.0f };
}

//*****************
// コンストラクタ
//*****************
CCustomerManager::CCustomerManager()
{
	for (int nCntX = 0; nCntX < nMaxGridX; nCntX++)
	{
		for (int nCntZ = 0; nCntZ < nMaxGridZ; nCntZ++)
		{
			m_playAreaGrid[nCntX][nCntZ].pos = VECTOR3_NULL;
			m_playAreaGrid[nCntX][nCntZ].type = PLAYAREA_TYPE_NONE;
		}
	}
	m_nCntOrder = 0;
	m_nCntPlay = 0;
	m_nCntBagging = 0;
	m_nCntPlayOrder = 0;
	m_bDeath = false;
}

//*****************
// デストラクタ
//*****************
CCustomerManager::~CCustomerManager()
{

}

//*****************
// 初期化処理
//*****************
HRESULT CCustomerManager::Init(void)
{
	CCustomer::ResetNumAll();

	for (int nCntX = 0; nCntX < nMaxGridX; nCntX++)
	{
		for (int nCntZ = 0; nCntZ < nMaxGridZ; nCntZ++)
		{
			m_playAreaGrid[nCntX][nCntZ].pos = D3DXVECTOR3(Position::GRID.x + fGridSize * nCntX, Position::GRID.y, Position::GRID.z - fGridSize * nCntZ);
		}
	}
	for (int nCnt = nMinPlayAreaGrid; nCnt < nMaxPlayAreaGrid; nCnt++)
	{
		m_playAreaGrid[0][nCnt].type = PLAYAREA_TYPE_NONE;
	}

	m_playAreaGrid[0][1].pos = Position::PLAYGRID1;
	m_playAreaGrid[0][2].pos = Position::PLAYGRID2;
	m_playAreaGrid[0][3].pos = Position::PLAYGRID3;
	m_playAreaGrid[0][4].pos = Position::PLAYGRID4;
	m_playAreaGrid[0][5].pos = Position::PLAYGRID5;
	m_playAreaGrid[0][6].pos = Position::PLAYGRID6;

	m_playAreaGrid[1][6].pos = D3DXVECTOR3(Position::TURNGRID.x + fGridSize, Position::TURNGRID.y, Position::TURNGRID.z);

	for (int nCnt = 0; nCnt < nMaxCustomer; nCnt++)
	{
		CCustomer* pCustomer = CCustomer::Create(Position::CUSTOMER);
		m_Customer.push_back(pCustomer);
		m_Order.push_back(pCustomer);
	}


	return S_OK;
}

//*****************
// 終了処理
//*****************
void CCustomerManager::Uninit(void)
{

}

//*****************
// 更新処理
//*****************
void CCustomerManager::Update(void)
{
	if (CCustomer::GetNumAll() < nMaxCustomer)
	{
		CCustomer* pCustomer = CCustomer::Create(Position::CUSTOMER);
		m_Customer.push_back(pCustomer);
		m_Order.push_back(pCustomer);
	}
	OrderUpdate();
	PlayUpdate();
	PlayOrderUpdate();
	BaggingUpdate();
	if (m_bDeath)
	{
		Death();
	}
}

//************************
// 注文時の更新
//************************
void CCustomerManager::OrderUpdate(void)
{
	if ((int)m_Order.size() <= 0)return;

	COrderDesk* pOrderDesk = CGame::GetCOrderDesk();

	m_Order[0]->OrderScreen(true);
	for (int nCnt = 0; nCnt < (int)m_Order.size(); nCnt++)
	{
		D3DXVECTOR3 orderDeskPos = pOrderDesk->GetPos();
		orderDeskPos.x += fDistanceLine * (1 + nCnt);
		m_Order[nCnt]->SetPosDest(orderDeskPos);
		m_Order[nCnt]->SetRotDest(D3DXVECTOR3(0.0f, PI_HALF, 0.0f));
	}

	if (m_Order[0]->GetReceive())
	{// 客の注文が全部出せたら状態変更
		m_Order[0]->OrderScreen(false);
		m_Order[0]->SetState(CCustomer::STATE_PLAY);
		m_Play.push_back(m_Order[0]);
		m_Order.erase(m_Order.begin());
		CMoney::Add(400);
	}
}

//************************
// 遊んでいるときの更新
//************************
void CCustomerManager::PlayUpdate(void)
{
	for (auto play = m_Play.begin(); play != m_Play.end(); )
	{
		CCustomer* Customer = *play;
		if (Customer->GetState() == CCustomer::STATE_BAGGING)
		{
			m_Bagging.push_back(Customer);
			play = m_Play.erase(play);
		}
		else if (Customer->GetState() == CCustomer::STATE_PLAYORDER)
		{
			m_PlayOrder.push_back(Customer);
			play = m_Play.erase(play);
		}
		else
		{
			++play;
		}
	}
}

//*****************************
// 追加注文しているときの更新
//*****************************
void CCustomerManager::PlayOrderUpdate(void)
{
	if ((int)m_PlayOrder.size() <= 0)return;

	COrderDesk* pOrderDesk = CGame::GetCOrderDesk();

	m_PlayOrder[0]->OrderScreen(true);

	if (m_PlayOrder[0]->GetPlayReceive())
	{// 客の注文が全部出せたら状態変更
		m_PlayOrder[0]->OrderScreen(false);
		m_PlayOrder[0]->SetState(CCustomer::STATE_PLAY);
		m_Play.push_back(m_PlayOrder[0]);
		m_PlayOrder.erase(m_PlayOrder.begin());
		CMoney::Add(nMoneyAdd);
	}
}

//***************************
// 袋詰めしているときの更新
//***************************
void CCustomerManager::BaggingUpdate(void)
{
	if ((int)m_Bagging.size() <= 0)return;
	m_Bagging[0]->OrderScreen(true);
	CBaggingDesk* pBaggingDesk = CGame::GetCBaggingDesk();
	for (int nCnt = 0; nCnt < (int)m_Bagging.size(); nCnt++)
	{
		D3DXVECTOR3 baggingDeskPos = pBaggingDesk->GetPos();
		baggingDeskPos.x += fDistanceLine * (1 + nCnt);
		m_Bagging[nCnt]->SetBaggingPos(baggingDeskPos);
	}

	if (m_Bagging[0]->GetBagging())
	{// 客の注文が全部出せたら状態変更
		m_Bagging[0]->OrderScreen(false);
		m_Bagging[0]->SetState(CCustomer::STATE_GOBACK);
		m_Bagging.erase(m_Bagging.begin());
		CMoney::Add(100);
	}
}

//*****************
// 注文を受け取る
//*****************
void CCustomerManager::Order(CItem* pItem)
{
	if ((int)m_Order.size() <= 0)
	{	// 並んでいる人が0人
		pItem->Uninit();
		pItem = nullptr;
		return;
	}

	if (GetIsFill())
	{// 遊ぶ場所がすべて埋まってる
		COrderDesk* pOrderDesk = CGame::GetCOrderDesk();
		D3DXVECTOR3 orderDeskPos = pOrderDesk->GetPos();
		orderDeskPos.y += fDistanceLine;
		CFullCustomerUI::Create(orderDeskPos, D3DXVECTOR2(fFullCustoUISizeX, fFullCustoUISizeZ));
		pItem->Uninit();
		pItem = nullptr;
		return;
	}

	m_Order[0]->Receive(pItem);
}

//*****************
// 注文を受け取る
//*****************
void CCustomerManager::Play(CItem* pItem)
{
	if ((int)m_PlayOrder.size() <= 0)
	{
		pItem->Uninit();
		pItem = nullptr;
		return;
	}

	m_PlayOrder[0]->Receive(pItem);
}

//*****************
// 袋を受け取る
//*****************
void CCustomerManager::Bagging(CItem* pItem)
{
	if ((int)m_Bagging.size() <= 0)
	{
		pItem->Uninit();
		pItem = nullptr;
		return;
	}

	m_Bagging[0]->Receive(pItem);
}

//*********************************
// そのグリッドのタイプを設定する
//*********************************
void CCustomerManager::SetPlayAreaGrid(int nCntX, int nCntZ, PLAYAREA_TYPE type)
{
	if (nCntX >= nMaxGridX || nCntZ >= nMaxGridZ)
	{
		return;
	}

	m_playAreaGrid[nCntX][nCntZ].type = type;
}

//***************************
// そのグリッドのタイプを返す
//***************************
CCustomerManager::PLAYAREA CCustomerManager::GetPlayAreaGrid(int nCntX, int nCntZ)
{
	PLAYAREA playArea;
	playArea.pos = VECTOR3_NULL;
	playArea.type = PLAYAREA_TYPE_NONE;
	if (nCntX >= nMaxGridX || nCntZ >= nMaxGridZ)
	{
		return playArea;
	}

	return m_playAreaGrid[nCntX][nCntZ];
}

//***************************
// 全て埋まっているか
//***************************
bool CCustomerManager::GetIsFill(void)
{
	int nCntFill = 0;
	for (int nCnt = nMinPlayAreaGrid; nCnt < nMaxPlayAreaGrid; nCnt++)
	{
		if (m_playAreaGrid[0][nCnt].type == PLAYAREA_TYPE_FILL)
		{
			nCntFill++;
		}
	}
	if (nCntFill >= nMaxPlayCustomer)
	{
		return true;
	}

	return false;
}

//***************************
// 更新時の終了処理
//***************************
void CCustomerManager::Death(void)
{
	// 注文に並んでいる人
	for (auto order = m_Order.begin(); order != m_Order.end(); )
	{
		CCustomer* Customer = *order;
		order = m_Order.erase(order);
	}
	// 遊んでいる人
	for (auto play = m_Play.begin(); play != m_Play.end(); )
	{
		CCustomer* Customer = *play;
		play = m_Play.erase(play);
	}
	// 追加注文している人
	for (auto playOrder = m_PlayOrder.begin(); playOrder != m_PlayOrder.end(); )
	{
		CCustomer* Customer = *playOrder;
		playOrder = m_PlayOrder.erase(playOrder);
	}
	// 袋詰めに並んでいる人
	for (auto bagging = m_Bagging.begin(); bagging != m_Bagging.end(); )
	{
		CCustomer* Customer = *bagging;
		bagging = m_Bagging.erase(bagging);
	}
	// 客全員
	for (auto cust = m_Customer.begin(); cust != m_Customer.end(); )
	{
		CCustomer* Customer = *cust;
		Customer->Death();
		cust = m_Customer.erase(cust);
	}
}