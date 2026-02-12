//**********************************
//
// お金3D処理 [money3D.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "money3D.h"
#include "manager.h"
#include "number.h"
#include "renderer.h"
#include "texture.h"
#include "object3D.h"
#include "sound.h"
#include "money.h"

//****************
// 静的メンバ変数
//****************
CNumber3D* CMoney3D::m_apNumber[MAX_DIGIT] = {};

// 定数
namespace Money3D
{
	D3DXVECTOR3 ROT = { -PI_HALF, 0.0f, 0.0f };
}

//******************
// コンストラクタ
//******************
CMoney3D::CMoney3D(int nPriority) :CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	m_nIdxTexture = 0;
	for (int nCnt = 0; nCnt < nTextureNum; nCnt++)
	{
		m_pObject3D[nCnt] = nullptr;
	}
}

//******************
// デストラクタ
//******************
CMoney3D::~CMoney3D()
{
}

//******************
// インスタンス生成
//******************
CMoney3D* CMoney3D::Create(D3DXVECTOR3 pos)
{
	CMoney3D* pScore;
	CTexture* pTexture = CManager::GetCTexture();

	pScore = new CMoney3D;
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		if (m_apNumber[nCntD] == NULL)
		{
			m_apNumber[nCntD] = new CNumber3D;
		}
	}
	pScore->SetPos(pos);
	pScore->Init();
	pScore->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\number.png");

	return pScore;
}

//******************
// 初期化処理
//******************
HRESULT CMoney3D::Init(void)
{
	D3DXVECTOR3 pos = GetPos();
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		m_apNumber[nCntD]->Init();
		m_apNumber[nCntD]->SetNumber(0);
		m_apNumber[nCntD]->SetSize(D3DXVECTOR2(fSizeX, fSizeY));
		m_apNumber[nCntD]->SetPos(D3DXVECTOR3(pos.x - fSizeX * nCntD, pos.y, pos.z));
		m_apNumber[nCntD]->SetRot(Money3D::ROT);
	}

	// 「\」と「円」のテクスチャの位置設定
	m_pObject3D[0] = CObject3D::Create(D3DXVECTOR3(pos.x - fOffsetX00, pos.y, pos.z),
		D3DXVECTOR2(fTextureSize_X, fTextureSize_Y),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),
		"data\\TEXTURE\\yenMark.png");
	m_pObject3D[0]->SetRot(Money3D::ROT);

	m_pObject3D[1] = CObject3D::Create(D3DXVECTOR3(pos.x + fOffsetX01, pos.y, pos.z),
		D3DXVECTOR2(fTextureSize_X, fTextureSize_Y),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f), "data\\TEXTURE\\yen.png");

	m_pObject3D[1]->SetRot(Money3D::ROT);

	return S_OK;
}

//******************
// 終了処理
//******************
void CMoney3D::Uninit(void)
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

//******************
// 更新処理
//******************
void CMoney3D::Update(void)
{
	int nData[2];
	nData[0] = 10; nData[1] = 1;
	for (int nCntD = 0; nCntD < MAX_DIGIT; nCntD++)
	{
		m_apNumber[nCntD]->SetNumber((CMoney::GetScore() % nData[0]) / nData[1]);
		nData[0] *= 10;
		nData[1] *= 10;
	}
}

//******************
// 描画処理
//******************
void CMoney3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CTexture* pTexture = CManager::GetCTexture();
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	for (int nCntD = 0; nCntD < Digit(CMoney::GetScore()); nCntD++)
	{
		pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture));
		m_apNumber[nCntD]->Draw();
	}
}

//******************
// 位置設定
//******************
void CMoney3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//******************
// 向き設定
//******************
void CMoney3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//******************
// 桁数のカウント
//******************
int CMoney3D::Digit(int nScore)
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