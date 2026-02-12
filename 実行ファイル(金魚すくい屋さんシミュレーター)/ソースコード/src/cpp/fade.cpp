//**********************************
//
// フェード処理 [fade.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "fade.h"
#include "manager.h"
#include "renderer.h"

//**********************
// 静的メンバ変数初期化
//**********************
CFade::STATE CFade::m_state = STATE_IN;
CScene::MODE CFade::m_modeNext=CScene::MODE_GAME;

// 定数
namespace FadeInfo
{
	D3DXCOLOR COLOR = { 0.0f,0.0f,0.0f,1.0f };
}

//*****************
// コンストラクタ
//*****************
CFade::CFade()
{
	m_pVtxBuff = NULL;
	m_col = FadeInfo::COLOR;
}

//*****************
// デストラクタ
//*****************
CFade::~CFade()
{
}

//*****************
// 初期化処理
//*****************
HRESULT CFade::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	VERTEX_2D* pVtx;// 頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,// (sizeof(VERTEX_2D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定　　　　
	pVtx[0].col = D3DXCOLOR(m_col);
	pVtx[1].col = D3DXCOLOR(m_col);
	pVtx[2].col = D3DXCOLOR(m_col);
	pVtx[3].col = D3DXCOLOR(m_col);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// モードを設定
	CManager::SetMode(m_modeNext);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*****************
// 更新処理
//*****************
void CFade::Update(void)
{
	VERTEX_2D* pVtx;

	if (m_state != STATE_NONE)
	{
		if (m_state == STATE_IN)
		{// フェードイン状態
			m_col.a -= fAlphaDelta;// ポリゴンを透明にしておく
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_state = STATE_NONE;
			}
		}
		else if (m_state == STATE_OUT)
		{// フェードアウト状態
			m_col.a += fAlphaDelta;
			if (m_col.a >= 0.0f)
			{
				m_col.a = 0.0f;
				m_state = STATE_IN;
				// 次の画面の移行
				CManager::SetMode(m_modeNext);
			}
		}

		// 頂点バッファをロックする
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(m_col);
		pVtx[1].col = D3DXCOLOR(m_col);
		pVtx[2].col = D3DXCOLOR(m_col);
		pVtx[3].col = D3DXCOLOR(m_col);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

}

//*****************
// 描画処理
//*****************
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, NULL);
	pDevice->SetTexture(1, NULL);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*****************
// シーン設定
//*****************
void CFade::Set(CScene::MODE modeNext)
{
	m_state = STATE_OUT;	// フェードアウト状態に
	m_modeNext = modeNext;	// 次の画面を設定
}