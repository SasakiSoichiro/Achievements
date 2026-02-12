//**********************************
//
// 満員のUI処理 [fullCustomerUI.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "fullCustomerUI.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//*******************
// コンストラクタ
//*******************
CFullCustomerUI::CFullCustomerUI(int nPriority) :CObjectBillboard(nPriority)
{
	m_nIdxTexture = -1;
	m_nLife = 0;
}

//*******************
// デストラクタ
//*******************
CFullCustomerUI::~CFullCustomerUI()
{
}

//*******************
// インスタンス生成 
//*******************
CFullCustomerUI* CFullCustomerUI::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CFullCustomerUI* pSpark;
	CTexture* pTexture = CManager::GetCTexture();

	pSpark = new CFullCustomerUI;
	pSpark->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\fullCustomer.png");
	pSpark->Init();
	pSpark->SetPos(pos);
	pSpark->SetSize(size);

	return pSpark;
}

//*******************
// 初期化処理
//*******************
HRESULT CFullCustomerUI::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObjectBillboard::Init();
	// テクスチャを割り当てる
	CObjectBillboard::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	CObjectBillboard::BindTexture(pTexture->GetAddres(m_nIdxTexture));
	m_nLife;

	return S_OK;
}

//*******************
// 終了処理
//*******************
void CFullCustomerUI::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//*******************
// 更新処理
//*******************
void CFullCustomerUI::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	pos.y += fSpeed;
	SetPos(pos);

	// 寿命で消す
	m_nLife++;
	if (m_nLife > nMaxLife)
	{
		Uninit();
	}
}

//*******************
// 描画処理
//*******************
void CFullCustomerUI::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CTexture* pTexture = CManager::GetCTexture();
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture));
	CObjectBillboard::Draw();
}

//*******************
// 位置を設定
//*******************
void CFullCustomerUI::SetPos(D3DXVECTOR3 pos)
{
	CObjectBillboard::SetPos(pos);
}

//*******************
// 色を設定
//*******************
void CFullCustomerUI::SetCol(D3DXCOLOR col)
{
	CObjectBillboard::SetCol(col);
}

//*******************
// 大きさを設定
//*******************
void CFullCustomerUI::SetSize(D3DXVECTOR2 size)
{
	CObjectBillboard::SetSize(size);
}

//*******************
// 位置情報取得
//*******************
D3DXVECTOR3 CFullCustomerUI::GetPos(void)
{
	return CObjectBillboard::GetPos();
}

//*******************
// 大きさ情報取得
//*******************
D3DXVECTOR2 CFullCustomerUI::GetSize(void)
{
	return CObjectBillboard::GetSize();
}