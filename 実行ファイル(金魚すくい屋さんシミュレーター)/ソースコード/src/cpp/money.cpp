//**********************************
//
// お金処理 [money.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "money.h"
#include "manager.h"
#include "number.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "sound.h"

//****************
// 静的メンバ変数
//****************
CNumber* CMoney::m_apNumber[MAX_DIGIT] = {};
int CMoney::m_nScore = 0;

//=================
// コンストラクタ
//=================
CMoney::CMoney(int nPriority) :CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	m_nIdxTexture = 0;
	m_bDeath = false;
	for (int nCnt = 0; nCnt < nTextureNum; nCnt++)
	{
		m_pObject2D[nCnt] = nullptr;
	}
}

//********************
// デストラクタ
//********************
CMoney::~CMoney()
{
}

//********************
// インスタンス生成
//********************
CMoney* CMoney::Create(D3DXVECTOR3 pos)
{
	CMoney* pScore;
	CTexture* pTexture = CManager::GetCTexture();

	pScore = new CMoney;
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		if (m_apNumber[nCntD] == NULL)
		{
			m_apNumber[nCntD] = new CNumber;
		}
	}
	pScore->SetPos(pos);
	pScore->Init();
	pScore->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\number.png");

	return pScore;
}

//********************
// 初期化処理
//********************
HRESULT CMoney::Init(void)
{
	GameInit(GetPos());
	return S_OK;
}

//********************
// 終了処理
//********************
void CMoney::Uninit(void)
{
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		if (m_apNumber[nCntD] != NULL)
		{
			m_apNumber[nCntD]->Uninit();
			delete m_apNumber[nCntD];
			m_apNumber[nCntD] = NULL;
		}
	}
	CObject::Release();
}

//********************
// 更新処理
//********************
void CMoney::Update(void)
{
	int nData[2];
	nData[0] = 10; nData[1] = 1;
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		m_apNumber[nCntD]->SetNumber((m_nScore % nData[0]) / nData[1]);
		nData[0] *= 10;
		nData[1] *= 10;
	}

	if (m_bDeath)
	{
		for (int nCnt = 0; nCnt < nTextureNum; nCnt++)
		{
			m_pObject2D[nCnt]->Uninit();
			m_pObject2D[nCnt] = nullptr;
		}

		Uninit();
	}
}

//********************
// 描画処理
//********************
void CMoney::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CTexture* pTexture = CManager::GetCTexture();
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	for (int nCntD = 0; nCntD < Digit(m_nScore); nCntD++)
	{
		pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture));
		m_apNumber[nCntD]->Draw();
	}
}

//********************
// ゲーム開始時の初期化
//********************
void CMoney::GameInit(D3DXVECTOR3 pos)
{
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		m_apNumber[nCntD]->Init();
		m_apNumber[nCntD]->SetNumber(0);
		m_apNumber[nCntD]->SetSize(D3DXVECTOR2(fSizeX, fSizeY));
		m_apNumber[nCntD]->SetPos(D3DXVECTOR3(pos.x - fSizeX * nCntD, pos.y, 0.0f));
	}

	// 「\」と「円」のテクスチャの位置設定
	m_pObject2D[0] = CObject2D::Create(D3DXVECTOR3(pos.x - fOffsetX00, pos.y, 0.0f), "data\\TEXTURE\\yenMark.png");
	m_pObject2D[1] = CObject2D::Create(D3DXVECTOR3(pos.x + fOffsetX01, pos.y, 0.0f), "data\\TEXTURE\\yen.png");

	// 共通部分の初期化
	for (int nCnt = 0; nCnt < nTextureNum; nCnt++)
	{
		m_pObject2D[nCnt]->SetSize(D3DXVECTOR2(fTextureSize_X, fTextureSize_Y));
		m_pObject2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject2D[nCnt]->SetTex(D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f),
			D3DXVECTOR2(1.0f, 1.0f));
	}
}

void CMoney::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
void CMoney::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}
void CMoney::SetScore(int nScore)
{
	m_nScore = nScore;
}
void CMoney::Add(int nValue)
{
	m_nScore += nValue;
	CSound* pSound = CManager::GetCSound();
	pSound->Play(CSound::LABEL_SE01);
}
int CMoney::Digit(int nScore)
{
	int nNum = nScore;//スコアの０の部分をカウント
	int nKeepNum = 0;//スコアの桁数
	while (nNum != 0)//割り切れなくなるまで繰り返す
	{
		nNum /= 10;//10で割る
		nKeepNum++;//桁数を加算
	}
	if (nScore == 0)
	{
		nKeepNum = 1;//一桁目が０の時
	}
	return nKeepNum;
}