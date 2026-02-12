//**********************************
//
// ナンバー処理 [number.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "mathUtil.h"


//*******************
// コンストラクタ
//*******************
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	nNum = 0;
}
//*******************
// デストラクタ
//*******************
CNumber::~CNumber()
{
}

//*******************
// 初期化
//*******************
HRESULT CNumber::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//(sizeof(VERTEX_2D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);

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

//*******************
// 描画処理
//*******************
void CNumber::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(1, NULL);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//*******************
// 終了
//*******************
void CNumber::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*******************
// 数字の設定
//*******************
void CNumber::SetNumber(int nNum)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	float fData = 0.1f;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].tex = D3DXVECTOR2(nNum * fData,0.0f);
	pVertex[1].tex = D3DXVECTOR2(fData + nNum * fData, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(nNum * fData, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(fData + nNum * fData, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//*******************
// 位置の設定
//*******************
void CNumber::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	m_pos = pos;
	// 長さ
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	fRength = MathUtil::Half(fRength);
	// 向き
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//*******************
// 大きさの設定
//*******************
void CNumber::SetSize(D3DXVECTOR2 size)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	m_size = size;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//*******************
// 色の設定
//*******************
void CNumber::SetCol(D3DXCOLOR col)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	//頂点座標の設定
	pVertex[0].col = col;
	pVertex[1].col = col;
	pVertex[2].col = col;
	pVertex[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//**************************************
// 3Dのナンバークラス
//**************************************
//*******************
// コンストラクタ
//*******************
CNumber3D::CNumber3D()
{
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_mtxWorld = {};
	nNum = 0;
}
//*******************
// デストラクタ
//*******************
CNumber3D::~CNumber3D()
{
}

//*******************
// 初期化
//*******************
HRESULT CNumber3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	VERTEX_3D* pVtx;//頂点情報へのポインタ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//(sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-sizeX, 0.0f, sizeY);
	pVtx[1].pos = D3DXVECTOR3(sizeX, 0.0f, sizeY);
	pVtx[2].pos = D3DXVECTOR3(-sizeX, 0.0f, -sizeY);
	pVtx[3].pos = D3DXVECTOR3(sizeX, 0.0f, -sizeY);

	// 法線設定
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

//*******************
// 描画処理
//*******************
void CNumber3D::Draw(void)
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
	pDevice->SetTexture(1, NULL);
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//*******************
// 終了
//*******************
void CNumber3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*******************
// 数字の設定
//*******************
void CNumber3D::SetNumber(int nNum)
{
	VERTEX_3D* pVertex;//頂点情報へのポインタ
	float fData = 0.1f;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].tex = D3DXVECTOR2(nNum * fData, 0.0f);
	pVertex[1].tex = D3DXVECTOR2(fData + nNum * fData, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(nNum * fData, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(fData + nNum * fData, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//*******************
// 大きさの設定
//*******************
void CNumber3D::SetSize(D3DXVECTOR2 size)
{
	VERTEX_3D* pVtx;//頂点情報へのポインタ
	m_size = size;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-sizeX, 0.0f, sizeY);
	pVtx[1].pos = D3DXVECTOR3(sizeX, 0.0f, sizeY);
	pVtx[2].pos = D3DXVECTOR3(-sizeX, 0.0f, -sizeY);
	pVtx[3].pos = D3DXVECTOR3(sizeX, 0.0f, -sizeY);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//*******************
// 色の設定
//*******************
void CNumber3D::SetCol(D3DXCOLOR col)
{
	VERTEX_3D* pVtx;//頂点情報へのポインタ
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

//**************************************
// 記号
//**************************************
//*******************
// コンストラクタ
//*******************
CSymbol::CSymbol()
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}
//*******************
// デストラクタ
//*******************
CSymbol::~CSymbol()
{
}

//*******************
// 初期化
//*******************
HRESULT CSymbol::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//(sizeof(VERTEX_2D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);

	//rhwの設定
	pVertex[0].rhw = 1.0f;
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;

	pVertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点カラーの設定　　　　
	pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*******************
// 描画処理
//*******************
void CSymbol::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//*******************
// 終了
//*******************
void CSymbol::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*******************
// 位置の設定
//*******************
void CSymbol::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	m_pos = pos;
	float fRength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) / 2.0f;
	float fAngle = atan2f(m_size.x, m_size.y);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//*******************
// 大きさの設定
//*******************
void CSymbol::SetSize(D3DXVECTOR2 size)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	m_size = size;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	float sizeX = MathUtil::Half(m_size.x);
	float sizeY = MathUtil::Half(m_size.y);
	//頂点座標の設定
	pVertex[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, 0.0f);//←右回りで設定する
	pVertex[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//**********************
// 色の設定
//**********************
void CSymbol::SetCol(D3DXCOLOR col)
{
	VERTEX_2D* pVertex;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);
	//頂点座標の設定
	//頂点座標の設定
	pVertex[0].col = col;
	pVertex[1].col = col;
	pVertex[2].col = col;
	pVertex[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//**********************
// テクスチャの割り当て
//**********************
void CSymbol::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}