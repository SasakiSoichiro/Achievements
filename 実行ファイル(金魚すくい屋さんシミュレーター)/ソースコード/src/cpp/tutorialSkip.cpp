//***********************************************
//
// チュートリアルスキップ処理 [tutorialSkip.cpp]
// Author:Sasaki Soichiro
//
//***********************************************

//*******************
// インクルード
//*******************
#include "tutorialSkip.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

// 定数
namespace TutorialSkip
{
	D3DXVECTOR2 SIZE = { 300.0f, 80.0f };
}

//*****************
// コンストラクタ
//*****************
CTutorialSkip::CTutorialSkip(int nPriority) :CObject2D(nPriority)
{
	m_nIdxTexture = 0;	// インデックス
	m_bDeath = false;
	m_nCnt = 0;
}

//*****************
// デストラクタ
//*****************
CTutorialSkip::~CTutorialSkip()
{
}

//******************
// インスタンス生成
//******************
CTutorialSkip* CTutorialSkip::Create(D3DXVECTOR3 pos)
{
	CTutorialSkip* pTutorialSkip;
	CTexture* pTexture = CManager::GetCTexture();

	pTutorialSkip = new CTutorialSkip;// インスタンス生成
	// テクスチャ
	pTutorialSkip->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\GoldFishTutorialSkip.png");

	// 初期化
	pTutorialSkip->Init();
	pTutorialSkip->SetPos(pos);

	// 情報を渡す
	return pTutorialSkip;
}

//*****************
// 初期化処理
//*****************
HRESULT CTutorialSkip::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture));

	// 2Dの初期化
	CObject2D::Init();

	// UVの設定
	CObject2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObject2D::SetSize(TutorialSkip::SIZE);
	CObject2D::SetRot(VECTOR3_NULL);
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CTutorialSkip::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//*****************
// 更新処理
//*****************
void CTutorialSkip::Update(void)
{
	if (m_bDeath)
	{
		// α値をどんどん減らす
		D3DXCOLOR col = GetCol();
		col.a = 1.0f - 1.0f / nAlphaCnt * m_nCnt;
		SetCol(col);
		m_nCnt++;

		if (m_nCnt > nAlphaCnt)
		{// 透明になったら終了
			Uninit();
			return;
		}
	}
}

//*****************
// 描画処理
//*****************
void CTutorialSkip::Draw(void)
{
	CObject2D::Draw();
}

//*****************
// 位置設定
//*****************
void CTutorialSkip::SetPos(D3DXVECTOR3 pos)
{
	CObject2D::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CTutorialSkip::SetRot(D3DXVECTOR3 rot)
{
	CObject2D::SetRot(rot);
}

//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CTutorialSkip::GetPos(void)
{
	return CObject2D::GetPos();
}