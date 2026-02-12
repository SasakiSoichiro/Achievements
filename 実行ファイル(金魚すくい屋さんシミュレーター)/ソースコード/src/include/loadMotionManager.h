//***************************************************************
//
// モーション読み込みマネージャ処理 [loadMotionManager.h]
// Author Sasaki Soichiro
//
//****************************************************************
#ifndef _LOADMOTIONMANAGER_H_// このマクロ定義がされていなかったら
#define _LOADMOTIONMANAGER_H_// 二重インクルード防止のマクロ定義

//*******************
// インクルード
//*******************
#include "main.h"
#include "loadMotion.h"

//*************************************
// モーション読み込みマネージャクラス
//*************************************
class CLoadMotionManager
{
public:
	typedef struct
	{
		CModel* apModel[20];
		CLoadMotion::Load apMotion;
		int nNumModel;
	}LoadInfo;
	CLoadMotionManager();		// コンストラクタ
	~CLoadMotionManager();		// デストラクタ
	HRESULT Load(void);			// ロード
	void Unload(void);			// アンロード
	int Register(const char* pFilename);	// モーションテキスト登録
	CLoadMotion::Load GetAddres(int nIdx);	// モーションテキスト情報取得
private:
	// 定数
	static constexpr int nMaxLoad = 16;
	static constexpr int nMaxModel = 20;
	// 構造体
	LoadInfo m_LoadInfo[nMaxLoad];
	// 数値
	static int m_nNumAll;				// 現在のモーションテキスト数
	// 文字
	const char* MOTION[nMaxLoad] = {};	// テクスチャの名前
};


//*************************************
// モーション読み込みマネージャクラス
//*************************************
class CLoadMotionManagerS
{
public:
	typedef struct
	{
		CLoadMotionS::Load load;
		int nNumModel;
	}LoadInfo;
	CLoadMotionManagerS();		// コンストラクタ
	~CLoadMotionManagerS();		// デストラクタ
	int Register(const char* pFilename);	// モーションテキスト登録
	CMotion* GetAddres(int nIdx,CModel** ppModel,int &nModelNum);	// モーションテキスト情報取得
private:
	// 定数
	static constexpr int nMaxMotion = 15;
	static constexpr int nMaxLoad = 16;
	static constexpr int nMaxModel = 20;
	// 構造体
	LoadInfo m_LoadInfo[nMaxLoad];
	// 数値
	static int m_nNumAll;				// 現在のモーションテキスト数
	// 文字
	const char* MOTION[nMaxLoad] = {};	// テクスチャの名前
};

#endif