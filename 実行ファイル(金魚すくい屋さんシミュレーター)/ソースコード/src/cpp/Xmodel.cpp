//******************************************
//
// Xファイル処理 [Xmodel.cpp]
// Author:Sasaki Soichiro
//
//******************************************

//*******************
// インクルード
//*******************
#include "Xmodel.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数初期化
int CXModel::m_nNumAll = 0;

//*****************
// コンストラクタ
//*****************
CXModel::CXModel()
{
	for (int nCnt = 0; nCnt < nMaxXModel; nCnt++)
	{
		m_apXModel[nCnt].dwNumMat = NULL;
		m_apXModel[nCnt].pBuffMat = NULL;
		m_apXModel[nCnt].pMesh = NULL;
		m_apXModel[nCnt].vtxMax = NONE;
		m_apXModel[nCnt].vtxMin = NONE;
	}
	m_nNumAll++;
}

//*****************
// デストラクタ
//*****************
CXModel::~CXModel()
{
}

//*****************
// 読み込み
//*****************
HRESULT CXModel::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	return S_OK;
}

//*****************
// 破棄
//*****************
void CXModel::Unload(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < nMaxXModel; nCnt++)
	{
		if (m_apXModel[nCnt].pBuffMat != NULL)
		{
			m_apXModel[nCnt].pBuffMat->Release();
			m_apXModel[nCnt].pBuffMat = NULL;
		}
		if (m_apXModel[nCnt].pMesh != NULL)
		{
			m_apXModel[nCnt].pMesh->Release();
			m_apXModel[nCnt].pMesh = NULL;
		}
	}
}

//*****************
// 登録
//*****************
int CXModel::Register(const char* pFilename)
{
	int nCntRegister;
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (nCntRegister = 0; nCntRegister < nMaxXModel; nCntRegister++)
	{
		if (XFILE[nCntRegister].empty() == true)
		{
			if (m_apXModel[nCntRegister].dwNumMat==0
				&& m_apXModel[nCntRegister].pBuffMat == NULL
				&& m_apXModel[nCntRegister].pMesh == NULL)
			{
				// デバイスの取得
				LPDIRECT3DDEVICE9 pDevice;
				CRenderer* renderer = CManager::GetCRenderer();
				pDevice = renderer->GetDevice();
				//xファイルの読み込み
				D3DXLoadMeshFromX(pFilename,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_apXModel[nCntRegister].pBuffMat,
					NULL,
					&m_apXModel[nCntRegister].dwNumMat,
					&m_apXModel[nCntRegister].pMesh);
				nNumVtx = m_apXModel[nCntRegister].pMesh->GetNumVertices();
				sizeFVF = D3DXGetFVFVertexSize(m_apXModel[nCntRegister].pMesh->GetFVF());
				//頂点バッファのロック
				m_apXModel[nCntRegister].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					//頂点座標を比較してモデルの最小値最大値を取得
					//X============================
					if (vtx.x > m_apXModel[nCntRegister].vtxMax.x)
					{
						m_apXModel[nCntRegister].vtxMax.x = vtx.x;
					}

					if (vtx.x < m_apXModel[nCntRegister].vtxMin.x)
					{
						m_apXModel[nCntRegister].vtxMin.x = vtx.x;
					}
					//Y============================
					if (vtx.y > m_apXModel[nCntRegister].vtxMax.y)
					{
						m_apXModel[nCntRegister].vtxMax.y = vtx.y;
					}

					if (vtx.y < m_apXModel[nCntRegister].vtxMin.y)
					{
						m_apXModel[nCntRegister].vtxMin.y = vtx.y;
					}
					//Z=============================
					if (vtx.z > m_apXModel[nCntRegister].vtxMax.z)
					{
						m_apXModel[nCntRegister].vtxMax.z = vtx.z;
					}

					if (vtx.z < m_apXModel[nCntRegister].vtxMin.z)
					{
						m_apXModel[nCntRegister].vtxMin.z = vtx.z;
					}

					//頂点フォーマットのサイズ分ポインタを進める
					pVtxBuff += sizeFVF;
				}

				//頂点バッファのアンロック
				m_apXModel[nCntRegister].pMesh->UnlockVertexBuffer();
				XFILE[nCntRegister] = pFilename;
				return nCntRegister;
			}
			else
			{
				return -1;
			}
		}

		else if (XFILE[nCntRegister] == pFilename)
		{
			return nCntRegister;
		}
	}
	return -1;
}

//*****************
// 情報取得
//*****************
CXModel::XMODEL CXModel::GetAddres(int nIdx)
{
	return m_apXModel[nIdx];
}