//*************************************************
//
// オブジェクト3Dシェーダ処理 [object3DShader.cpp]
// Author:Sasaki Soichiro
//
//*************************************************

//*******************
// インクルード
//*******************
#include "objectShader3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "mathUtil.h"

CObjectShader3D::CObjectShader3D(int nPriority) : CObject(nPriority)
{
	m_pShader = NULL;
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
}
CObjectShader3D::~CObjectShader3D()
{

}

HRESULT CObjectShader3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	D3DXVECTOR3 vec0, vec1, nor0, nor1;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//(sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = VECTOR3_NULL;
	pVtx[1].pos = VECTOR3_NULL;
	pVtx[2].pos = VECTOR3_NULL;
	pVtx[3].pos = VECTOR3_NULL;

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定　　　　
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

void CObjectShader3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	CObject::Release();
}
void CObjectShader3D::Update(void)
{

}
void CObjectShader3D::Draw(void)
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
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
	pDevice->SetTexture(1, NULL);
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (m_pShader)
	{
		SetShaderParam();
		m_pShader->CommitChanges();
		// シェーダ起動
		m_pShader->Begin(0, 0);
		// パス起動
		m_pShader->BeginPass(0);
	}

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (m_pShader)
	{
		// パス終了
		m_pShader->EndPass();
		// シェーダ終了
		m_pShader->End();
	}
}

void CObjectShader3D::SetSize(D3DXVECTOR2 size)
{
	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fSizeX = MathUtil::Half(size.x);
	float fSizeY = MathUtil::Half(size.y);
	////頂点座標の更新
	pVtx[0].pos.x = -fSizeX;
	pVtx[0].pos.y;
	pVtx[0].pos.z = +fSizeY;
	pVtx[1].pos.x = +fSizeX;
	pVtx[1].pos.y;
	pVtx[1].pos.z = +fSizeY;
	pVtx[2].pos.x = -fSizeX;
	pVtx[2].pos.y;
	pVtx[2].pos.z = -fSizeY;
	pVtx[3].pos.x = +fSizeX;
	pVtx[3].pos.y;
	pVtx[3].pos.z = -fSizeY;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

void CObjectShader3D::SetCol(D3DXCOLOR col)
{
	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObjectShader3D::SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2)
{

	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(uv1.x, uv1.y);
	pVtx[1].tex = D3DXVECTOR2(uv2.x, uv1.y);
	pVtx[2].tex = D3DXVECTOR2(uv1.x, uv2.y);
	pVtx[3].tex = D3DXVECTOR2(uv2.x, uv2.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

void CObjectShader3D::BindShader(LPD3DXEFFECT pShader)
{
	m_pShader = pShader;
}