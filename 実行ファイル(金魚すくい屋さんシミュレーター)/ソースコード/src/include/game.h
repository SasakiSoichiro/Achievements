//************************************
//
// ゲームシーン処理 [game.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _GAME_H_ // このマクロ定義がされていなかったら
#define _GAME_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "scene.h"
#include <memory>
#include <vector>
#include "state.h"


class CBlockManager;
class CObject3D;			// object3Dへのポインタ
class CMesh;				// メッシュへのポインタ
class CPlayer;				// プレイヤーへのポインタ
class CWave;				// ウェーブへのポインタ
class CSlow;				// スローへのポインタ
class CSnagManager;			// 障害物マネージャーへのポインタ
class CPauseManager;		// ポーズマネージャーへのポインタ
class CRailManager;			// レールマネージャーへのポインタ
class CCustomerManager;		// 客マネージャーへのポインタ
class COrderDesk;			// 注文机
class CBaggingDesk;			// 袋詰め机
class CItemBoxManager;		// アイテムボックスマネージャへのポインタ
class CMoney;
class CGameStateBase;
class CStartAndEnd;
class CTitleUI;
class CTimer;
class CCustomer;
class CGoldFish;
class CTutorialSkip;
class CControllTutorial;
class CPasserbyManager;


//********************
// ゲームシーンクラス
//********************
class CGame:public CScene
{
public:
	// 状態
	typedef enum
	{
		STATE_NONE=0,	// 何もなし
		STATE_NORMAL,	// 通常
		STATE_END,		// 終了
		STATE_MAX
	}STATE;

	// ゲーム内シーン
	typedef enum
	{
		SCENE_TITLE = 0,	// タイトル
		SCENE_TUTORIAL,		// チュートリアル
		SCENE_GAME,			// ゲーム
		SCENE_RESULT,		// リザルト
		SCENE_MAX
	}SCENE;

	CGame();		// コンストラクタ
	~CGame();		// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void ChangeState(CGameStateBase* pState);	// 状態を変える
	D3DXVECTOR3 GetPos(void) { return NONE; }	// 位置情報取得

	// セッター
	static void SetCustomerManager(CCustomerManager* pCustomerM) { m_pCustomerManager = pCustomerM; }
	static void SetScene(SCENE scene) { m_Scene = scene; }
	static void SetState(STATE state) { m_State = state; }
	// ゲッター
	static CObject3D* GetCObject3D(void) { return m_pObject3D; }						
	static CMesh* GetCMesh(void) { return m_pMesh; }									
	static CPlayer* GetCPlayer(void) { return m_pPlayer; }								
	static CBlockManager* GetCBlockManager(void) { return m_pBlockManager; }			
	static COrderDesk* GetCOrderDesk(void) { return m_pOrderDesk; }						
	static CBaggingDesk* GetCBaggingDesk(void) { return m_pBaggingDesk; }				
	static CCustomerManager* GetCCustomerManager(void) { return m_pCustomerManager; }	
	static CItemBoxManager* GetCItemBoxManager(void) { return m_pItemBoxManager; }		
	static SCENE GetScene(void) { return m_Scene; }
	static STATE GetState(void) { return m_State; }	
private:
	// 定数
	static constexpr float deltaTime = 0.016f;
	// ポインタ
	CObject3D* m_pMountain;					// 山
	CPasserbyManager* m_pPasserbyManager;	// 通行人マネージャーへのポインタ
	CStateMachine* m_Machine;				// ステートマシンへのポインタ
	static CObject3D* m_pObject3D;			// object3Dへのポインタ
	static CMesh* m_pMesh;					// メッシュへのポインタ
	static CPlayer* m_pPlayer;				// プレイヤーへのポインタ
	static CWave* m_pWave;					// ウェーブへのポインタ
	static STATE m_State;					// 状態へのポインタ
	static CBlockManager* m_pBlockManager;	// ブロックマネージャへのポインタ
	static CPauseManager* m_pPauseManager;	// ポーズマネージャへのポインタ
	static CCustomerManager* m_pCustomerManager;// 客マネージャへのポインタ
	static COrderDesk* m_pOrderDesk;			// 注文机
	static CBaggingDesk* m_pBaggingDesk;		// 袋詰め机
	static CItemBoxManager* m_pItemBoxManager;	// アイテムボックスマネージャへのポインタ
	static CMoney* m_pMoney;					// お金のUI
	static SCENE m_Scene;
	// 数値
	int m_nCnt;
};

//**************************************
// ゲーム専用のステートの基底クラス
//**************************************
class CGameStateBase :public CStateBase
{
public:
	CGameStateBase();				// コンストラクタ
	~CGameStateBase();				// デストラクタ
	void SetOwner(CGame* pGame);	// プレイヤーのポインタ設定
protected:
	CGame* m_pGame;					// ゲームへのポインタ
};
#endif