//**************************************
//
// 金魚処理 [goldfish.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _GOLDFISH_H_ // このマクロ定義がされていなかったら
#define _GOLDFISH_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

class CBowl;

//********************
// 金魚クラス
//********************
class CGoldFish :public CObjectX
{
public:
	typedef enum
	{
		FEELING_STRAIGHT=0,
		FEELING_FULLSPEED,
		FEELING_RIGHT,
		FEELING_LEFT,
		FEELING_STOP,
		FEELING_TURN,
		FEELING_MAX
	}FEELING;
	typedef enum
	{
		ESCAPE_RIGHT,
		ESCAPE_LEFT,
		ESCAPE_MAX
	}ESCAPE;
	typedef enum
	{
		STATE_AQUARIUM=0,
		STATE_BOWL,
		STATE_MAX
	}STATE;
	typedef enum
	{
		TYPE_RED = 0,
		TYPE_BLACK,
		TYPE_MAX
	}TYPE;


	CGoldFish(int nPriority = 4);	// コンストラクタ
	~CGoldFish();					// デストラクタ
	static CGoldFish* Create(D3DXVECTOR3 pos, int nColorType);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool AquariumCollision(D3DXVECTOR3 pos,D3DXVECTOR3 posOld);	// 水槽との当たり判定
	void Feeling(void);				// 気分を決める
	void IsEscape(void);			// 逃げる判定
	void Escape(void);				// 逃げる
	void Scoop(void);				// すくわれる処理
	bool ScoopArea(void);			// すくわれるエリアにいるか
	void ScoopRand(int nCnt);		// 一定の確率でポイを破る
	void Death(void) { m_bDeath = true; }

	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	void SetBowl(CBowl* pBowl) { m_pBowl = pBowl; }
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
private:
	// 定数
	static constexpr int nPoiRand = 10;
	static constexpr int nPoiOutRand = 7;
	static constexpr int nRandBowlOffset = 6;
	static constexpr int nRandFeeling = 4;
	static constexpr int nMinFeeling = 2;
	static constexpr int nFeelCool = 100;
	static constexpr int nEscapeCnt = 120;
	static constexpr int nMaxChangeFeelRand = 600;
	static constexpr int nChangeFeelRand = 180;
	static constexpr float fBowlDistance = 7.0f;
	static constexpr float fBowlHeight = 7.0f;
	static constexpr float fSpeed = 0.1f;
	static constexpr float fMaxSpeed = 0.3f;
	static constexpr float fScoopAreaOffset = 10.0f;
	static constexpr float fHeight = -0.6f;
	static constexpr float fBowlHeightOffset = 5.0f;
	static constexpr float fAngleRatio = 0.05f;
	static constexpr float fIsEscapeRange = 20.0f;
	static constexpr float fFishSize = 5.0f;
	static constexpr float fRotDelta = 0.01f;
	static constexpr float fStopSpped = 0.01f;
	// ポインタ
	CBowl* m_pBowl;			// ボウル
	// 座標
	D3DXVECTOR3 m_posOld;	// 前フレームの位置
	// enum
	STATE m_State;
	// フラグ
	bool m_bEscape;			// 逃げているか
	bool m_bDeath;
	// 数値
	int m_feeling;			// 気分
	int m_escape;			// 逃げるタイプ
	int m_nEscapeCnt;
	int m_nFeelingCnt;
	int m_nChangeFeelingCnt;
	int m_nColorType;
	int m_nIdxXFile;		// 使うモデルの番号
	float m_fAngle;			// 向き
	float m_fAngleDest;		// 目標の向き
	float m_fSpeed;			// 速度
	float m_fOffsetBowl;	// ボウルの中にいるときのオフセット
};
#endif