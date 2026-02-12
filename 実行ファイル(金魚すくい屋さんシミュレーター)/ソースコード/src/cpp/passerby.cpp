//***************************************
//
// 通行人処理 [passerby.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "passerby.h"
#include "manager.h"
#include "loadMotionManager.h"
#include "renderer.h"

//*******************
// コンストラクタ
//*******************
CPasserby::CPasserby(int nPriority) :CObject(nPriority)
{
	for (int nCnt = 0; nCnt < maxModelParts; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}
	m_pMotion = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
	m_bDeath = false;
	m_bRight = false;
	m_bDash = false;
	m_nNumModel = 0;
	m_nUninitCnt = 0;
	m_fSpeed = 0.0f;
}

//*******************
// デストラクタ
//*******************
CPasserby::~CPasserby()
{
}

//*******************
// インスタンス生成
//*******************
CPasserby* CPasserby::Create(D3DXVECTOR3 pos,bool bRight,bool bDash)
{
	// 自身のポインタ
	CPasserby* pCustomer = nullptr;

	// インスタンス生成
	pCustomer = new CPasserby;

	// 初期化
	pCustomer->m_pos = pos;
	pCustomer->m_bRight = bRight;
	pCustomer->m_bDash = bDash;
	pCustomer->Init();

	return pCustomer;

}

//*************
// 初期化処理
//*************
HRESULT CPasserby::Init(void)
{
	int nMotionIdx = -1;
	nMotionIdx = CManager::GetLoadMotionManager()->Register("data\\MOTION\\motion_passerby.txt");

	CModel** ppModel = &m_apModel[0];
	m_pMotion = CManager::GetLoadMotionManager()->GetAddres(nMotionIdx, ppModel, m_nNumModel);
	m_apModel[0] = *ppModel;

	if (m_bDash)
	{
		m_fSpeed = dashSpeed;
		m_pMotion->SetMotion(MOTION_DASH);
	}
	else
	{
		m_fSpeed = walkSpeed;
		m_pMotion->SetMotion(MOTION_MOVE);
	}

	if (m_bRight)
	{
		m_fSpeed = -m_fSpeed;
		m_rot.y = 0.0f;

	}
	else
	{
		m_rot.y = D3DX_PI;
	}
	return S_OK;
}

//*************
// 終了処理
//*************
void CPasserby::Uninit(void)
{
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	CObject::Release();
}

//*************
// 更新処理
//*************
void CPasserby::Update(void)
{
	m_pos.z += m_fSpeed;

	// モーションの更新
	m_pMotion->Update();
	// 終了処理
	m_nUninitCnt++;
	if (m_bDeath|| m_nUninitCnt> UninitCnt)
	{
		Uninit();
	}
}

//*************
// 描画処理
//*************
void CPasserby::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}