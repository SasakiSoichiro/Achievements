//***************************************
// 
// モデル処理[model.cpp]
// Author:Sasaki Soichiro
// 
//***************************************

//******************
// インクルード
//******************
#include "model.h"
#include "manager.h"
#include "renderer.h"

// 定数
namespace Model
{
	D3DXVECTOR3 SIZE = { 1.0f,1.0f,1.0f };					// 大きさ
	D3DXVECTOR3 PLANE_POINT = { 0.0f, 0.3f, 0.0f };			// 平面投影座標
	D3DXVECTOR3 PLANE_DIR = { 0.0f, -0.5f, 0.0f };			// 平面投影法線
	D3DXVECTOR4 LIDHIT_DIR = { 0.4f, -0.6f, 0.4f, 0.0f };	// ライトの方向
	D3DXCOLOR SHADOW_DIFFUSE = { 0.0f,0.0f,0.0f,1.0f };		// 影の色
	D3DXCOLOR SHADOW_AMBIENT = { 0.0f,0.0f,0.0f,1.0f };		// 影の環境光
}

//*****************
// コンストラクタ
//*****************
CModel::CModel()
{
	m_pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
	m_rot = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
	m_nIdxXFile = 0;
	m_pParent = NULL;
	m_mtxWorld = {};
	m_pXModel = {};
	
}

//********************
// デストラクタ
//********************
CModel::~CModel()
{
}

CModel* CModel::Create(D3DXVECTOR3 pos, const char* pFileName)
{
	CModel* pModel;
	CXModel* pXModel = CManager::GetCXModel();

	pModel = new CModel;
	pModel->m_nIdxXFile = pXModel->Register(pFileName);
	pModel->SetPos(pos);
	pModel->Init();

	return pModel;
}

//********************
// 初期化処理
//********************
HRESULT CModel::Init(void)
{
	// XModelの取得
	CXModel* pXModel = CManager::GetCXModel();

	//xファイルの読み込み
	m_pXModel = pXModel->GetAddres(m_nIdxXFile);
	m_offsetPos = GetPos();
	m_rot = VECTOR3_NULL;
	m_size= Model::SIZE;

	return S_OK;
}

//********************
// 終了処理
//********************
void CModel::Uninit(void)
{
}

//********************
// 更新処理
//********************
void CModel::Update(void)
{

}

//********************
// 描画処理
//********************
void CModel::Draw(void)
{
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

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxParent;// 親のマトリックス
	if (m_pParent != NULL)
	{// 親が存在する
		// 親のマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// 現在(最新)のマトリックスを取得する[=プレイヤーのマトリックス]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	// 親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pXModel.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pXModel.dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		////テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//モデル（パーツ）の描画
		m_pXModel.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);

	DrawShaodow();
}

//****************************************
// マトリックスシャドウ処理
//****************************************
void CModel::DrawShaodow(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetCRenderer()->GetDevice();
	if (!pDevice) return;

	// 平面投影座標を設定
	D3DXPLANE plane;
	D3DXPlaneFromPointNormal(&plane, &Model::PLANE_POINT, &Model::PLANE_DIR);

	// 影マトリックス
	D3DXMATRIX mtxShadow;
	D3DXMatrixShadow(&mtxShadow, &Model::LIDHIT_DIR, &plane);

	// 影をモデルの位置に合わせる
	D3DXMATRIX mtxWorldShadow;
	D3DXMatrixMultiply(&mtxWorldShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldShadow);

	// 影の色設定
	D3DMATERIAL9 shadowMat = {};
	shadowMat.Diffuse = Model::SHADOW_DIFFUSE;
	shadowMat.Ambient = Model::SHADOW_AMBIENT;

	// マテリアルの設定
	pDevice->SetMaterial(&shadowMat);

	// マテリアル数だけ回す
	for (int nCntMat = 0; nCntMat < (int)m_pXModel.dwNumMat; nCntMat++)
	{
		// モデル(パーツ)の描画
		m_pXModel.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&shadowMat);
}


//****************
// 親の設定
//****************
void CModel::SetParent(CModel* pModel)
{
	m_pParent = pModel;
}

//************************
// マトリックス情報の取得
//************************
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}