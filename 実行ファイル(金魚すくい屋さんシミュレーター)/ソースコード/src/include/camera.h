//***************************************
//
// カメラ処理 [camera.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _CAMERA_H_//このマクロ定義がされていなかったら
#define _CAMERA_H_//２重インクルード防止のマクロ定義
//**************
// インクルード
//**************
#include "main.h"

//**************
// カメラクラス
//**************
class CCamera
{
public:
	typedef enum
	{
		TYPE_TARGET=0,
		TYPE_TITLE,
		TYPE_GAME,
		TYPE_RESULT,
		TYPE_FREE
	}TYPE;
	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ
	void Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void SetCamera(void);	// カメラの設定
	void Mode(void);
	void Target(D3DXVECTOR3 pos,D3DXVECTOR3 rot);	// 映すターゲットの設定
	void BackTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot);// 後ろに進む場合の設定
	void InitTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot);// カメラの初期化
	void InitTargetDest(D3DXVECTOR3 pos, D3DXVECTOR3 rot);// カメラの初期化
	void JumpTarget(D3DXVECTOR3 pos,float fAngle);	// 空中にいる場合の設定
	void Fixed(D3DXVECTOR3 pos);					// 固定カメラの設定
	void SetVandR(void);							// 視点と注視点の設定
	void MoveMouseR(void);							// マウスで注視点を動かす
	void MoveMouseV(void);							// マウスで視点を動かす
	void Shake(void);								// カメラを揺らす
	void SetShake(int nTime);						// カメラを揺らす時間を設定する
	void SetType(int nType) { m_nType = nType; }	// カメラのタイプを決める

	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetProjMtx(void){ return m_mtxProjection; }
	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
private:
	// 定数
	static constexpr float ofsetV = 90.0f;		// プレイヤーから視点までの距離
	static constexpr float ofsetBackV = 90.0f;	// プレイヤーから視点までの距離(後ろ向きに進んでいるとき)
	static constexpr float ofsetR = 50.0f;		// プレイヤーから視点までの距離
	static constexpr float dest = 0.02f;		// 目標に到達するまでの遅延
	static constexpr float speed = 2.0f;		// カメラ操作時のスピード
	static constexpr float sensitivity = 0.009f;// マウス感度
	static constexpr float shakeDistance = 1.5f;// カメラ揺れの動く距離
	static constexpr float FOV = 70.0f;			// 視野角
	static constexpr float nearScreen = 10.0f;	// 描画距離(近く)
	static constexpr float farScreen = 10000.0f;// 描画距離(遠く)
	// 座標
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	// 数値
	int m_nShakeTime;			// カメラが揺れている時間
	int m_nType;				// タイプ
	float m_fDistance;			// 視点から注視点の距離
};
#endif