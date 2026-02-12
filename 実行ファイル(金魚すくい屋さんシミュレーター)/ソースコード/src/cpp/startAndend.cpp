//*****************************************
//
// 開始と終わりのUI処理 [startAndend.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "startAndend.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "timer.h"
#include "game.h"
#include "sound.h"

namespace StartAndEnd
{
	const D3DXVECTOR2 SIZE = { 600.0f, 300.0f };
}

//*****************
// コンストラクタ
//*****************
CStartAndEnd::CStartAndEnd(int nPriority) :CObject2D(nPriority)
{
	for (int nCnt = 0; nCnt < TEXTUREIDX_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = 0;	// インデックス
	}
	m_nCnt = 0;
	m_bStart = false;
	m_bDeath = false;
}

//*****************
// デストラクタ
//*****************
CStartAndEnd::~CStartAndEnd()
{
}

//******************
// インスタンス生成
//******************
CStartAndEnd* CStartAndEnd::Create(D3DXVECTOR3 pos)
{
	CStartAndEnd* pStartAndEnd;								// ライフのポインタ
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ

	pStartAndEnd = new CStartAndEnd;// インスタンス生成
	// テクスチャ
	pStartAndEnd->m_nIdxTexture[TEXTUREIDX_START] = pTexture->Register("data\\TEXTURE\\open.png");
	pStartAndEnd->m_nIdxTexture[TEXTUREIDX_END] = pTexture->Register("data\\TEXTURE\\close.png");
	// 初期化
	pStartAndEnd->Init();
	pStartAndEnd->SetPos(pos);

	// 情報を渡す
	return pStartAndEnd;
}

//*****************
// 初期化処理
//*****************
HRESULT CStartAndEnd::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_START]));

	// 2Dの初期化
	CObject2D::Init();

	// UVの設定
	CObject2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObject2D::SetSize(StartAndEnd::SIZE);
	CObject2D::SetRot(VECTOR3_NULL);

	m_bStart = true;
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CStartAndEnd::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//*****************
// 更新処理
//*****************
void CStartAndEnd::Update(void)
{
	m_nCnt++;
	if (m_nCnt > nStartScreenTime&&m_bStart)
	{// スタートのUIを見えなくする
		CObject2D::SetSize(VECTOR2_NULL);

		CTexture* pTexture = CManager::GetCTexture();
		CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_END]));
		m_bStart = false;
	}
	
	if (CGame::GetState() == CGame::STATE_END&& !m_bStart)
	{
		// 音
		CSound* pSound = CManager::GetCSound();
		pSound->Play(CSound::LABEL_SE07);
		CObject2D::SetSize(StartAndEnd::SIZE);
		m_bStart = true;
		m_nCnt = 0;
		
	}

	if (m_bDeath)
	{// 安全に終了
		Uninit();
		return;
	}
}

//*****************
// 描画処理
//*****************
void CStartAndEnd::Draw(void)
{
	CObject2D::Draw();
}

//*****************
// 位置設定
//*****************
void CStartAndEnd::SetPos(D3DXVECTOR3 pos)
{
	CObject2D::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CStartAndEnd::SetRot(D3DXVECTOR3 rot)
{
	CObject2D::SetRot(rot);
}

//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CStartAndEnd::GetPos(void)
{
	return CObject2D::GetPos();
}