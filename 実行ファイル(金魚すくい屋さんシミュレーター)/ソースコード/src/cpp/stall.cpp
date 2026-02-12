//*****************************************
//
// 屋台処理 [stall.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "stall.h"
#include "manager.h"

//*****************
// コンストラクタ
//*****************
CStall::CStall(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
}

//*****************
// デストラクタ
//*****************
CStall::~CStall()
{
}

//******************
// インスタンス生成
//*******************
CStall* CStall::Create(D3DXVECTOR3 pos, float rotY)
{
	CStall* pStall;

	// インスタンス生成
	pStall = new CStall;
	// モデルの設定
	pStall->SetXModel("data\\MODEL\\stall.x");
	// 初期化
	pStall->SetPos(pos);
	pStall->SetRot(D3DXVECTOR3(0.0f, rotY, 0.0f));
	pStall->Init();

	return pStall;
}

//*****************
// 初期化処理
//*****************
HRESULT CStall::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CStall::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CStall::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CStall::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CStall::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CStall::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CStall::SetXModel(const char* fileName)
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
D3DXVECTOR3 CStall::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CStall::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CStall::GetXModel(void)
{
	return CObjectX::GetXModel();
}