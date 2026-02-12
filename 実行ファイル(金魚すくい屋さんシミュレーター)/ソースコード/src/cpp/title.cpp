//**********************************
//
// タイトル処理 [title.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "scene.h"
#include "title.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "object3D.h"

//*******************
// コンストラクタ
//*******************
CTitle::CTitle() :CScene(CScene::MODE_TITLE)
{
}

//*******************
// デストラクタ
//*******************
CTitle::~CTitle()
{
}

//*************
// 初期化処理
//*************
HRESULT CTitle::Init(void)
{
	CSound* pSound = CManager::GetCSound();
	pSound->Play(CSound::LABEL_BGM00);
	// カメラの初期化
	CManager::GetCCamera()->SetType(CCamera::TYPE_TITLE);

	return S_OK;
}

//*************
// 終了処理
//*************
void CTitle::Uninit(void)
{
	CObject::Release();
}

//*************
// 更新処理
//*************
void CTitle::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetCInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetCInputJoypad();
	if (pInputKeyboard->Trigger(DIK_RETURN)==true||pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_START)==true)
	{
		CFade::Set(CScene::MODE_GAME);
	}
}

//**************
// 描画処理
//**************
void CTitle::Draw(void)
{

}