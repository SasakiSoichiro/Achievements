//**************************************
//
// 怒るエフェクト処理 [angryEffect.cpp]
// Author:Sasaki Soichiro
//
//**************************************

//*******************
// インクルード
//*******************
#include "angryEffect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//********************
// コンストラクタ
//********************
CAngryEffect::CAngryEffect(int nPriority) :CObjectBillboard(nPriority)
{
	m_nIdxTexture = -1;
	m_nLife = 0;
}

//********************
// デストラクタ
//********************
CAngryEffect::~CAngryEffect()
{
}

//********************
// インスタンス生成 
//********************
CAngryEffect* CAngryEffect::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	CAngryEffect* pSpark;
	CTexture* pTexture = CManager::GetCTexture();

	int nNum = GetNumAll();
	if (nNum < MAX_OBJ - 1)
	{
		pSpark = new CAngryEffect;
		pSpark->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\angry.png");
		pSpark->Init();
		pSpark->SetPos(pos);
		pSpark->SetSize(size);

		return pSpark;
	}
	return NULL;
}

//********************
// 初期化処理
//********************
HRESULT CAngryEffect::Init(void)
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

//********************
// 終了処理
//********************
void CAngryEffect::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//********************
// 更新処理
//********************
void CAngryEffect::Update(void)
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

//********************
// 描画処理
//********************
void CAngryEffect::Draw(void)
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

//********************
// 位置を設定
//********************
void CAngryEffect::SetPos(D3DXVECTOR3 pos)
{
	CObjectBillboard::SetPos(pos);
}

//********************
// 色を設定
//********************
void CAngryEffect::SetCol(D3DXCOLOR col)
{
	CObjectBillboard::SetCol(col);
}

//********************
// 大きさを設定
//********************
void CAngryEffect::SetSize(D3DXVECTOR2 size)
{
	CObjectBillboard::SetSize(size);
}

//********************
// 位置情報
//********************
D3DXVECTOR3 CAngryEffect::GetPos(void)
{
	return CObjectBillboard::GetPos();
}

//********************
// 大きさ情報
//********************
D3DXVECTOR2 CAngryEffect::GetSize(void)
{
	return CObjectBillboard::GetSize();
}