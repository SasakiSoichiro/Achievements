//***************************************
//
// 丸影処理 [shadow.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//**********************
// インクルードファイル
//**********************
#include "shadow.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"

//*******************
// コンストラクタ
//*******************
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	// 値のクリア
	m_nIdxTexture = -1;
	m_bScreen = true;
}

//*******************
// デストラクタ
//*******************
CShadow::~CShadow()
{
}

//*******************
// インスタンス生成
//*******************
CShadow* CShadow::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CShadow* pShadow = new CShadow;
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ

	// 初期化
	pShadow->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\shadow000.jpg");
	pShadow->Init();
	pShadow->SetPos(pos);


	// 影のポインタを返す
	return pShadow;
}
//*******************
// 初期化処理
//*******************
HRESULT CShadow::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();

	CObject3D::Init();
	CObject3D::BindTexture(pTexture->GetAddres(m_nIdxTexture));
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
	SetSize(D3DXVECTOR2(fSize, fSize));

	return S_OK;
}

//*******************
// 終了処理
//*******************
void CShadow::Uninit(void)
{
	CObject3D::Uninit();
}

//*******************
// 更新処理
//*******************
void CShadow::Update(void)
{

}

//*******************
// 描画処理
//*******************
void CShadow::Draw(void)
{
	if (m_bScreen)
	{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetCRenderer()->GetDevice();

		// 減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CObject3D::Draw();

		// 減算合成を戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//*******************
// 位置設定
//*******************
void CShadow::SetPos(D3DXVECTOR3 pos)
{
	CObject3D::SetPos(pos);
}
