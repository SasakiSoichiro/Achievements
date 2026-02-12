//***************************************
// 
// メッシュ処理[mesh.cpp]
// Author:Sasaki Soichiro
// 
//***************************************

//******************
// インクルード
//******************
#include "mesh.h"
#include "manager.h"
#include <string>
#include <iostream>
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "wave.h"
#include "camera.h"
#include "rStick.h"
#include "sound.h"
#include "shaderManager.h"
#include "texture.h"

// 定数
namespace Mesh
{
	D3DXCOLOR COLOR = { 0.2f, 0.2f, 0.6f, 1.0f };
}

//****************
// コンストラクタ
//****************
CMesh::CMesh(int nPriority) : CObject(nPriority)
{
	m_pTextureWater = nullptr;	// テクスチャのポインタ
	m_pTextureNoise = nullptr;	// テクスチャのポインタ
	m_pVtxBuff = nullptr;	// 頂点バッファ
	m_pIdxBuff = nullptr;	// インデックスバッファ
	m_pShader = nullptr;	// シェーダのポインタ
	m_pos = VECTOR3_NULL;	// 位置
	m_rot = VECTOR3_NULL;	// 向き
	m_size= VECTOR2_NULL;	// 大きさ
	m_mtxWorld = {};		// ワールドマトリックス
	m_nTime = 0;			// 時間
	m_WaveCnt = 0;			// 波のカウント
	m_bWaveTirgger = false;	// 波のトリガー
	m_fTime = 0.0f;			// 時間
	m_fHeightOffset = 0.0f;	// 高さのオフセット
}

//****************
// デストラクタ
//****************
CMesh::~CMesh()
{

}

//*******************
// インスタンス生成
//*******************
CMesh* CMesh::Create(D3DXVECTOR3 pos)
{
	CMesh* pMesh;// メッシュ情報取得
	CTexture* pTexture = CManager::GetCTexture();// テクスチャ情報の取得

	// インスタンス生成
	pMesh = new CMesh;
	// 初期化
	pMesh->SetPos(pos);
	pMesh->Init();

	return pMesh;
}

//**************
// 初期化
//**************
HRESULT CMesh::Init(void)
{
	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXCNT,//(sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(VERTEX_3D) * IDXCNT,//(sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ
	D3DXVECTOR3 pos = GetPos();
	m_fHeightOffset = pos.y;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カウント用変数
	int nCntPOS = 0;
	float Xmesh = (float)(XMESH);
	float Zmesh = (float)(ZMESH);

	// 頂点バッファの設定
	for (int nCntZ = 0; nCntZ < nMeshLoopNumZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nMeshLoopNumX; nCntX++, nCntPOS++)
		{
			float Angle = (PI_HALF / XMESH * nCntX);
			pVtx[nCntPOS].pos = D3DXVECTOR3(pos.x+((MathUtil::Double(OBJ_X) / Xmesh) * nCntX - OBJ_X), 
											pos.y, 
											pos.z + (-1 * (MathUtil::Double(OBJ_Z) / Zmesh) * nCntZ + OBJ_Z));
		}
	}


	for (int nCnt = 0; nCnt < VTXCNT; nCnt++)
	{//各頂点の法線の設定
		pVtx[nCnt].nor = VECTOR3_NULL;
		pVtx[nCnt].col = Mesh::COLOR;
	}

	// カウント用変数
	int nCntTEX = 0;
	for (int nCntZ = 0; nCntZ < nMeshLoopNumZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nMeshLoopNumX; nCntX++, nCntTEX++)
		{
			pVtx[nCntTEX].tex = D3DXVECTOR2((float)nCntX/XMESH, (float)nCntZ / ZMESH);
		}
	}


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// カウント用変数
	int nCntX=0;
	int nCnt = 0;
	int nIdxXMesh = MathUtil::Double(XMESH + 1);
	// 直前の計算結果保存
	std::vector<int> deltaA(IDXCNT, 0);
	std::vector<int> deltaB(IDXCNT, 0);
	// インデックスの設定
	for (nCnt = 0; nCnt < IDXCNT; nCnt++, nCntX++)
	{
		if (nCnt == 0)
		{
			// 初期値設定
			deltaA[nCnt] = MathUtil::Double(XMESH) + nStartXMeshOffset;
			deltaB[nCnt] = -XMESH - 1;
			pIdx[nCnt] = XMESH + 1;
		}
		else
		{
			// Aは符号反転
			deltaA[nCnt] = -1 * (deltaA[nCnt - 1]);
			// Bは前回A分だけ進める
			deltaB[nCnt] = deltaB[nCnt - 1] + deltaA[nCnt - 1];

			if (nCntX == nIdxXMesh)
			{// 行末
				// 同じインデックスを維持
				pIdx[nCnt] = pIdx[nCnt - 1];
			}
			else if (nCntX == nIdxXMesh + 1)
			{// 次の行の開始位置へジャンプ
				pIdx[nCnt] = pIdx[nCnt - 2] + deltaB[nCnt - 2];
			}
			else if (nCntX == nIdxXMesh + 2)
			{// 行切り替え完了
				// カウントをリセット
				nCntX = 0;
				pIdx[nCnt] = pIdx[nCnt - 1];
			}
			else
			{// 通常進行
				pIdx[nCnt] = pIdx[nCnt - 1] + deltaB[nCnt - 1];
			}
		}
	}
	//頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

	CShaderManager* pShader = CManager::GetShaderManager();
	m_pShader = pShader->GetAddres(pShader->Register("data\\SHADER\\water.hlsl"));

	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ
	m_pTextureWater = pTexture->GetAddres(pTexture->Register("data\\TEXTURE\\water.png"));
	m_pTextureNoise = pTexture->GetAddres(pTexture->Register("data\\TEXTURE\\waterNoise.png"));
	m_fTime = 0.0f;

	m_size = D3DXVECTOR2(OBJ_X, OBJ_Z);	// 大きさ

	return S_OK;
}

//**************
// 終了処理
//**************
void CMesh::Uninit(void)
{
	// 波紋処理の破棄
	for (auto wave = m_pWave.begin(); wave != m_pWave.end();)
	{
		CWave* pWave = *wave;
		pWave->Uninit();
		delete pWave;
		wave = m_pWave.erase(wave);
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
void CMesh::Update(void)
{
	// 法線の更新
	NorUpdate();
	// 波のトリガー判定更新
	WaveTriggerUpdate();
	// 波紋の更新
	WaveUpdate();

	m_fTime += fDeltaTime;
}

//**************
// 描画
//**************
void CMesh::Draw(void)
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
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VTXCNT, 0, POLYCNT);

	if (m_pShader)
	{
		// パス終了
		m_pShader->EndPass();
		// シェーダ終了
		m_pShader->End();
	}
}

//***********************
// シェーダの設定
//***********************
void CMesh::SetShaderParam(void)
{
	D3DXMATRIX mtxView, mtxProj;
	mtxView = CManager::GetCCamera()->GetViewMtx();
	mtxProj = CManager::GetCCamera()->GetProjMtx();

	m_pShader->SetTechnique("Basic");
	m_pShader->SetFloat("g_fTime", m_fTime);
	m_pShader->SetMatrix("g_mtxWld", &m_mtxWorld);
	m_pShader->SetMatrix("g_mtxView", &mtxView);
	m_pShader->SetMatrix("g_mtxProj", &mtxProj);
	m_pShader->SetTexture("g_WaterMap", m_pTextureWater);
	m_pShader->SetTexture("g_NoiseMap", m_pTextureNoise);
}

//***********************
// 法線設定の更新
//***********************
void CMesh::NorUpdate(void)
{
	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 法線の更新
	int nCntPOS = 0;
	for (int nCntZ = 0; nCntZ < nMeshLoopNumZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nMeshLoopNumX; nCntX++, nCntPOS++)
		{
			// 法線設定
			MeshNor(pVtx, nCntX, nCntZ, nCntPOS);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//***********************
// 法線更新時の設定
//***********************
void CMesh::MeshNor(VERTEX_3D* pVtx,int nCntX,int nCntZ,int nCntPOS)
{
	D3DXVECTOR3 norAnswer, nor[nNorNum];

	bool bLeft = (nCntX == 0);		// 左辺
	bool bRight = (nCntX == XMESH);	// 右辺
	bool bUp = (nCntZ == 0);		// 上辺
	bool bDown = (nCntZ == ZMESH);	// 下辺
	// 角の法線計算===================================================
	if (bUp && bLeft)
	{// 左上
		pVtx[nCntPOS].nor = VtxNor(pVtx, nCntPOS, nStrideX, nStrideZ);
		return;
	}
	if (bUp && bRight)
	{// 右上
		pVtx[nCntPOS].nor = VtxNor(pVtx, nCntPOS, nStrideZ, -nStrideX);
		return;
	}
	if (bDown && bLeft)
	{// 左下
		pVtx[nCntPOS].nor = VtxNor(pVtx, nCntPOS, -nStrideZ, nStrideX);
		return;
	}
	if (bDown && bRight)
	{// 右下
		pVtx[nCntPOS].nor = VtxNor(pVtx, nCntPOS, -nStrideX, -nStrideZ);
		return;
	}
	// 各辺の法線計算===================================================
	if (bUp)
	{// 上辺
		nor[0] = VtxNor(pVtx, nCntPOS, nStrideX, nStrideZ);
		nor[1] = VtxNor(pVtx, nCntPOS, nStrideZ, -nStrideX);
	}
	else if (bDown)
	{// 下辺
		nor[0] = VtxNor(pVtx, nCntPOS, -nStrideZ, nStrideX);
		nor[1] = VtxNor(pVtx, nCntPOS, -nStrideX, -nStrideZ);
	}
	else if (bLeft)
	{// 左辺
		nor[0] = VtxNor(pVtx, nCntPOS, nStrideX, nStrideZ);
		nor[1] = VtxNor(pVtx, nCntPOS, -nStrideZ, nStrideX);
	}
	else if (bRight)
	{// 右辺
		nor[0] = VtxNor(pVtx, nCntPOS, -nStrideX, -nStrideZ);
		nor[1] = VtxNor(pVtx, nCntPOS, nStrideZ, -nStrideX);
	}
	else
	{// 中央の法線計算(辺と角以外)
		nor[0] = VtxNor(pVtx, nCntPOS, nStrideX, nStrideZ);
		nor[1] = VtxNor(pVtx, nCntPOS, -nStrideZ, nStrideX);
		nor[2] = VtxNor(pVtx, nCntPOS, -nStrideX, -nStrideZ);
		nor[3] = VtxNor(pVtx, nCntPOS, nStrideZ, -nStrideX);
		// 平均を法線にする
		norAnswer = (nor[0] + nor[1] + nor[2] + nor[3] + NOR_UP) / nNorCenterRatio;
		D3DXVec3Normalize(&norAnswer, &norAnswer);
		pVtx[nCntPOS].nor = norAnswer;
		return;
	}
	// 平均を法線にする(各辺)
	norAnswer = (nor[0] + nor[1]) / nNorSideRatio;
	D3DXVec3Normalize(&norAnswer, &norAnswer);
	pVtx[nCntPOS].nor = norAnswer;
}

//***********************
// 波のトリガー判定の更新
//***********************
void CMesh::WaveTriggerUpdate(void)
{
	if (m_bWaveTirgger)
	{// 波紋の生成
		WaveTrigger(CursorCollision());
	}
}

//***********************
// 波紋の更新
//***********************
void CMesh::WaveUpdate(void)
{
	for (const auto& wave : m_pWave)
	{// 更新
		wave->Update();
	}

	// 波紋の終了処理
	for (auto wave = m_pWave.begin(); wave != m_pWave.end();)
	{
		CWave* pWave = *wave;
		if (!pWave->GetIsLife())
		{// 死んでたら消す
			pWave->Uninit();
			delete pWave;
			wave = m_pWave.erase(wave);
		}
		else
		{// 生きてたらポインタを次に
			wave++;
		}
	}
}

//***********************
// 波紋の発生
//***********************
void CMesh::WaveTrigger(D3DXVECTOR3 pos)
{
	// メッシュの上じゃ無かったら返す
	if (pos.x > m_pos.x + OBJ_X || pos.x < m_pos.x - OBJ_X
		|| pos.z > m_pos.z + OBJ_Z || pos.z < m_pos.z - OBJ_Z)
	{
		m_bWaveTirgger = false;
		return;
	}

	// 位置を正の数にする
	D3DXVECTOR2 TriggerPos;
	TriggerPos.x = (pos.x - m_pos.x) + OBJ_X;
	TriggerPos.y = (m_pos.z- pos.z) + OBJ_Z;

	// uv座標の計算
	D3DXVECTOR2 uv;
	uv.x = TriggerPos.x / MathUtil::Double(OBJ_X);
	uv.y = TriggerPos.y / MathUtil::Double(OBJ_Z);

	// 波紋の生成
	WAVE wave;
	wave.pMeshVtxBuff = m_pVtxBuff;
	wave.nXMesh = XMESH;
	wave.nZMesh = ZMESH;
	wave.size = D3DXVECTOR2(OBJ_X, OBJ_Z);
	wave.uv = uv;
	wave.fHeightOffset = m_fHeightOffset;
	m_pWave.push_back(CWave::Create(wave));
	CSound* pSound = CManager::GetCSound();
	pSound->Play(CSound::LABEL_SE02);
}

//******************************************
// カーソルと地面当たり判定
//******************************************
D3DXVECTOR3 CMesh::CursorCollision(void)
{
	D3DXVECTOR3 pos;

	// レイの方向と原点の取得
	D3DXVECTOR3 rayDir = CManager::GetCInputMouse()->GetCursorRay();
	// 逆行列からレイの原点を求める
	D3DXMATRIX viewMtx = CManager::GetCCamera()->GetViewMtx();
	D3DXMATRIX invViewMtx;
	D3DXMatrixInverse(&invViewMtx, NULL, &viewMtx);
	D3DXVECTOR3 rayOrigin = CManager::GetCCamera()->GetPosV();

	if (fabsf(rayDir.y) < fParallel)
	{// 平行だったら返す
		return VECTOR3_NULL;
	}

	// 原点から当たったとこまでの距離
	float fDistance = (m_pos.y - rayOrigin.y) / rayDir.y;
	if (fDistance < 0.0f)
	{
		return VECTOR3_NULL;
	}
	rayDir.z = rayDir.z;
	// レイが当たった位置
	pos = rayOrigin + (rayDir * fDistance);

	return pos;
}

//*****************
// 頂点の法線計算
//*****************
D3DXVECTOR3 CMesh::VtxNor(VERTEX_3D* pVtx, int nCount0, int nStride0, int nStride1)
{
	D3DXVECTOR3 vec1, vec2, nor;

	// ベクトル1の計算
	vec1 = pVtx[nCount0 + nStride0].pos - pVtx[nCount0].pos;
	// ベクトル2の計算
	vec2 = pVtx[nCount0 + nStride1].pos - pVtx[nCount0].pos;
	// 外積で法線を求める
	D3DXVec3Cross(&nor, &vec1, &vec2);
	// 正規化
	D3DXVec3Normalize(&nor, &nor);

	return nor;
}
