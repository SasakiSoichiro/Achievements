//**********************************
//
// 釣り糸処理 [fishingLine.h]
// Author:SasakiSoichiro
//
//**********************************
#ifndef _FISHINGLINE_H_// このマクロ定義がされていなかったら
#define _FISHINGLINE_H_// 2重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//*******************
// 前方宣言
//*******************
class CItem;

//**********************
// 釣り糸クラス
//**********************
class CFishingLine :public CObject
{
public:
	typedef enum
	{
		STATE_NONE=0,
		STATE_START,
		STATE_HOOKLINE,
		STATE_END
	}STATE;
	typedef enum
	{
		DIRTYPE_ORDER = 0,
		DIRTYPE_BAGGING,
		DIRTYPE_PLAY,
		DIRTYPE_MAX
	}DIRTYPE;
	CFishingLine(int nPriority = 4);	// コンストラクタ
	~CFishingLine();					// デストラクタ
	static CFishingLine* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	void MoveTrigger(int nDirType);	// 動かすトリガー
	void Clear(void);				// 持っているアイテムをクリアする
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }		// 位置情報
	D3DXVECTOR3 GetRot(void) { return m_rot; }		// 向き情報
	CItem* GetItem(int nCnt) { return m_pItem[nCnt]; }
	bool GetIsHookAnim(void) { return m_bHookAnim; }
	bool GetIsFishingItem(void) { return m_bFishingItem; }
	bool GetIsFishingCancel(void) { return m_bFishingCancel; }
	int GetHaveNum(void) { return m_nHaveItemCnt; }
private:
	// 定数
	static constexpr int nEndFrame = 20;		// 終了時の時間
	static constexpr int nMaxItemNum = 2;		// アイテムの最大数
	static constexpr float fRadius = 100.0f;	// 釣り糸が伸びる範囲
	static constexpr float fAnimSpeed = 0.01f;	// アニメーションのスピード
	static constexpr float fAnimStartSpeed = 0.08f;	// 最初のアニメーションのスピード
	static constexpr float fHookRatio = 0.2f;	// フックの補間率
	static constexpr float fHookWidth = 0.3f;	// フックのポリゴンの幅
	static constexpr float fHookPosOffsetX = 4.0f;// フックについたアイテムのオフセット
	// 関数
	void Anim(void);			// アニメーション
	void SetVtx(void);			// 頂点の設定
	void StateStartAnim(float fHookDir,float fHookAnim,float fChangeTime);	// スタート状態のアニメーション
	void StateHookLineAnim(D3DXVECTOR3 startPos,D3DXVECTOR3 dir);// 釣り糸が動いている状態のアニメーション
	void StateEndAnim(void);	// エンド状態のアニメーション
	// ポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	CItem* m_pItem[nMaxItemNum];		// アイテム
	// 座標
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_hookPos;		// ひっかける位置
	D3DXVECTOR3 m_hookPosDest;	// ひっかける位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR2 m_size;			// 大きさ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	// enum
	STATE m_State;	// 状態
	// フラグ
	bool m_bHookAnim;	// アニメーション用
	bool m_bFishingItem;// 釣り糸でアイテムを取る
	bool m_bFishingCancel;// 釣り糸をキャンセル
	// 数値
	int m_nHaveItemCnt;	// 持っているアイテムの数
	int m_nDir;			// 方向
	int m_nEndFrame;	// 釣りモーションの最後のアニメーション
	int m_nIdxTexture;	// テクスチャのインデックス
	float m_fHookAnim;
};


#endif