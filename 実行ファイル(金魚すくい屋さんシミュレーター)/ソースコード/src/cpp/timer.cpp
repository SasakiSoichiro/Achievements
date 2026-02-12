//**********************************
//
// タイマー処理 [timer.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "timer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "renderer.h"

//*****************
// 静的メンバ変数
//*****************
CNumber* CTimer::m_apMin[2] = {};
CNumber* CTimer::m_apSec[2] = {};
CSymbol* CTimer::m_pSymbol = NULL;
int CTimer::m_nTime = 0;
int CTimer::m_nFps = 0;

// 定数
namespace Timer
{
	D3DXCOLOR COLOR = { 0.8f,0.8f,0.0f,1.0f };
}

//*****************
// コンストラクタ
//*****************
CTimer::CTimer(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_bDeath = false;
	for (int nCnt = 0; nCnt < TEXTUREIDX_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = 0;
	}
}

//*****************
// デストラクタ
//*****************
CTimer::~CTimer()
{
}

//*****************
// インスタンス生成
//*****************
CTimer* CTimer::Create(D3DXVECTOR3 pos, int nTime)
{
	CTimer* pTimer;
	CTexture* pTexture = CManager::GetCTexture();
	int nNum = GetNumAll();

	pTimer = new CTimer;
	for (int nCntTime = 0; nCntTime < nMaxDigit; nCntTime++)
	{
		m_apMin[nCntTime] = new CNumber;
		m_apSec[nCntTime] = new CNumber;
		pTimer->m_nIdxTexture[TEXTUREIDX_NUMBER] = pTexture->Register("data\\TEXTURE\\number.png");

	}
	m_pSymbol = new CSymbol;
	pTimer->m_nIdxTexture[TEXTUREIDX_COLON] = pTexture->Register("data\\TEXTURE\\Colon.png");

	pTimer->SetPos(pos);
	pTimer->Init();
	m_nTime = nTime;
	return pTimer;
}

//*****************
// 初期化処理
//*****************
HRESULT CTimer::Init(void)
{
	D3DXVECTOR3 pos = GetPos();
	for (int nCntTime = 0; nCntTime < nMaxDigit; nCntTime++)
	{
		m_apMin[nCntTime]->Init();
		m_apMin[nCntTime]->SetNumber(0);
		m_apMin[nCntTime]->SetSize(D3DXVECTOR2(fSizeX, fSizeY));
		m_apMin[nCntTime]->SetPos(D3DXVECTOR3((pos.x + fOffsetLeft) - fSizeX * nCntTime, pos.y, 0.0f));
		m_apMin[nCntTime]->SetCol(Timer::COLOR);

		m_apSec[nCntTime]->Init();
		m_apSec[nCntTime]->SetNumber(0);
		m_apSec[nCntTime]->SetSize(D3DXVECTOR2(fSizeX, fSizeY));
		m_apSec[nCntTime]->SetPos(D3DXVECTOR3((pos.x + fOffsetRight) - fSizeX * nCntTime, pos.y, 0.0f));
		m_apSec[nCntTime]->SetCol(Timer::COLOR);
	}
	m_pSymbol->Init();
	m_pSymbol->SetSize(D3DXVECTOR2(fSizeX + fSizeOffset, fSizeY + fSizeOffset));
	m_pSymbol->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	m_pSymbol->SetCol(Timer::COLOR);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CTimer::Uninit(void)
{
	for (int nCntTime = 0; nCntTime < nMaxDigit; nCntTime++)
	{
		if (m_apMin[nCntTime] != NULL)
		{
			m_apMin[nCntTime]->Uninit();
			delete m_apMin[nCntTime];
			m_apMin[nCntTime] = NULL;
		}
		if (m_apSec[nCntTime] != NULL)
		{
			m_apSec[nCntTime]->Uninit();
			delete m_apSec[nCntTime];
			m_apSec[nCntTime] = NULL;
		}
	}
	if (m_pSymbol != NULL)
	{
		m_pSymbol->Uninit();
		delete m_pSymbol;
		m_pSymbol = NULL;

	}
	CObject::Release();
}

//*****************
// 更新処理
//*****************
void CTimer::Update(void)
{
	m_nFps++;
	if (m_nFps >= nSecond && m_nTime>=0)
	{
		m_nTime--;
		m_nFps = 0;
	}
	int nData[2];
	nData[0] = 10; nData[1] = 1;
	int nMin = 0, nSec = 0;
	nMin = m_nTime / nSecond;
	nSec = m_nTime % nSecond;
	for (int nCntD = 0; nCntD < nMaxDigit; nCntD++)
	{
		m_apMin[nCntD]->SetNumber((nMin % nData[0]) / nData[1]);
		m_apSec[nCntD]->SetNumber((nSec % nData[0]) / nData[1]);

		nData[0] *= 10;
		nData[1] *= 10;
	}
	if (m_nTime <= 0)
	{
		m_nTime = 0;
		CGame::SetState(CGame::STATE_END);
	}

	if (m_bDeath)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void CTimer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CTexture* pTexture = CManager::GetCTexture();
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	for (int nCntTime = 0; nCntTime < nMaxDigit; nCntTime++)
	{

		pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_NUMBER]));
		m_apMin[nCntTime]->Draw();
		pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_NUMBER]));
		m_apSec[nCntTime]->Draw();
	}
	pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture[TEXTUREIDX_COLON]));
	m_pSymbol->Draw();

}

//*****************
// 位置設定
//*****************
void CTimer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//*****************
// 向き設定
//*****************
void CTimer::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//*****************
// 時間設定
//*****************
void CTimer::SetTime(int nTime)
{
	m_nTime = nTime;
}

//*****************
// 加算
//*****************
void CTimer::Add(int nTime)
{
	m_nTime += nTime;
}
