//**********************************
//
// Xファイル処理 [objectX.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "objectX.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "shadow.h"

//=================
// コンストラクタ
//=================
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{
	m_XModel.pMesh = nullptr;
	m_XModel.pBuffMat = nullptr;
	m_XModel.dwNumMat = NULL;
	m_apTextureModel = nullptr;
	m_pShadow = nullptr;
	m_mtxWorld = {};
	m_pos = VECTOR3_NULL;		// 位置
	m_rot = VECTOR3_NULL;		// 向き
	m_bQua = false;
	for (int nCnt = 0; nCnt < nMaxTexture; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;
	}
}

//=================
// デストラクタ
//=================
CObjectX::~CObjectX()
{
}

//******************
// インスタンス生成
//******************
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, bool bQua = false)
{
	CObjectX* pObjectX;
	CTexture* pTexture = CManager::GetCTexture();

	pObjectX = new CObjectX;
	pObjectX->SetPos(pos);
	pObjectX->Init();
	return pObjectX;
}

//******************
// 初期化処理
//******************
HRESULT CObjectX::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//各種変数の初期化
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XModel.pBuffMat->GetBufferPointer();
	CTexture* pTexture = CManager::GetCTexture();

	for (int nCntMat = 0; nCntMat < (int)m_XModel.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
				//テクスチャの読み込み
			m_nIdxTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
		}
	}

	float fRandHeight = (rand() % nShadowHeightRand) * nShadowRandSmall;
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y + nShadowOffsetHeight + fRandHeight, m_pos.z));

	return TRUE;
}
//******************
// 終了処理
//******************
void CObjectX::Uninit(void)
{	
	CObject::Release();
}

//******************
// 更新処理
//******************
void CObjectX::Update(void)
{

}

//******************
// 描画処理
//******************
void CObjectX::Draw(void)
{
	CTexture* pTexture = CManager::GetCTexture();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//現在のマテリアルの保存用
	D3DMATERIAL9 matDef;//現在のマテリアルの保存用
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxTrans, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bQua)
	{
		D3DXQuaternionNormalize(&m_qua, &m_qua);
		D3DXMatrixRotationQuaternion(&mtxRot, &m_qua);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else
	{
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XModel.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XModel.dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_nIdxTexture[nCntMat] != -1)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAddres(m_nIdxTexture[nCntMat]));
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		//モデル（パーツ）の描画
		m_XModel.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);

}

//******************
// 影の表示設定
//******************
void CObjectX::SetShadowScreen(bool bScreen)
{
	m_pShadow->SetScreen(bScreen);
}

//**********************
// テクスチャ割り当て
//**********************
void CObjectX::BindXFile(CXModel::XMODEL pXFile)
{
	m_XModel = pXFile;
}
