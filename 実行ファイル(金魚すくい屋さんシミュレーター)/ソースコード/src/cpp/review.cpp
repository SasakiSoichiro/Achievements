//**********************************
//
// レビュー処理 [review.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "review.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "mathUtil.h"

//*****************
// 静的メンバ変数
//*****************
float CReview::m_fScore = 0.0f;

//*****************
// コンストラクタ
//*****************
CReview::CReview(int nPriority) :CObject3DMulti(nPriority)
{
	// テクスチャ枚数分
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_nIdxTexture[nCnt] = 0;	// インデックス
	}

	m_size = D3DXVECTOR2(0.0f, 0.0f);	// 大きさ
}

//*****************
// デストラクタ
//*****************
CReview::~CReview()
{
}

//******************
// インスタンス生成
//******************
CReview* CReview::Create(D3DXVECTOR3 pos)
{
	CReview* pReview;								// ライフのポインタ
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ

	pReview = new CReview;// インスタンス生成
	// マルチテクスチャ1枚目(電池の画像)
	pReview->m_nIdxTexture[TEXTUREIDX_STAR] = pTexture->Register("data\\TEXTURE\\review.png");
	// マルチテクスチャ2枚目(緑・白の二色の画像)
	pReview->m_nIdxTexture[TEXTUREIDX_COLOR] = pTexture->Register("data\\TEXTURE\\yellow001.jpg");

	// 初期化
	pReview->Init();
	pReview->SetPos(pos);

	// 情報を渡す
	return pReview;
}

//*****************
// 初期化処理
//*****************
HRESULT CReview::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ取得
	CObject3DMulti::Init();						// 2Dの初期化
	// テクスチャを割り当てる
	CObject3DMulti::BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_STAR]), pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_COLOR]));

	// UVの設定
	CObject3DMulti::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));

	// テクスチャの緑部分が移るように設定
	CObject3DMulti::SetTex2(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(fTexHalf, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(fTexHalf, 1.0f));

	// 大きさの設定
	CObject3DMulti::SetSize(D3DXVECTOR2(fSizeX, fSizeY));
	// 向き
	CObject3DMulti::SetRot(D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f));

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CReview::Uninit(void)
{
	// 終了処理
	CObject3DMulti::Uninit();
}

//*****************
// 更新処理
//*****************
void CReview::Update(void)
{
	float fLife = fMaxReview - m_fScore;	// 計算用
	float p = fLife / fMaxReview;			// 1から100の値に直す

	if (fLife >= 0.0f && fLife <= fMaxReview)
	{
		// UV座標(割合に応じて色が変わるように計算)
		CObject3DMulti::SetTex2(D3DXVECTOR2(p * fTexHalf, 0.0f),
			D3DXVECTOR2(fTexHalf + p * fTexHalf, 0.0f),
			D3DXVECTOR2(p * fTexHalf, 1.0f),
			D3DXVECTOR2(fTexHalf + p * fTexHalf, 1.0f));
	}

}

//*****************
// 描画処理
//*****************
void CReview::Draw(void)
{
	// 描画処理
	CObject3DMulti::Draw();
}

//*****************
// 位置設定
//*****************
void CReview::SetPos(D3DXVECTOR3 pos)
{
	// 位置設定
	CObject3DMulti::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CReview::SetRot(D3DXVECTOR3 rot)
{
	// 位置設定
	CObject3DMulti::SetRot(rot);
}

//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CReview::GetPos(void)
{
	// 位置情報取得
	return CObject3DMulti::GetPos();
}

//*****************
// 書き込み
//*****************
void CReview::Write(float fScore)
{
	if (fScore > fMaxReview)
	{
		fScore = fMaxReview;
	}

	if (m_fScore <= 0.0f)
	{
		m_fScore = fScore;
	}
	else
	{
		m_fScore = MathUtil::Half(m_fScore + fScore);
	}
}