//**********************************
//
// タイトルUI処理 [titleUI.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "titleUI.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"
#include "texture.h"

// 定数
namespace TitleUI
{
	D3DXVECTOR2 SIZE = { 800.0f,650.0f };
}

//*****************
// コンストラクタ
//*****************
CTitleUI::CTitleUI(int nPriority) :CObjectShader2D(nPriority)
{
	m_nIdxShader = 0;	// インデックス
	m_pTextureDissolve = nullptr;
	m_pTextureLogo = nullptr;
	m_fTime = 0.0f;
	m_bDeath = false;
}

//*****************
// デストラクタ
//*****************
CTitleUI::~CTitleUI()
{
}

//******************
// インスタンス生成
//******************
CTitleUI* CTitleUI::Create(D3DXVECTOR3 pos)
{
	CTitleUI* pTitleUI;
	CShaderManager* pShader = CManager::GetShaderManager();

	pTitleUI = new CTitleUI;// インスタンス生成
	// シェーダ
	pTitleUI->BindShader(pShader->GetAddres(pShader->Register("data\\SHADER\\dissolve.hlsl")));
	// 初期化
	pTitleUI->Init();
	pTitleUI->SetPos(pos);

	return pTitleUI;
}

//*****************
// 初期化処理
//*****************
HRESULT CTitleUI::Init(void)
{
	// 2Dの初期化
	CObjectShader2D::Init();
	// UVの設定
	CObjectShader2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObjectShader2D::SetSize(TitleUI::SIZE);
	CObjectShader2D::SetRot(VECTOR3_NULL);
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ
	m_pTextureDissolve = pTexture->GetAddres(pTexture->Register("data\\TEXTURE\\dissolve.png"));
	m_pTextureLogo = pTexture->GetAddres(pTexture->Register("data\\TEXTURE\\goldFishTitle02.png"));
	m_fTime = 1.0f;
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CTitleUI::Uninit(void)
{
	// 終了処理
	CObjectShader2D::Uninit();
}

//*****************
// 更新処理
//*****************
void CTitleUI::Update(void)
{
	if (m_bDeath)
	{
		DeathUpdate();
	}
	else
	{
		ScreenUpdate();
	}
}

//*****************
// 描画処理
//*****************
void CTitleUI::Draw(void)
{
	CObjectShader2D::Draw();
}

//*****************
// パラメータ設定
//*****************
void CTitleUI::SetShaderParam(void)
{
	LPD3DXEFFECT pShader = CObjectShader2D::GetShader();
	pShader->SetTechnique("Basic");
	pShader->SetFloat("g_fTime", m_fTime);
	pShader->SetTexture("g_logoMap", m_pTextureLogo);
	pShader->SetTexture("g_dissolveMap", m_pTextureDissolve);
}

void CTitleUI::DeathUpdate(void)
{
	m_fTime += fDeltaTime;

	if (m_fTime >= 1.0f)
	{// 透明になったら終了
		Uninit();
		return;
	}

}
void CTitleUI::ScreenUpdate(void)
{
	if (m_fTime > 0.0f)
	{
		m_fTime -= fDeltaTime;
	}
}

//*****************
// 位置設定
//*****************
void CTitleUI::SetPos(D3DXVECTOR3 pos)
{
	// 位置設定
	CObjectShader2D::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CTitleUI::SetRot(D3DXVECTOR3 rot)
{
	// 位置設定
	CObjectShader2D::SetRot(rot);
}