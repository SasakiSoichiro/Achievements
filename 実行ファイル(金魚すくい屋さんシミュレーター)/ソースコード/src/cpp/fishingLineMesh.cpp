//*******************************************
// 
// 釣り糸メッシュ処理[fishingLineMesh.cpp]
// Author:Sasaki Soichiro
// 
//*******************************************

//******************
// インクルード
//******************
#include "fishingLineMesh.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "input.h"

//****************
// コンストラクタ
//****************
CFishingLineMesh::CFishingLineMesh(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;	// 頂点バッファ
	m_pIdxBuff = nullptr;	// インデックスバッファ
	m_pTexture = nullptr;	//	テクスチャのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_nNumVtx = 0;
	m_nNumIdx = 0;
}

//****************
// デストラクタ
//****************
CFishingLineMesh::~CFishingLineMesh()
{

}

//*******************
// インスタンス生成
//*******************
CFishingLineMesh* CFishingLineMesh::Create(D3DXVECTOR3 pos)
{
	CFishingLineMesh* pOrbit;// メッシュ情報取得
	CTexture* pTexture = CManager::GetCTexture();// テクスチャ情報の取得

	// インスタンス生成
	pOrbit = new CFishingLineMesh;
	// 初期化
	pOrbit->SetPos(pos);
	pOrbit->Init();
	// pOrbitを渡す
	return pOrbit;
}

//**************
// 初期化
//**************
HRESULT CFishingLineMesh::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\floor.jpg",
		&m_pTexture);

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXCNT,	// (sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(VERTEX_3D) * IDXCNT,	// (sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	m_nNumVtx = 4;
	m_nNumIdx = 4;

	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_pos;
	pVtx[1].pos = m_pos;
	pVtx[2].pos = m_pos;
	pVtx[3].pos = m_pos;
	for (int nCnt = 0; nCnt < VTXCNT; nCnt++)
	{
		pVtx[nCnt].pos = m_pos;
	}


	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCnt = 0; nCnt < m_nNumIdx; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}

	//頂点バッファをアンロックする
	m_pIdxBuff->Unlock();
	return S_OK;
}

//**************
// 終了処理
//**************
void CFishingLineMesh::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//頂点バッファの解放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	CObject::Release();
}

//**************
// 更新
//**************
void CFishingLineMesh::Update(void)
{

}

//**************
// 描画
//**************
void CFishingLineMesh::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumVtx-2);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//*****************
// 頂点の追加
//*****************
void CFishingLineMesh::Add(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	m_nNumVtx += 2;
	m_nNumIdx += 2;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[m_nNumVtx-1].pos = pos0;
	pVtx[m_nNumVtx].pos = pos1;

	for (int nCnt = m_nNumVtx-2; nCnt < m_nNumVtx; nCnt++)
	{
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCnt = m_nNumIdx-2; nCnt < m_nNumIdx; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}

	//頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

}

//*******************
// 頂点位置の設定
//*******************
void CFishingLineMesh::SetVtxPos(D3DXVECTOR3 pos, int nCnt)
{
	VERTEX_3D* pVtx = 0;// 頂点情報へのポインタ

	bool bVtx = nCnt >= 0 && nCnt < m_nNumVtx;
	
	if (bVtx)
	{// nCntが範囲内だったら

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[nCnt].pos = pos;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
