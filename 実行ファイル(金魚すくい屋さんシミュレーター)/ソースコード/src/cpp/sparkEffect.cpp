//**************************************
//
// 散るエフェクト処理 [sparkEffect.cpp]
// Author:Sasaki Soichiro
//
//**************************************

//*******************
// インクルード
//*******************
#include "sparkEffect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "mathUtil.h"

//*******************
// コンストラクタ
//*******************
CSparkEffect::CSparkEffect(int nPriority) :CObjectBillboard(nPriority)
{
	m_offsetPos = VECTOR3_NULL;
	m_vec = VECTOR3_NULL;
	m_nIdxTexture = -1;
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_nType = 0;
}

//*******************
// デストラクタ
//*******************
CSparkEffect::~CSparkEffect()
{
}

//*******************
// インスタンス生成 
//*******************
CSparkEffect* CSparkEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DXCOLOR col,int nType)
{
	CSparkEffect* pSpark;
	CTexture* pTexture = CManager::GetCTexture();

	pSpark = new CSparkEffect;
	pSpark->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\effect000.jpg");
	pSpark->m_nType = nType;
	pSpark->Init();
	pSpark->SetPos(pos);
	pSpark->SetOffsetPos(pos);
	pSpark->SetCol(col);
	pSpark->SetVec(vec);

	return pSpark;
}

//*******************
// 初期化処理
//*******************
HRESULT CSparkEffect::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObjectBillboard::Init();
	// テクスチャを割り当てる
	CObjectBillboard::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	CObjectBillboard::BindTexture(pTexture->GetAddres(m_nIdxTexture));
	CObjectBillboard::SetSize(D3DXVECTOR2(fSizeX, fSizeY));
	m_nLife = maxLife;
	switch (m_nType)
	{
	case TYPE_ZERO:
		m_fSpeed = fSpeed0;
		break;
	case TYPE_ONE:
		m_fSpeed = fSpeed1;
		break;
	case TYPE_TWO:
		m_fSpeed = fSpeed2;
		break;
	}

	return S_OK;
}

//*******************
// 終了処理
//*******************
void CSparkEffect::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//*******************
// 更新処理
//*******************
void CSparkEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXCOLOR col = GetCol();

	pos += m_vec * m_fSpeed;
	col.a = (1.0f / MathUtil::Half(maxLife)) * m_nLife;
	pos.y -= fGravity;

	SetPos(pos);
	SetCol(col);
	m_nLife--;
	m_fSpeed -= fSpeedDec;

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//*******************
// 描画処理
//*******************
void CSparkEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CTexture* pTexture = CManager::GetCTexture();
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture));
	CObjectBillboard::Draw();
	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//*******************
// 位置を設定
//*******************
void CSparkEffect::SetPos(D3DXVECTOR3 pos)
{
	CObjectBillboard::SetPos(pos);
}

//*******************
// 色を設定
//*******************
void CSparkEffect::SetCol(D3DXCOLOR col)
{
	CObjectBillboard::SetCol(col);
}

//*******************
// 大きさを設定
//*******************
void CSparkEffect::SetSize(D3DXVECTOR2 size)
{
	CObjectBillboard::SetSize(size);
}

//*******************
// 位置情報取得
//*******************
D3DXVECTOR3 CSparkEffect::GetPos(void)
{
	return CObjectBillboard::GetPos();
}

//*******************
// 大きさ情報取得
//*******************
D3DXVECTOR2 CSparkEffect::GetSize(void)
{
	return CObjectBillboard::GetSize();
}