//***************************************
//
// 通行人管理処理 [passerbyManager.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "passerbyManager.h"
#include "passerby.h"

//*****************
// コンストラクタ
//*****************
CPasserbyManager::CPasserbyManager()
{
	m_nCnt = 0;
}

//*****************
// デストラクタ
//*****************
CPasserbyManager::~CPasserbyManager()
{

}

//*****************
// 初期化処理
//*****************
HRESULT CPasserbyManager::Init(void)
{
	Create();
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CPasserbyManager::Uninit(void)
{

}

//*****************
// 更新処理
//*****************
void CPasserbyManager::Update(void)
{
	m_nCnt++;
	if (m_nCnt > nCoolDown)
	{
		Create();

		m_nCnt = 0;
	}
}

//*****************
// 生成処理
//*****************
void CPasserbyManager::Create(void)
{
	// 走る確率
	int nDash = rand() % nMaxRand;
	bool bDash = false;
	if (nDash < nDashRand)
	{
		bDash = true;
	}

	// 右か左か
	int nRight = rand() % nRandNum;
	bool bRight = nRight == 0;

	if (bRight)
	{
		CPasserby::Create(D3DXVECTOR3(backPosX, 0.0f, rightPosZ), bRight, bDash);
	}
	else
	{
		CPasserby::Create(D3DXVECTOR3(flontPosX, 0.0f, leftPosZ), bRight, bDash);
	}
}