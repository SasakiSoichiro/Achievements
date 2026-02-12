//**********************************
//
// 火花処理 [sparkEffect.h]
// Author Sasaki Soichiro
//
//**********************************
#ifndef _SPARKEFFECT_H_//このマクロ定義がされていなかったら
#define _SPARKEFFECT_H_//2重インクルード防止のマクロ定義
//*******************
// インクルード
//*******************
#include "main.h"
#include "objectBillboard.h"

//************************
// 火花クラス
//************************
class CSparkEffect :public CObjectBillboard
{
public:
	typedef enum
	{
		TYPE_ZERO = 0,
		TYPE_ONE,
		TYPE_TWO,
		TYPE_MAX
	}TYPE;
	CSparkEffect(int nPriority = 3);	// コンストラクタ
	~CSparkEffect();					// デストラクタ
	static CSparkEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType);// インスタンス生成

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetCol(D3DXCOLOR col);		// 色の設定
	void SetSize(D3DXVECTOR2 size);	// 大きさの設定
	void SetOffsetPos(D3DXVECTOR3 offsetPos) { m_offsetPos = offsetPos; }	// 向き
	void SetVec(D3DXVECTOR3 vec) { m_vec = vec; }	// 向き
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
	D3DXVECTOR2 GetSize(void);		// 大きさ情報取得
private:
	// 定数
	static constexpr int maxLife = 80;
	static constexpr float fSizeX = 10.0f;
	static constexpr float fSizeY = 10.0f;
	static constexpr float fGravity = 3.0f;
	static constexpr float fSpeed0 = 8.0f;
	static constexpr float fSpeed1 = 11.0f;
	static constexpr float fSpeed2 = 12.0f;
	static constexpr float fSpeedDec = 0.02f;// 速度を減らす
	// 座標
	D3DXVECTOR3 m_offsetPos;	// 花火の中心位置
	D3DXVECTOR3 m_vec;			// 向き
	// 数値
	int m_nLife;				// 寿命
	int  m_nIdxTexture;			// 使っているテクスチャの番号
	int m_nType;
	float m_fSpeed;				// 速さ
};
#endif