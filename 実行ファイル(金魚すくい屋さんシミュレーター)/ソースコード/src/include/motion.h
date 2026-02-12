//********************************************
//
// モーション処理 [motion.h]
// Author:Sasaki Soichiro
//
//********************************************
#ifndef _MOTION_H_// このマクロ定義がされていなかったら
#define _MOTION_H_// 2重インクルード防止のマクロ定義

//*****************
// インクルード
//*****************
#include "main.h"
#include "model.h"

//*****************
// マクロ定義
//*****************
#define PARTS_MAX (20)// パーツ数の最大
#define MAX_WORD (256)// 読み込み時の文字数

//*******************
// モーションクラス
//*******************
class CMotion
{
public:
	//**************
// キーの構造体
//**************
	typedef struct
	{
		float fPosX;// 位置
		float fPosY;
		float fPosZ;
		float fRotX;// 向き
		float fRotY;
		float fRotZ;
	}KEY;

	//******************
	// キー情報の構造体
	//******************
	typedef struct
	{
		int nFlame;		// 再生フレーム
		KEY aKey[20];	// パーツの最大数を入れる//キー情報

	}KEY_INFO;

	//***********************
	//モーション情報の構造体
	//***********************
	typedef struct
	{
		bool bLoop;				// ループするかどうか
		int nNumKey;			// キーの総数
		KEY_INFO aKeyInfo[30];	// キーの情報
	}MOTION_INFO;

	CMotion();		// コンストラクタ
	~CMotion();		// デストラクタ
	static CMotion*Create(CModel** ppModel, int nNumModel,bool bSlow);	// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetInfo(MOTION_INFO motionInfo);		// モーション情報の設定
	void SetMotion(int type);					// モーションの設定
	// ゲッター
	MOTION_INFO GetMotionInfo(int nNum) { return m_aInfo[nNum]; }
	bool GetFinish(void) { return m_bFinish; }	// モーションが終わったかどうか
	int GetType(void) { return m_type; }		// 種類情報取得
	float Normalize(float fData0, float fData1);// 正規化
private:
	// 定数
	static constexpr int nMaxMotionInfo = 15;
	static constexpr int nBlendFrame = 20;
	// ポインタ
	CModel** m_ppModel;	// モデルのポインタ
	// 構造体
	MOTION_INFO m_aInfo[nMaxMotionInfo];// モーションの最大数
	// フラグ
	bool m_bLoop;	// ループするかどうか
	bool m_bFinish;	// 現在のモーションが終了しているかどうか
	bool m_bBlend;	// ブレンドモーションがあるかどうか
	bool m_bSlow;	// スローになるか
	// 数値
	int m_nNumModel;		// モデルのパーツ総数
	int m_nNum;				// モーションの総数
	int m_type;				// モーションの種類
	int m_nNumKey;			// キーの総数
	int m_nKey;				// 現在のキーNo.
	int m_nCnt;				// モーションカウンター
	int m_typeBlend;		// モーションの種類
	int m_nNumKeyBlend;		// キーの総数
	int m_nKeyBlend;		// 現在のキーNo.
	int m_nCntBlend;		// モーションカウンター
	int m_nFrameBlend;		// ブレンドのフレーム数
	int m_nCntMotionBlend;	// モーションカウンター
};

#endif