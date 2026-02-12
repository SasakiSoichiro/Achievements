//***************************************
//
// 波紋処理 [wave.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "wave.h"
#include "mathUtil.h"

//*******************
// コンストラクタ
//*******************
CWave::CWave()
{
	m_Wave.pMeshVtxBuff = nullptr;
	m_Wave.nXMesh = 0;
	m_Wave.nZMesh = 0;
	m_Wave.uv = VECTOR2_NULL;
	m_Wave.fHeightOffset = 0.0f;
	m_nTime = 0;
	m_bLife = true;
}

//*******************
// デストラクタ
//*******************
CWave::~CWave()
{
}

//*******************
// インスタンス生成
//*******************
CWave* CWave::Create(WAVE wave)
{
	CWave* pWave;// メッシュ情報取得

	// インスタンス生成
	pWave = new CWave;
	// 初期化
	pWave->Init();
	pWave->m_Wave = wave;

	return pWave;
}

//*************
// 初期化処理
//*************
HRESULT CWave::Init(void)
{

	return S_OK;
}

//*************
// 終了処理
//*************
void CWave::Uninit(void)
{
	m_Wave.pMeshVtxBuff = nullptr;
}

//*************
// 更新処理
//*************
void CWave::Update(void)
{
	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	D3DXVECTOR2 uv;
	D3DXVECTOR2 uv2;
	float distance;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Wave.pMeshVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPOS = 0;

	float Xmesh = (float)(m_Wave.nXMesh);
	float Zmesh = (float)(m_Wave.nZMesh);

	for (int nCntZ = 0; nCntZ < m_Wave.nZMesh + 1; nCntZ++)
	{
		//uvの値Y座標が大きくなるにつれて0.0f～1.0fになる
		uv.y = 1.0f / m_Wave.nZMesh * nCntZ;

		for (int nCntX = 0; nCntX < m_Wave.nXMesh + 1; nCntX++, nCntPOS++)
		{
			//uvの値X座標が大きくなるにつれて0.0f～1.0fになる
			uv.x = 1.0f / m_Wave.nXMesh * nCntX;
			uv2.x = MathUtil::Double(uv.x - m_Wave.uv.x);
			uv2.y = MathUtil::Double(uv.y - m_Wave.uv.y);


			// 絶対値にする
			uv2.x = fabsf(uv2.x);
			uv2.y = fabsf(uv2.y);

			distance = sqrtf(((uv2.x) * (uv2.x)) + ((uv2.y) * (uv2.y)));

			bool bWaveRing = WaveRing(distance);

			// 波の位置を計算
			distance = sinf(distance * (fAmount * D3DX_PI) - float(m_nTime) * fSpeed) * (1.0f - distance) * fScale;
			// 絶対値にする
			distance = fabsf(distance);
			// 正規化
			distance = MaxDistance(distance);
			// 波の高さ
			WAVERING waveRing;
			waveRing.pVtx = pVtx;
			waveRing.Xmesh = Xmesh;
			waveRing.Zmesh = Zmesh;
			waveRing.nCntX = nCntX;
			waveRing.nCntZ = nCntZ;
			waveRing.nCntPOS = nCntPOS;
			waveRing.fDistance = distance;
			waveRing.bWaveRing = bWaveRing;
			WaveRingPos(waveRing);
			// 時間が経った処理
			NOLIFE noLife;
			noLife.pVtx = pVtx;
			noLife.Xmesh = Xmesh;
			noLife.Zmesh = Zmesh;
			noLife.nCntX = nCntX;
			noLife.nCntZ = nCntZ;
			noLife.nCntPOS = nCntPOS;
			NoLifePos(noLife);
		}
	}
	//頂点バッファをアンロックする
	m_Wave.pMeshVtxBuff->Unlock();

	m_nTime++;
}

//*************************
// リングの中か
//*************************
bool CWave::WaveRing(float fDistance)
{
	float fTime = float(m_nTime) * (fSpeed / (fAmount * D3DX_PI));
	return fDistance > fWidth + fTime || fDistance < fTime;
}

//*************************
// 最大値1.0fにする
//*************************
float CWave::MaxDistance(float fDistance)
{
	if (fDistance > 1.0f)
	{
		return 1.0f;
	}
	return fDistance;
}

//*************************
// リング内の処理
//*************************
void CWave::WaveRingPos(WAVERING wavering)
{
	if (!wavering.bWaveRing)
	{
		wavering.pVtx[wavering.nCntPOS].pos = D3DXVECTOR3(
			(MathUtil::Double(m_Wave.size.x) / wavering.Xmesh) * wavering.nCntX - m_Wave.size.x,
			wavering.fDistance * fWaveRatio + m_Wave.fHeightOffset,
			(-1 * (MathUtil::Double(m_Wave.size.y) / wavering.Zmesh) * wavering.nCntZ + m_Wave.size.y));
	}
}

//*************************
// 寿命を超えたら
//*************************
void CWave::NoLifePos(NOLIFE noLife)
{
	if (m_nTime > nLife)
	{
		noLife.pVtx[noLife.nCntPOS].pos = D3DXVECTOR3(
			(MathUtil::Double(m_Wave.size.x) / noLife.Xmesh) * noLife.nCntX - m_Wave.size.x,
			m_Wave.fHeightOffset, 
			(-1 * (MathUtil::Double(m_Wave.size.y) / noLife.Zmesh) * noLife.nCntZ + m_Wave.size.y));

		m_bLife = false;
	}
}