//***********************************
//
// 釣り糸処理 [fishingLine.cpp]
// Author:Sasaki Soichiro
//
//***********************************
//*******************
// インクルード
//*******************
#include "fishingLine.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "itemBoxManager.h"
#include "game.h"
#include "sound.h"

// 定数
namespace FishingLine
{
	D3DXVECTOR3 NOR = { 0.0f,1.0f,0.0f };
	D3DXCOLOR INITCOLOR = { 0.0f,0.0f,0.0f,1.0f };
}

//*******************
// コンストラクタ
//*******************
CFishingLine::CFishingLine(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_hookPos = VECTOR3_NULL;
	m_hookPosDest = VECTOR3_NULL;
	m_mtxWorld = {};
	m_bHookAnim = false;
	m_bFishingItem = false;
	m_bFishingCancel = false;
	m_nHaveItemCnt = 0;
	m_fHookAnim = 0.0f;
	m_State = STATE_NONE;
	m_nDir = 0;
	m_nEndFrame = 0;
	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}
	m_nIdxTexture = -1;
}

//*******************
// デストラクタ
//*******************
CFishingLine::~CFishingLine()
{

}

//*******************
// インスタンス生成
//*******************
CFishingLine* CFishingLine::Create(D3DXVECTOR3 pos)
{
	CFishingLine* pObject3D;
	CTexture* pTexture = CManager::GetCTexture();

	pObject3D = new CFishingLine;
	pObject3D->SetPos(pos);
	pObject3D->Init();

	return pObject3D;
}

//*******************
// 初期化
//*******************
HRESULT CFishingLine::Init(void)
{
	m_hookPos = GetPos();

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

	//頂点座標の更新
	pVtx[0].pos = m_pos;
	pVtx[1].pos = m_pos;
	pVtx[2].pos = m_pos;
	pVtx[3].pos = m_pos;

	// 法線の設定
	pVtx[0].nor = FishingLine::NOR;
	pVtx[1].nor = FishingLine::NOR;
	pVtx[2].nor = FishingLine::NOR;
	pVtx[3].nor = FishingLine::NOR;

	//頂点カラーの設定　　　　
	pVtx[0].col = FishingLine::INITCOLOR;
	pVtx[1].col = FishingLine::INITCOLOR;
	pVtx[2].col = FishingLine::INITCOLOR;
	pVtx[3].col = FishingLine::INITCOLOR;

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
// 終了処理
//*******************
void CFishingLine::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	CObject::Release();
}

//*******************
// 更新処理
//*******************
void CFishingLine::Update(void)
{
	// 取得と距離計算
	CItemBoxManager* pItemBoxManager = CGame::GetCItemBoxManager();
	D3DXVECTOR3 startPos=CGame::GetCItemBoxManager()->GetItemLineLeftPos();
	D3DXVECTOR3 endPos = CGame::GetCItemBoxManager()->GetItemLineRightPos();
	D3DXVECTOR3 dir = endPos - startPos;
	bool bInput = CManager::GetCInputKeyboard()->Trigger(DIK_RETURN) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X);
	if (m_bHookAnim)
	{
		Anim();
		pItemBoxManager->PickUp(m_hookPos);
		if (bInput && m_nHaveItemCnt< nMaxItemNum)
		{// アイテムを制限個数持ってなかったら通る処理
			CItem* pItem = nullptr;
			pItem = pItemBoxManager->Pick(m_hookPos);
			if (pItem)
			{// アイテムを拾えたら持っている個数をインクリメント
				m_pItem[m_nHaveItemCnt]= pItem;
				m_nHaveItemCnt++;
				// 音と釣りとモーション
				CSound* pSound = CManager::GetCSound();
				pSound->Play(CSound::LABEL_SE08);
			}
		}
	}
	else
	{
		m_hookPos = m_pos;
		m_hookPosDest = m_pos;
	}
	
	m_hookPos += (m_hookPosDest - m_hookPos) * fHookRatio;

	// アイテムを持っていたら釣り針の位置に合わせる
	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{
		if (m_pItem[nCnt] != nullptr)
		{
			m_pItem[nCnt]->SetPos(D3DXVECTOR3(m_hookPos.x+ fHookPosOffsetX *nCnt, m_hookPos.y, m_hookPos.z));
		}
	}

	SetVtx();
}

//*******************
// 描画処理
//*******************
void CFishingLine::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, nullptr);
	pDevice->SetTexture(1, nullptr);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*******************
// アイテムを消す
//*******************
void CFishingLine::Clear(void)
{
	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}
	m_nHaveItemCnt = 0;
}

//*********************
// アニメーション処理
//*********************
void CFishingLine::Anim(void)
{
	// 情報取得
	CItemBoxManager* pItemBoxManager = CGame::GetCItemBoxManager();
	D3DXVECTOR3 startPos = CGame::GetCItemBoxManager()->GetItemLineLeftPos();
	D3DXVECTOR3 endPos = CGame::GetCItemBoxManager()->GetItemLineRightPos();
	
	float fHookAnim = 0.0f;
	float fHookDir = 1.0f;
	float fChangeTime = 1.0f;

	if (m_nDir == DIRTYPE_ORDER)
	{// 左から右
		fHookAnim = -m_fHookAnim;
		fHookDir = 1.0f;
	}
	else if (m_nDir == DIRTYPE_BAGGING)
	{// 右から左
		fHookAnim = m_fHookAnim;
		fHookDir = -1.0f;
		D3DXVECTOR3 hold = endPos;
		endPos = startPos;
		startPos = hold;

	}
	else
	{
		// 状態を瞬時に変える
		fChangeTime = 0.0f;
	}

	D3DXVECTOR3 dir = endPos - startPos;

	// 状態ごとの処理
	switch (m_State)
	{
	case STATE_START:
		// 始まり
		StateStartAnim(fHookDir, fHookAnim, fChangeTime);
		break;
	case STATE_HOOKLINE:
		// 途中
		StateHookLineAnim(startPos, dir);
		break;
	case STATE_END:
		// 終わり
		StateEndAnim();
		break;
	}
}

//************************************
// アニメーションをスタートさせる処理
//************************************
void CFishingLine::MoveTrigger(int nDirType)
{
	m_bHookAnim = true;
	m_nDir = nDirType;
	m_State = STATE_START;
}

//**********************
// ポリゴンの頂点設定
//**********************
void CFishingLine::SetVtx(void)
{
	D3DXVECTOR3 dir;
	D3DXVECTOR3 width;
	D3DXVECTOR3 up = FishingLine::NOR;

	// 進行方向
	dir = m_hookPos - m_pos;
	m_rot.y = atan2f(dir.x, dir.z);

	// 横を求める
	D3DXVec3Normalize(&dir, &dir);
	D3DXVec3Cross(&width, &up, &dir);
	D3DXVec3Normalize(&width, &width);

	VERTEX_3D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 手元と釣り針の位置をポリゴンでつなぐ
	pVtx[0].pos = m_pos + width * fHookWidth;
	pVtx[1].pos = m_pos - width * fHookWidth;
	pVtx[2].pos = m_hookPos + width * fHookWidth;
	pVtx[3].pos = m_hookPos - width * fHookWidth;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//*******************************
// スタート状態のアニメーション
//*******************************
void CFishingLine::StateStartAnim(float fHookDir, float fHookAnim, float fChangeTime)
{
	m_hookPos.x = m_pos.x + fHookDir * sinf(fHookAnim) * fRadius;
	m_hookPos.z = m_pos.z + fHookDir * cosf(fHookAnim) * fRadius;
	m_hookPosDest = m_hookPos;

	m_fHookAnim += fAnimStartSpeed;
	if (m_fHookAnim > fChangeTime)
	{
		m_State = STATE_HOOKLINE;
		m_fHookAnim = 0.0f;
	}
}

//****************************************
// 釣り糸が動いている状態のアニメーション
//****************************************
void CFishingLine::StateHookLineAnim(D3DXVECTOR3 startPos, D3DXVECTOR3 dir)
{
	// 入力受付
	bool bInput = CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) 
				|| CManager::GetCInputKeyboard()->Trigger(DIK_RSHIFT);

	m_bFishingCancel = bInput;
	m_fHookAnim += fAnimSpeed;
	// 釣り針の目標位置を決める
	m_hookPosDest = startPos + (dir * m_fHookAnim);

	if (m_fHookAnim > 1.0f || bInput)
	{// 0.0f～1.0fのアニメーション
		m_State = STATE_END;
		m_fHookAnim = 0.0f;
	}
}

//*******************************
// エンド状態のアニメーション
//*******************************
void CFishingLine::StateEndAnim(void)
{
	// 目標位置を手元にする
	m_hookPosDest = m_pos;
	// 完全にアニメーションが終わるまで余裕を持たせる
	m_nEndFrame++;
	if (m_nEndFrame > nEndFrame)
	{// 決めたフレーム以上になったら通常状態に戻る
		m_nEndFrame = 0;
		m_bHookAnim = false;
		m_State = STATE_NONE;
	}
}