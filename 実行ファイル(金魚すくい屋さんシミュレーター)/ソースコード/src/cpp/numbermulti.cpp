//************************************************
//
// 数字のマルチテクスチャ処理 [numberMulti.cpp]
// Author:Sasaki Soichiro
//
//************************************************

//*******************
// インクルード
//*******************
#include "numbermulti.h"
#include "manager.h"
#include "texture.h"
#include "mathUtil.h"

// 定数
namespace NumberMulti
{
	const D3DXVECTOR2 SIZE = { 120.0f, 150.0f };
}

//*****************
// コンストラクタ
//*****************
CNumberMulti::CNumberMulti(int nPriority) :CObject2DMulti(nPriority)
{
	for (int nCnt = 0; nCnt < TEXTUREIDX_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt]=0;	// インデックス
	}
	m_nNum=0;
}

//*****************
// デストラクタ
//*****************
CNumberMulti::~CNumberMulti()
{
}

//*****************
// 初期化処理
//*****************
HRESULT CNumberMulti::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ取得
	CObject2DMulti::Init();							// 2Dの初期化
	// テクスチャを割り当てる
	CObject2DMulti::BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_NUMBER]), pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_COLOR]));

	// UVの設定
	CObject2DMulti::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// テクスチャの緑部分が移るように設定
	CObject2DMulti::SetTex2(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(fTexHalf, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(fTexHalf, 1.0f));

	// 大きさの設定
	CObject2DMulti::SetSize(NumberMulti::SIZE);
	SetNumber(m_nNum);
	SetGauge(nInitGauge);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CNumberMulti::Uninit(void)
{
	// 終了処理
	CObject2DMulti::Uninit();
}

//*****************
// 更新処理
//*****************
void CNumberMulti::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CNumberMulti::Draw(void)
{
	// 描画処理
	CObject2DMulti::Draw();
}

//******************
// インスタンス生成
//******************
CNumberMulti* CNumberMulti::Create(D3DXVECTOR3 pos)
{
	CNumberMulti* pNumberMulti;
	CTexture* pTexture = CManager::GetCTexture();	

	pNumberMulti = new CNumberMulti;// インスタンス生成
	// マルチテクスチャ1枚目
	pNumberMulti->m_nIdxTexture[TEXTUREIDX_NUMBER] = pTexture->Register("data\\TEXTURE\\number005.png");
	// マルチテクスチャ2枚目
	pNumberMulti->m_nIdxTexture[TEXTUREIDX_COLOR] = pTexture->Register("data\\TEXTURE\\orange.jpg");

	// 初期化
	pNumberMulti->Init();
	pNumberMulti->SetPos(pos);

	return pNumberMulti;
}

//*****************
// 位置設定
//*****************
void CNumberMulti::SetPos(D3DXVECTOR3 pos)
{
	// 位置設定
	CObject2DMulti::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CNumberMulti::SetRot(D3DXVECTOR3 rot)
{
	// 位置設定
	CObject2DMulti::SetRot(rot);
}
void CNumberMulti::SetCol(D3DXCOLOR col)
{
	CObject2DMulti::SetCol(col);
}
//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CNumberMulti::GetPos(void)
{
	// 位置情報取得
	return CObject2DMulti::GetPos();
}

void CNumberMulti::SetNumber(int nNum)
{
	// UVの設定
	CObject2DMulti::SetTex(D3DXVECTOR2(nNum * fTexOffset, 0.0f),
		D3DXVECTOR2(fTexOffset + nNum * fTexOffset, 0.0f),
		D3DXVECTOR2(nNum * fTexOffset, 1.0f),
		D3DXVECTOR2(fTexOffset + nNum * fTexOffset, 1.0f));
}
void CNumberMulti::SetGauge(int nGauge)
{
	float p = (float)nGauge / fMaxRatio;		// 1から100の値に直す

	if (nGauge >= 0.0f && nGauge <= fMaxRatio)
	{
		float offsetY = MathUtil::Half(p);
		// UV座標(割合に応じて色が変わるように計算)
		CObject2DMulti::SetTex2(D3DXVECTOR2(0.0f, offsetY),
			D3DXVECTOR2(1.0f, offsetY),
			D3DXVECTOR2(0.0f, fTexHalf + offsetY),
			D3DXVECTOR2(1.0f, fTexHalf + offsetY));
	}
}