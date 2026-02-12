//**************************************************
// 
// 操作チュートリアル処理 [controllTutorial.h]
// Author:Sasaki Soichiro
//
//**************************************************
#ifndef _CONTROLLTUTORIAL_H_ // このマクロ定義がされていなかったら
#define _CONTROLLTUTORIAL_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object2D.h"
#include "specBase.h"

//***********************************
// チュートリアル進行情報
//***********************************
typedef struct
{
	int nMoveCnt;
	int nFishingCnt;
	int nFishingItemCnt;
	int nFishingCancelCnt;
	int nTapCnt;
	int nFrameCnt;
}TutorialStepInfo;

//***********************************
// コントロールチュートリアルクラス
//***********************************
class CControllTutorial :public CObject2D
{
public:
	typedef enum
	{// フェード状態
		FADE_IN=0,
		FADE_NONE,
		FADE_OUT
	}FADE;

	typedef enum
	{// ステップごとの条件
		TUTORIAL_STEP_MOVE=0,		// 動く
		TUTORIAL_STEP_FISHING,		// 釣り糸を出す
		TUTORIAL_STEP_FISHINGITEM,	// 釣り糸でアイテムを取る
		TUTORIAL_STEP_FISHINGCANCEL,// 釣り糸をキャンセル
		TUTORIAL_STEP_TAP,			// 水面をタッチする
		TUTORIAL_STEP_FRAME,		// 時間
		TUTORIAL_STEP_SELECT,		// 選択
		TUTORIAL_STEP_MAX
	}TUTORIAL_STEP;

	CControllTutorial(int nPriority = 5);	// コンストラクタ
	~CControllTutorial();					// デストラクタ
	static CControllTutorial* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Arrow(int nTextureCnt);
	void Death(void) { m_bDeath = true; }
	void TutorialStepUpdate(void);
	bool TutorialList(int nStepCnt);
	void StepListCountUpdate(void);
	void InitStepInfo(void);
	void Fade(void);
	bool SelectRetry(void);
	void SetPos(D3DXVECTOR3 pos);	// 位置設定
	void SetRot(D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
private:
	// 定数
	static constexpr int nAlphaCnt = 20;
	static constexpr int nChangeCnt = 240;
	static constexpr int nTextureNum = 8;
	static constexpr int nIsStepMoveCnt = 3;
	static constexpr int nIsStepFishingCnt = 3;
	static constexpr int nIsStepFishingItemCnt = 3;
	static constexpr int nIsStepFishingCancelCnt = 3;
	static constexpr int nIsStepTapCnt = 3;
	static constexpr int nIsStepFrameCnt = 240;
	static constexpr int nTutorialStartCnt = 150;
	// enum
	FADE m_fade;
	TutorialStepInfo m_TutorialStep;
	// フラグ
	bool m_bDeath;
	// 数値
	int m_nIdxTexture[nTextureNum];			// 使っているテクスチャの番号
	int m_nCnt;
	int m_nTextureCnt;
	int m_nStep;
	int m_nStartCnt;

};

//***********************************
// チュートリアルスペッククラス
//***********************************
class CTutorialSpec
{
public:
	CTutorialSpec() {}			// コンストラクタ
	virtual~CTutorialSpec() {}	// デストラクタ
	virtual bool IsCheck(const TutorialStepInfo& TS)const = 0;
};

//***********************************
// 動いた回数の判定
//***********************************
class CMoveCountSpec :public CTutorialSpec
{
public:
	CMoveCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CMoveCountSpec() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nMoveCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 釣り糸を出した回数の判定
//***********************************
class CFishingCountSpec :public CTutorialSpec
{
public:
	CFishingCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CFishingCountSpec() {}		// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nFishingCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 釣り糸でアイテムを取った回数の判定
//***********************************
class CFishingItemCountSpec :public CTutorialSpec
{
public:
	CFishingItemCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CFishingItemCountSpec() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nFishingItemCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 釣り糸をキャンセルした回数の判定
//***********************************
class CFishingCancelCountSpec :public CTutorialSpec
{
public:
	CFishingCancelCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CFishingCancelCountSpec() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nFishingCancelCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 水面をタッチした回数の判定
//***********************************
class CTapCountSpec :public CTutorialSpec
{
public:
	CTapCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CTapCountSpec() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nTapCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 時間の判定
//***********************************
class CFrameCountSpec :public CTutorialSpec
{
public:
	CFrameCountSpec(int nCnt) :m_nCnt(nCnt) {}// コンストラクタ
	~CFrameCountSpec() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return TS.nFrameCnt >= m_nCnt;
	}
private:
	int m_nCnt;
};

//***********************************
// 次に進む条件が整った
//***********************************
class CTutorialStep :public CTutorialSpec
{
public:
	CTutorialStep(CTutorialSpec& spec) :m_spec(spec) {}// コンストラクタ
	~CTutorialStep() {}	// デストラクタ
	bool IsCheck(const TutorialStepInfo& TS)const
	{
		return m_spec.IsCheck(TS);
	}
private:
	CTutorialSpec& m_spec;
};
#endif