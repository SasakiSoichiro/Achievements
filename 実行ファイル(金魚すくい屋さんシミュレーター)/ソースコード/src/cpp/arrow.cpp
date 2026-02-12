//*****************************************
//
// 矢印処理 [arrow.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "arrow.h"
#include "manager.h"

//*****************
// コンストラクタ
//*****************
CArrow::CArrow(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_fHeightOffset = 0.0f;
	m_nLife = 0;
}

//*****************
// デストラクタ
//*****************
CArrow::~CArrow()
{
}

//******************
// インスタンス生成
//*******************
CArrow* CArrow::Create(D3DXVECTOR3 pos)
{
	CArrow* pStall;

	// インスタンス生成
	pStall = new CArrow;
	// モデルの設定
	pStall->SetXModel("data\\MODEL\\arrowTutorial.x");
	// 初期化
	pStall->Init();
	pStall->SetPos(pos);
	pStall->m_fHeightOffset = pos.y;
	return pStall;
}

//*****************
// 初期化処理
//*****************
HRESULT CArrow::Init(void)
{
	CObjectX::Init();
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CArrow::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CArrow::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	pos.y = m_fHeightOffset + sinf((float)m_nLife * fRatioSin) * fCross;
	rot.y += fRotYDelta;

	SetPos(pos);
	SetRot(rot);

	m_nLife++;
	if (m_nLife > maxLife)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void CArrow::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CArrow::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CArrow::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CArrow::SetXModel(const char* fileName)
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
D3DXVECTOR3 CArrow::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CArrow::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CArrow::GetXModel(void)
{
	return CObjectX::GetXModel();
}