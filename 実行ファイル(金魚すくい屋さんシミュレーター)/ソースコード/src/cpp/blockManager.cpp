//***************************************
//
// ブロック管理処理 [blockManager.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "blockManager.h"

//*******************
// コンストラクタ
//*******************
CBlockManager::CBlockManager()
{
}

//*******************
// デストラクタ
//*******************
CBlockManager::~CBlockManager()
{
}

//*************
// 初期化処理
//*************
HRESULT CBlockManager::Init(void)
{

	return S_OK;
}

//*************
// 終了処理
//*************
void CBlockManager::Uninit(void)
{
}

//*************
// 更新処理
//*************
void CBlockManager::Update(void)
{
}

//**************
// 登録
//**************
void CBlockManager::Set(CBlock*block)
{
	m_Block.push_back(block);
}