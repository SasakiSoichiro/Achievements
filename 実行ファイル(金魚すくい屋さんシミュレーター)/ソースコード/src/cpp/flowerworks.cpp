//**********************************
//
// 花火処理 [flowerworks.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "flowerworks.h"
#include "sparkEffect.h"
#include "manager.h"
#include <time.h>
#include "sound.h"

//********************
// コンストラクタ
//********************
CFlowerworks::CFlowerworks(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAngle = 0;			// 向き
	m_nType0Num = 0;
	m_nType1Num = 0;
	m_nType2Num = 0;
}

//********************
// デストラクタ
//********************
CFlowerworks::~CFlowerworks()
{
}

//********************
// インスタンス生成
//********************
CFlowerworks* CFlowerworks::Create(D3DXVECTOR3 pos, int type0Num, int type1Num, int type2Num)
{
	CFlowerworks* pParticle;

	pParticle = new CFlowerworks;
	pParticle->Init();
	pParticle->SetPos(pos);
	pParticle->m_nType0Num = type0Num;
	pParticle->m_nType1Num = type1Num;
	pParticle->m_nType2Num = type2Num;

	return pParticle;
}


//********************
// 初期化処理
//********************
HRESULT CFlowerworks::Init(void)
{
	return S_OK;
}

//********************
// 終了処理
//********************
void CFlowerworks::Uninit(void)
{
	CObject::Release();
}

//********************
// 更新処理
//********************
void CFlowerworks::Update(void)
{
	// 1つ目のタイプ
	float r = (float)(rand() % nRand) * fRatio;
	float g = (float)(rand() % nRand) * fRatio;
	float b = (float)(rand() % nRand) * fRatio;
	Sphere(m_nType0Num, TYPE_ZERO, D3DXCOLOR(r, g, b, fAlpha));

	// 2つ目のタイプ
	r = (float)(rand() % nRand) * fRatio;
	g = (float)(rand() % nRand) * fRatio;
	b = (float)(rand() % nRand) * fRatio;
	Sphere(m_nType1Num, TYPE_ONE, D3DXCOLOR(r, g, b, fAlpha));

	// 3つ目のタイプ
	r = (float)(rand() % nRand) * fRatio;
	g = (float)(rand() % nRand) * fRatio;
	b = (float)(rand() % nRand) * fRatio;
	Sphere(m_nType2Num, TYPE_TWO, D3DXCOLOR(r, g, b, fAlpha));

	Uninit();
}

//********************
// 描画処理
//********************
void CFlowerworks::Draw(void)
{

}

//********************
// 球生成
//********************
void CFlowerworks::Sphere(int nNum, int nType, D3DXCOLOR col)
{
	for (int nCntAngle = 0; nCntAngle < nNum; nCntAngle++)
	{
		for (int nCntPhi = 0; nCntPhi < nNum; nCntPhi++)
		{
			D3DXVECTOR3 vec = VECTOR3_NULL;

			float fAngle = D3DX_PI / nNum * nCntAngle;
			float fPhi = PI_DOUBLE / nNum * nCntPhi;

			vec.x = sinf(fAngle) * cosf(fPhi);
			vec.y = cosf(fAngle);
			vec.z = sinf(fAngle) * sinf(fPhi);

			CSparkEffect::Create(m_pos, vec, D3DXCOLOR(col), nType);
		}
	}
}

//********************
// 位置設定
//********************
void CFlowerworks::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}