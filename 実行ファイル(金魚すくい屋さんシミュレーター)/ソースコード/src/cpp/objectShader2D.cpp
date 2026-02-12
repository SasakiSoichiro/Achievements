//**************************************************
//
// オブジェクト2Dシェーダ処理 [object2DShader.cpp]
// Author:Sasaki Soichiro
//
//**************************************************

//*******************
// インクルード
//*******************
#include "objectShader2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "shaderManager.h"
#include "mathUtil.h"

//*******************
// コンストラクタ
//*******************
CObjectShader2D::CObjectShader2D(int nPriority) :CObject(nPriority)
{
	m_pShader = NULL;
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//*******************
// デストラクタ
//*******************
CObjectShader2D::~CObjectShader2D()
{
}

//**************
// 初期化処理
//**************
HRESULT CObjectShader2D::Init(void)
{
	// 取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//(sizeof(VERTEX_2D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	float fAngle = atan2f(m_size.x, m_size.y);
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * m_size.x, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * m_size.x, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * m_size.x, m_pos.y + cosf(m_rot.z - fAngle) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * m_size.x, m_pos.y + cosf(m_rot.z + fAngle) * m_size.y, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;

}

//**************
// 終了処理
//**************
void CObjectShader2D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	CObject::Release();
}

//**************
// 更新処理
//**************
void CObjectShader2D::Update(void)
{

}

//**************
// 描画処理
//**************
void CObjectShader2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, NULL);
	pDevice->SetTexture(1, NULL);
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

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

//**************
// 位置設定
//**************
void CObjectShader2D::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	m_pos = pos;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//**************
// 向きの設定
//**************
void CObjectShader2D::SetRot(D3DXVECTOR3 rot)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	m_rot = rot;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//**************
// UV座標の設定
//**************
void CObjectShader2D::SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ
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

//**************
// 色の設定
//**************
void CObjectShader2D::SetCol(D3DXCOLOR col)
{
	m_col = col;
	VERTEX_2D* pVtx;//頂点情報へのポインタ
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

//**************
// 大きさの設定
//**************
void CObjectShader2D::SetSize(D3DXVECTOR2 size)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	m_size = size;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (-D3DX_PI + fAngle)) * fRength, 0.0f);//←右回りで設定する
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fRength, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fRength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fRength, m_pos.y + cosf(m_rot.z - fAngle) * fRength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fRength, m_pos.y + cosf(m_rot.z + fAngle) * fRength, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObjectShader2D::BindShader(LPD3DXEFFECT pShader)
{
	m_pShader = pShader;
}