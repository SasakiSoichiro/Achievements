//*****************************************
//
// 水槽処理 [aquarium.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "aquarium.h"
#include "renderer.h"
#include "manager.h"

//*****************
// コンストラクタ
//*****************
CAquarium::CAquarium(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
}

//*****************
// デストラクタ
//*****************
CAquarium::~CAquarium()
{
}

//******************
// インスタンス生成
//*******************
CAquarium* CAquarium::Create(D3DXVECTOR3 pos)
{
	CAquarium* pBlock;

	int nNum = CObject::GetNumAll();

	// インスタンス生成
	pBlock = new CAquarium;
	// モデルの設定
	pBlock->SetXModel("data\\MODEL\\aquarium.x");
	// 初期化
	pBlock->Init();
	pBlock->SetPos(pos);

	return pBlock;
}

//*****************
// 初期化処理
//*****************
HRESULT CAquarium::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CAquarium::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CAquarium::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CAquarium::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CAquarium::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CAquarium::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CAquarium::SetXModel(const char* fileName)
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
D3DXVECTOR3 CAquarium::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CAquarium::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CAquarium::GetXModel(void)
{
	return CObjectX::GetXModel();
}