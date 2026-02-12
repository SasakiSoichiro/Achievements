//*****************************************
//
// 花火処理 [flowerworks.h]
// Author:Sasaki Soichiro
//
//*****************************************
#ifndef _FLOWERWORKS_H_ // このマクロ定義がされていなかったら
#define _FLOWERWORKS_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//************************
// 花火クラス
//************************
class CFlowerworks :public CObject
{
public:
	typedef enum
	{
		TYPE_ZERO=0,
		TYPE_ONE,
		TYPE_TWO,
		TYPE_MAX
	}TYPE;
	CFlowerworks(int nPriority = 3);	// コンストラクタ
	~CFlowerworks();					// デストラクタ
	static CFlowerworks* Create(D3DXVECTOR3 pos,int type0Num, int type1Num, int type2Num);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Sphere(int nNum,int nType,D3DXCOLOR col);
	void SetPos(D3DXVECTOR3 pos);	// 位置の設定	
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// 位置情報の取得
private:
	// 定数
	static constexpr int nRand = 100;		// ランダムの値
	static constexpr float fAlpha = 1.0f;	// α値
	static constexpr float fRatio = 0.01f;	// 比率
	// 座標
	D3DXVECTOR3 m_pos;	// 位置
	// 数値
	int m_nAngle;		// 向き
	int m_nType0Num;
	int m_nType1Num;
	int m_nType2Num;
};
#endif