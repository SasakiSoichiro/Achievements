//**********************************
//
// リザルト処理 [result.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "scene.h"
#include "result.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "review.h"
#include "object2D.h"
#include "object3D.h"
#include "money.h"
#include "sound.h"

//*******************
// コンストラクタ
//*******************
CResult::CResult() :CScene(CScene::MODE_RESULT)
{
}

//*******************
// デストラクタ
//*******************
CResult::~CResult()
{
}

//*************
// 初期化処理
//*************
HRESULT CResult::Init(void)
{
	return S_OK;
}

//*************
// 終了処理
//*************
void CResult::Uninit(void)
{
	CObject::Release();
}

//*************
// 更新処理
//*************
void CResult::Update(void)
{
	CCamera* pCamera = CManager::GetCCamera();
	pCamera->Target(VECTOR3_NULL, VECTOR3_NULL);
	// キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetCInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetCInputJoypad();
	if (pInputKeyboard->Trigger(DIK_RETURN)|| pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		CFade::Set(CScene::MODE_TITLE);
	}

}

//**************
// 描画処理
//**************
void CResult::Draw(void)
{

}