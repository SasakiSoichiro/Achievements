//*****************************************
//
// ブロック処理 [block.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "block.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "blockManager.h"

//*****************
// コンストラクタ
//*****************
CBlock::CBlock(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
}

//*****************
// デストラクタ
//*****************
CBlock::~CBlock()
{
}

//******************
// インスタンス生成
//*******************
CBlock* CBlock::Create(D3DXVECTOR3 pos, float rotY,const char* pFileName,int nType)
{
	CBlock* pBlock;
	CBlockManager* pBlockManager=NULL;

	pBlockManager = CGame::GetCBlockManager();

	// インスタンス生成
	pBlock = new CBlock;	
	// モデルの設定
	pBlock->SetXModel(pFileName);
	// 初期化
	pBlock->SetPos(pos);
	pBlock->SetRot(D3DXVECTOR3(0.0f, rotY, 0.0f));
	pBlock->Init();
	// ブロックマネージャーのリストに設定
	pBlockManager->Set(pBlock);

	return pBlock;
}

//*****************
// 初期化処理
//*****************
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CBlock::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CBlock::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CBlock::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CBlock::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CBlock::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CBlock::SetXModel(const char*fileName)
{
	// CXModelのポインタに情報を入れる
	CXModel* pXModel = CManager::GetCXModel();
	// 使うモデルのインデックスを入れる
	m_nIdxXFile = pXModel->Register(fileName);
	// Xファイルを割り当てる
	BindXFile(pXModel->GetAddres(m_nIdxXFile));
}

//******************
// 位置情報の取得
//******************
D3DXVECTOR3 CBlock::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CBlock::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CBlock::GetXModel(void)
{
	return CObjectX::GetXModel();
}