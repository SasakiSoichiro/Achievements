//********************************************************
//
// コントロールチュートリアル処理 [controllTutorial.cpp]
// Author:Sasaki Soichiro
//
//*********************************************************

//*******************
// インクルード
//*******************
#include "controllTutorial.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "arrow.h"
#include "input.h"
#include "game.h"
#include "fishingLine.h"
#include "mesh.h"

// 定数
namespace ArrowPos
{
	const D3DXVECTOR3 ORDER = {0.0f,30.0f,150.0f};
	const D3DXVECTOR3 PLAY = { -80.0f,30.0f,0.0f };
	const D3DXVECTOR3 BAGGING = { 0.0f,30.0f,-150.0f };
	const D3DXVECTOR3 ITEM = { -110.0f,30.0f,0.0f };
}
namespace Vector2
{
	const D3DXVECTOR2 UISize = { 600.0f,160.0f };
}

//*****************
// コンストラクタ
//*****************
CControllTutorial::CControllTutorial(int nPriority) :CObject2D(nPriority)
{
	for (int nCnt = 0; nCnt < nTextureNum; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;
	}
	m_bDeath = false;
	m_nCnt = 0;
	m_nStartCnt = 0;
	m_nStep = 0;
	m_fade = FADE_IN;
	m_nTextureCnt = 0;
	m_TutorialStep.nMoveCnt = 0;
	m_TutorialStep.nFishingCnt = 0;
	m_TutorialStep.nFishingItemCnt = 0;
	m_TutorialStep.nTapCnt = 0;
}

//*****************
// デストラクタ
//*****************
CControllTutorial::~CControllTutorial()
{
}

//******************
// インスタンス生成
//******************
CControllTutorial* CControllTutorial::Create(D3DXVECTOR3 pos)
{
	CControllTutorial* TitleUI;								// ライフのポインタ
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ

	TitleUI = new CControllTutorial;// インスタンス生成
	// テクスチャ
	TitleUI->m_nIdxTexture[0] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial00.png");
	TitleUI->m_nIdxTexture[1] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial01.png");
	TitleUI->m_nIdxTexture[2] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial02.png");
	TitleUI->m_nIdxTexture[3] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial07.png");
	TitleUI->m_nIdxTexture[4] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial03.png");
	TitleUI->m_nIdxTexture[5] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial04.png");
	TitleUI->m_nIdxTexture[6] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial05.png");
	TitleUI->m_nIdxTexture[7] = pTexture->Register("data\\TEXTURE\\GoldFishTutorial06.png");

	// 初期化
	TitleUI->Init();
	TitleUI->SetPos(pos);

	return TitleUI;
}

//*****************
// 初期化処理
//*****************
HRESULT CControllTutorial::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture[0]));

	// 2Dの初期化
	CObject2D::Init();

	// UVの設定
	CObject2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObject2D::SetSize(Vector2::UISize);
	CObject2D::SetRot(VECTOR3_NULL);
	CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CControllTutorial::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//*****************
// 更新処理
//*****************
void CControllTutorial::Update(void)
{
	// リストごとのカウント
	StepListCountUpdate();
	// 次に進む更新
	TutorialStepUpdate();
	m_nStartCnt++;
	if (m_nStartCnt > nTutorialStartCnt)
	{// 次に行くフェード
		Fade();
	}

	// 消す
	if (m_bDeath)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void CControllTutorial::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//*****************
// 矢印を生成する
//*****************
void CControllTutorial::Arrow(int nTextureCnt)
{
	switch (nTextureCnt)
	{
	case TUTORIAL_STEP_FISHING:
		CArrow::Create(ArrowPos::ORDER);
		CArrow::Create(ArrowPos::PLAY);
		CArrow::Create(ArrowPos::BAGGING);
		break;
	case TUTORIAL_STEP_FISHINGITEM:
		CArrow::Create(ArrowPos::ITEM);
		break;
	case TUTORIAL_STEP_TAP:
		CArrow::Create(ArrowPos::PLAY);
		break;
	}
}

//**************************
// チュートリアルステップ
//**************************
void CControllTutorial::TutorialStepUpdate(void)
{
	if (CControllTutorial::TutorialList(m_nStep))
	{// 次のステップに進む
		InitStepInfo();
		m_fade = FADE_OUT;
		m_nStep++;
	}
}

//**************************
// ステップごとの条件
//**************************
bool CControllTutorial::TutorialList(int nStepCnt)
{
	switch (nStepCnt)
	{
	case TUTORIAL_STEP_MOVE:
	{// 動く
		CMoveCountSpec move(nIsStepMoveCnt);
		return move.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_FISHING:
	{// 釣り糸を出す
		CFishingCountSpec fishing(nIsStepFishingCnt);
		return fishing.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_FISHINGITEM:
	{// 釣り糸でアイテムを取る
		CFishingItemCountSpec fishingItem(nIsStepFishingItemCnt);
		return fishingItem.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_FISHINGCANCEL:
	{// 釣り糸をキャンセルする
		CFishingCancelCountSpec fishingCancel(nIsStepFishingCancelCnt);
		return fishingCancel.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_TAP:
	{// 水面をタップする
		CTapCountSpec tap(nIsStepTapCnt);
		return tap.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_FRAME:
	{// 時間経過
		CFrameCountSpec frame(nIsStepFrameCnt);
		return frame.IsCheck(m_TutorialStep);
		break;
	}
	case TUTORIAL_STEP_SELECT:
	{// 選択
		return SelectRetry();
		break;
	}
	}

	return false;
}

//**************************
// StepInfoの初期化
//**************************
void CControllTutorial::InitStepInfo(void)
{
	m_TutorialStep.nMoveCnt = 0;
	m_TutorialStep.nFishingCnt = 0;
	m_TutorialStep.nFishingItemCnt = 0;
	m_TutorialStep.nFishingCancelCnt = 0;
	m_TutorialStep.nTapCnt = 0;
	m_TutorialStep.nFrameCnt = 0;
}

//**************************
// StepListのカウントをする
//**************************
void CControllTutorial::StepListCountUpdate(void)
{
	CPlayer* pPlayer = CGame::GetCPlayer();
	CMesh* pMesh = CGame::GetCMesh();

	if (pPlayer->GetIsMove())
	{// 動いたカウント
		m_TutorialStep.nMoveCnt++;
	}
	if (pPlayer->GetIsFishing())
	{// 釣り糸を出したカウント
		m_TutorialStep.nFishingCnt++;
	}
	if (pPlayer->GetIsFishingItem())
	{// アイテムを取ったカウント
		m_TutorialStep.nFishingItemCnt++;
	}
	if (pPlayer->GetIsFishingCancel())
	{// 釣り糸をキャンセルしたカウント
		m_TutorialStep.nFishingCancelCnt++;
	}
	if (pMesh->GetWaveTrigger())
	{// 水面をタップしたカウント
		m_TutorialStep.nTapCnt++;
	}
	// 時間が経ったら
	m_TutorialStep.nFrameCnt++;
}

//*****************
// フェードの処理
//*****************
void CControllTutorial::Fade(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	D3DXCOLOR col = GetCol();

	switch (m_fade)
	{
	case FADE_IN:
		col.a = 1.0f / nAlphaCnt * m_nCnt;
		SetCol(col);
		m_nCnt++;
		if (m_nCnt > nAlphaCnt)
		{// 不透明になったら終了
			m_nCnt = 0;
			m_fade = FADE_NONE;
			// 矢印置く
			Arrow(m_nTextureCnt);
		}
		break;
	case FADE_NONE:

		break;
	case FADE_OUT:
		col.a = 1.0f - 1.0f / nAlphaCnt * m_nCnt;
		SetCol(col);
		m_nCnt++;

		if (m_nCnt > nAlphaCnt)
		{// 透明になったら終了
			m_nCnt = 0;
			m_nTextureCnt++;
			m_fade = FADE_IN;
			if (m_nTextureCnt < nTextureNum)
			{
				CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture[m_nTextureCnt]));
			}
			else
			{
				Uninit();
				break;
			}
		}
		break;
	}
}

//**************************
// リトライするかを選択する
//**************************
bool CControllTutorial::SelectRetry(void)
{
	CInputKeyboard* pKeyboard = CManager::GetCInputKeyboard();
	CInputJoypad* pJoypad = CManager::GetCInputJoypad();
	if (pKeyboard->Trigger(DIK_RETURN) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		// リセット
		m_nStep = -1;
		m_nTextureCnt = -1;
		return true;
	}
	return false;
}

//*****************
// 位置設定
//*****************
void CControllTutorial::SetPos(D3DXVECTOR3 pos)
{
	// 位置設定
	CObject2D::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CControllTutorial::SetRot(D3DXVECTOR3 rot)
{
	// 向き設定
	CObject2D::SetRot(rot);
}

//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CControllTutorial::GetPos(void)
{
	// 位置情報取得
	return CObject2D::GetPos();
}