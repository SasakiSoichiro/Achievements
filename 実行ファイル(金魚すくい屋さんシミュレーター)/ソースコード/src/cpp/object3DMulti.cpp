//*********************************************************
//
// オブジェクト3Dマルチテクスチャ処理 [object3DMulti.cpp]
// Author Sasaki Soichiro
//
//*********************************************************

//*******************
// インクルード
//*******************
#include "object3DMulti.h"
#include "renderer.h"
#include "manager.h"
#include "mathUtil.h"

//=================
// コンストラクタ
//=================
CObject3DMulti::CObject3DMulti(int nPriority) :CObject(nPriority)
{
	m_pTexture[0] = NULL;
	m_pTexture[1] = NULL;
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=================
// デストラクタ
//=================
CObject3DMulti::~CObject3DMulti()
{
}

//=================
// 初期化処理
//=================
HRESULT CObject3DMulti::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	float fAngle = atan2f(m_size.x, m_size.y);
	VERTEX_3D_MULTI* pVtx;//頂点情報へのポインタ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * 4,//(sizeof(VERTEX_3D_MULTI)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float fSizeX = MathUtil::Half(m_size.x);
	float fSizeY = MathUtil::Half(m_size.y);
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSizeX, 0.0f, +fSizeY);
	pVtx[1].pos = D3DXVECTOR3(+fSizeX, 0.0f, +fSizeY);
	pVtx[2].pos = D3DXVECTOR3(-fSizeX, 0.0f, -fSizeY);
	pVtx[3].pos = D3DXVECTOR3(+fSizeX, 0.0f, -fSizeY);
	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	// 頂点カラーの設定　　　　
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;

}

//=================
// 終了処理
//=================
void CObject3DMulti::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	CObject::Release();
}

//=================
// 更新処理
//=================
void CObject3DMulti::Update(void)
{
}

//=================
// 描画処理
//=================
void CObject3DMulti::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));
	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[0]);
	pDevice->SetTexture(1, m_pTexture[1]);
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//***************************
// テクスチャ座標の設定
//***************************
void CObject3DMulti::SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4)
{
	VERTEX_3D_MULTI* pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].tex = uv1;
	pVtx[1].tex = uv2;
	pVtx[2].tex = uv3;
	pVtx[3].tex = uv4;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//***************************
// テクスチャ座標の設定
//***************************
void CObject3DMulti::SetTex2(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4)
{
	VERTEX_3D_MULTI* pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].texM = uv1;
	pVtx[1].texM = uv2;
	pVtx[2].texM = uv3;
	pVtx[3].texM = uv4;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//***************************
// 色の設定
//***************************
void CObject3DMulti::SetCol(D3DXCOLOR col)
{
	m_col = col;
	VERTEX_3D_MULTI* pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//***************************
// 大きさ
//***************************
void CObject3DMulti::SetSize(D3DXVECTOR2 size)
{
	VERTEX_3D_MULTI* pVtx;//頂点情報へのポインタ
	m_size = size;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	float fSizeX = MathUtil::Half(m_size.x);
	float fSizeY = MathUtil::Half(m_size.y);

	pVtx[0].pos = D3DXVECTOR3(-fSizeX, 0.0f, +fSizeY);
	pVtx[1].pos = D3DXVECTOR3(+fSizeX, 0.0f, +fSizeY);
	pVtx[2].pos = D3DXVECTOR3(-fSizeX, 0.0f, -fSizeY);
	pVtx[3].pos = D3DXVECTOR3(+fSizeX, 0.0f, -fSizeY);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//***************************
// テクスチャの割り当て
//***************************
void CObject3DMulti::BindTexture(LPDIRECT3DTEXTURE9 pTexture0, LPDIRECT3DTEXTURE9 pTexture1)
{
	m_pTexture[0] = pTexture0;
	m_pTexture[1] = pTexture1;
}