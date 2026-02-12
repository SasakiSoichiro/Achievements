//*********************************************************
//
// オブジェクト2Dマルチテクスチャ処理 [object2DMulti.cpp]
// Author Sasaki Soichiro
//
//*********************************************************

//*******************
// インクルード
//*******************
#include "object2DMulti.h"
#include "renderer.h"
#include "manager.h"
#include "mathUtil.h"

//=================
// コンストラクタ
//=================
CObject2DMulti::CObject2DMulti(int nPriority) :CObject(nPriority)
{
	m_pTexture[0] = NULL;
	m_pTexture[1] = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=================
// デストラクタ
//=================
CObject2DMulti::~CObject2DMulti()
{
}

//=================
// 初期化処理
//=================
HRESULT CObject2DMulti::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	float fAngle = atan2f(m_size.x, m_size.y);
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_MULTI) * 4,//(sizeof(VERTEX_2D_MULTI)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D_MULTI,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * m_size.x, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * m_size.y, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * m_size.x, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * m_size.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * m_size.x, m_pos.y + cosf(m_rot.z - fAngle) * m_size.y, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * m_size.x, m_pos.y + cosf(m_rot.z + fAngle) * m_size.y, 0.0f);

	//rhwの設定
	pVertex[0].rhw = 1.0f;
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;
	//頂点カラーの設定　　　　
	pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;

}

//=================
// 終了処理
//=================
void CObject2DMulti::Uninit(void)
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
void CObject2DMulti::Update(void)
{
}

//=================
// 描画処理
//=================
void CObject2DMulti::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D_MULTI));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D_MULTI);
	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[0]);
	pDevice->SetTexture(1, m_pTexture[1]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetTexture(1, NULL);
}
void CObject2DMulti::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	m_pos = pos;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject2DMulti::SetRot(D3DXVECTOR3 rot)
{
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	m_rot = rot;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
void CObject2DMulti::SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4)
{
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].tex = uv1;
	pVertex[1].tex = uv2;
	pVertex[2].tex = uv3;
	pVertex[3].tex = uv4;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject2DMulti::SetTex2(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4)
{
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].texM = uv1;
	pVertex[1].texM = uv2;
	pVertex[2].texM = uv3;
	pVertex[3].texM = uv4;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject2DMulti::SetCol(D3DXCOLOR col)
{
	m_col = col;
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].col = col;
	pVertex[1].col = col;
	pVertex[2].col = col;
	pVertex[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject2DMulti::SetSize(D3DXVECTOR2 size)
{
	VERTEX_2D_MULTI* pVertex;//頂点情報へのポインタ
	m_size = size;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}


void CObject2DMulti::BindTexture(LPDIRECT3DTEXTURE9 pTexture0, LPDIRECT3DTEXTURE9 pTexture1)
{
	m_pTexture[0] = pTexture0;
	m_pTexture[1] = pTexture1;
}