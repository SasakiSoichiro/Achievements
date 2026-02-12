//**********************************
//
// レンダラー処理 [renderer.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "input.h"
#include "manager.h"
#include "blurManager.h"
#include "fade.h"

//*******************
// コンストラクタ
//*******************
CRenderer::CRenderer() 
{
	m_pD3D = NULL;			// Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
	for (int nCnt = 0; nCnt < nMaxMulti; nCnt++)
	{
		m_pTextureMT[nCnt] = NULL;
		m_pRenderMT[nCnt] = NULL;
	}
	m_pZBuffMT = NULL;
	m_viewportMT = {};
	m_vtxBuffMT = NULL;
}
//*******************
// デストラクタ
//*******************
CRenderer::~CRenderer()
{
}
//*******************
// 初期化処理
//*******************
HRESULT CRenderer::Init(HWND hWnd,BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デブスバッファとステンシブルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// Zバッファ:24bit ステンシル:8bit
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行なう)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//DirectX3Dデバイスの生成(描画処理はCPUで行なう)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3Dデバイスに生成(描画処理と頂点処理をCPUで行なう)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//========================
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//========================

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	//テクスチャステージステート
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

	LPDIRECT3DSURFACE9 pRenderDef = nullptr, pZBuffDef = nullptr;

	for (int nCnt = 0; nCnt < nMaxMulti; nCnt++)
	{
		// レンダリングターゲット用テクスチャの生成
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pTextureMT[nCnt],
			NULL);

		// テクスチャレンダリング用インターフェースの生成
		m_pTextureMT[nCnt]->GetSurfaceLevel(0, &m_pRenderMT[nCnt]);

	}

	// テクスチャレンダリング用Zバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pZBuffMT,
		NULL);

	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);
	for (int nCnt = 0; nCnt < nMaxMulti; nCnt++)
	{
		m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[nCnt]);
	}
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//(sizeof(VERTEX_3D)*必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_vtxBuffMT,
		NULL);

	VERTEX_2D* pVtx = 0;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(fPosOffset, fPosOffset, 0.0f);//←右回りで設定する
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH +fPosOffset, fPosOffset, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fPosOffset, SCREEN_HEIGHT + fPosOffset, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH + fPosOffset, SCREEN_HEIGHT + fPosOffset, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定　　　　
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_vtxBuffMT->Unlock();

	return S_OK;
}

//*******************
// 終了処理
//*******************
void CRenderer::Uninit(void)
{
	for (int nCnt = 0; nCnt < nMaxMulti; nCnt++)
	{
		if (m_pTextureMT[nCnt] != NULL)
		{
			m_pTextureMT[nCnt]->Release();
			m_pTextureMT[nCnt] = NULL;
		}

		if (m_pRenderMT[nCnt] != NULL)
		{
			m_pRenderMT[nCnt]->Release();
			m_pRenderMT[nCnt] = NULL;
		}

	}

	if (m_pZBuffMT != NULL)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = NULL;
	}

	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//*******************
// 更新処理
//*******************
void CRenderer::Update(void)
{
	// ポリゴンの更新
	CObject::UpdateAll();
}

//*******************
// 描画処理
//*******************
void CRenderer::Draw(void)
{
	CFade* pFade = CManager::GetFade();
	// 画面のクリア(バックバッファ＆Zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	// 描画処理
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		// ポリゴンの描画
		CObject::DrawAll();
		// フェードの描画
		pFade->Draw();
		CDebugProc::Draw();
		// 描画終了
		m_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//*****************
// デバイスの取得
//*****************
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}