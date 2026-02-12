//**************************************
//
// シーン処理 [scene.cpp]
// Author:Sasaki Soichiro
//
//**************************************
//******************
// インクルード
//******************
#include "scene.h"
#include "game.h"
#include "title.h"
#include "result.h"

//****************
// コンストラクタ
//****************
CScene::CScene(MODE mode) : CObject(0)
{
	m_mode = mode;
	CObject::SetType(CObject::TYPE_SCENE);
}

//****************
// デストラクタ
//****************
CScene::~CScene()
{
}

//********************
// インスタンス生成
//********************
CScene*CScene::Create(MODE mode)
{
	CScene* pScene = NULL;
	switch (mode)
	{
	case MODE_TITLE:
		pScene = new CTitle;
		break;
	case MODE_GAME:
		pScene = new CGame;
		break;
	case MODE_RESULT:
		pScene = new CResult;
		break;
	}
	pScene->Init();
	return pScene;
}