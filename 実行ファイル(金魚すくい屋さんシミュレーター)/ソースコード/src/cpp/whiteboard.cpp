//*****************************************
//
// ホワイトボード処理 [whiteboard.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "whiteboard.h"
#include "manager.h"
#include "object3D.h"
#include "money3D.h"
#include "review.h"

// 定数
namespace Whiteboard
{
	const D3DXVECTOR3 REVIEW_POS = { 0.0f,50.0f,-2.0f };
	const D3DXVECTOR3 REVIEWCHAR_POS = { -30.0f,60.0f,-2.0f };
	const D3DXVECTOR3 MONEY_POS = { 15.0f,25.0f,-2.0f };
	const D3DXVECTOR3 MONEYCHAR_POS = { -30.0f,40.0f,-2.0f };
	const D3DXVECTOR2 CHAR_SIZE = { 20.0f, 10.0f };
};

//*****************
// コンストラクタ
//*****************
CWhiteboard::CWhiteboard(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	for (int nCnt = 0; nCnt < nMaxUI; nCnt++)
	{
		m_pUI[nCnt] = nullptr;
	}
	m_pMoney3D = nullptr;
}

//*****************
// デストラクタ
//*****************
CWhiteboard::~CWhiteboard()
{
}

//******************
// インスタンス生成
//*******************
CWhiteboard* CWhiteboard::Create(D3DXVECTOR3 pos)
{
	CWhiteboard* pStall;

	// インスタンス生成
	pStall = new CWhiteboard;
	// モデルの設定
	pStall->SetXModel("data\\MODEL\\whiteboard.x");
	// 初期化
	pStall->SetPos(pos);
	pStall->Init();

	return pStall;
}

//*****************
// 初期化処理
//*****************
HRESULT CWhiteboard::Init(void)
{
	D3DXVECTOR3 pos = GetPos();

	CObjectX::Init();
	m_pUI[TYPE_REVIEW] = CObject3D::Create(pos+ Whiteboard::REVIEWCHAR_POS,
		Whiteboard::CHAR_SIZE,
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),
		"data\\TEXTURE\\reviewUI.png");
	m_pUI[TYPE_REVIEW]->SetRot(D3DXVECTOR3(-PI_HALF, 0.0f, 0.0f));

	m_pUI[TYPE_SCORE] = CObject3D::Create(pos + Whiteboard::MONEYCHAR_POS,
		Whiteboard::CHAR_SIZE,
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),
		"data\\TEXTURE\\scoreUI.png");
	m_pUI[TYPE_SCORE]->SetRot(D3DXVECTOR3(-PI_HALF, 0.0f, 0.0f));

	m_pMoney3D = CMoney3D::Create(pos + Whiteboard::MONEY_POS);

	CReview::Create(pos + Whiteboard::REVIEW_POS);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CWhiteboard::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CWhiteboard::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CWhiteboard::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CWhiteboard::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CWhiteboard::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CWhiteboard::SetXModel(const char* fileName)
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
D3DXVECTOR3 CWhiteboard::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CWhiteboard::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CWhiteboard::GetXModel(void)
{
	return CObjectX::GetXModel();
}