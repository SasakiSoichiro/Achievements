//**********************************
//
// マネージャ処理 [manager.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include <time.h>
#include "debugproc.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "mesh.h"
#include "object.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "Xmodel.h"
#include "scene.h"
#include "blurManager.h"
#include "fade.h"
#include "loadMotionManager.h"
#include "shaderManager.h"

//*******************
// 静的メンバ変数
//*******************
CRenderer *CManager::m_pRenderer=NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CSound* CManager::m_pSound = NULL;
CDebugProc* CManager::m_pDebugProc = NULL;
CTexture* CManager::m_pTexture = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CXModel* CManager::m_pXModel = NULL;
CScene* CManager::m_pScene = NULL;
CBlurManager* CManager::m_pBlur = NULL;
CPauseManager* CManager::m_pPauseManager = NULL;
CFade* CManager::m_pFade = NULL;
CLoadMotionManagerS* CManager::m_pLoadMotionManager = NULL;
CShaderManager* CManager::m_pShaderManager = NULL;


//*******************
// コンストラクタ
//*******************
CManager::CManager()
{
}

//*******************
// デストラクタ
//*******************
CManager::~CManager()
{
}

//*************
// 初期化処理
//*************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// インスタンス生成
	m_pRenderer = new CRenderer;
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputMouse = new CInputMouse;
	m_pInputJoypad = new CInputJoypad;
	m_pSound = new CSound;
	m_pDebugProc = new CDebugProc;
	m_pTexture = new CTexture;
	m_pCamera = new CCamera;
	m_pLight = new CLight;
	m_pXModel = new CXModel;
	m_pBlur = new CBlurManager;
	m_pFade = new CFade;
	m_pLoadMotionManager = new CLoadMotionManagerS;
	m_pShaderManager = new CShaderManager;

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pSound->Init(hWnd)))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pTexture->Load()))
	{//初期化処理が失敗した場合
		return-1;
	}
	// 初期化処理
	if (FAILED(m_pXModel->Load()))
	{//初期化処理が失敗した場合
		return-1;
	}

	// 初期化処理
	m_pCamera->Init();
	m_pLight->Init();
	m_pDebugProc->Init();
	m_pFade->Init();

	return S_OK;
}

//*************
// 終了処理
//*************
void CManager::Uninit(void)
{
	//終了処理
	CObject::ReleaseAll();
	if (m_pTexture != NULL)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pLoadMotionManager != NULL)
	{
		delete m_pLoadMotionManager;
		m_pLoadMotionManager = NULL;
	}
	if (m_pXModel != NULL)
	{
		m_pXModel->Unload();
		delete m_pXModel;
		m_pXModel = NULL;
	}
	if (m_pShaderManager != NULL)
	{
		m_pShaderManager->Unload();
		delete m_pShaderManager;
		m_pShaderManager = NULL;
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;

	}
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	if (m_pBlur != NULL)
	{
		m_pBlur->Uninit();
		delete m_pBlur;
		m_pBlur = NULL;
	}
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}


}

//*************
// 更新処理
//*************
void CManager::Update(void)
{
	m_pRenderer->Update();
	m_pInputKeyboard->Update();
	m_pInputMouse->Update();
	m_pInputJoypad->Update();
	m_pCamera->Update();
	m_pBlur->Update();
	m_pFade->Update();

#ifdef _DEBUG
	bool G = m_pInputKeyboard->GetPress(DIK_G);
	if (m_pInputKeyboard->Trigger(DIK_F1) == true)
	{
		CDebugProc::SwitchDebug();
	}
	if (m_pInputKeyboard->Trigger(DIK_P) == true)
	{
	}
	if (G && m_pInputKeyboard->Trigger(DIK_1) == true)
	{
		SetMode(CScene::MODE_TITLE);
	}
	if (G && m_pInputKeyboard->Trigger(DIK_2) == true)
	{
		SetMode(CScene::MODE_GAME);
	}
	if (G && m_pInputKeyboard->Trigger(DIK_3) == true)
	{
		SetMode(CScene::MODE_RESULT);
	}


	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		int nNum = CObject::GetNumPriority(nCntPriority);
		CDebugProc::Print("描画%d番目の数:%d\n", nCntPriority, nNum);
	}

#endif // !_DEBUG

}

//**************
// 描画処理
//**************
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//**************
// シーン設定
//**************
void CManager::SetMode(CScene::MODE mode)
{
	// サウンドの停止
	m_pSound->Stop();
	// フェードの破棄

	// 現在のモードの破棄
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();
		m_pScene = NULL;
	}
	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
	// 新しいモードの生成
	m_pScene = CScene::Create(mode);
}

CScene::MODE CManager::GetMode(void)
{
	CScene::MODE mode = m_pScene->GetMode();
	return mode;
}