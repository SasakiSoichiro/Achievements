//***************************************
// 
// モーション処理[motion.cpp]
// Author:Sasaki Soichiro
// 
//***************************************

//******************
// インクルード
//******************
#include "motion.h"
#include "game.h"

//*****************
// コンストラクタ
//*****************
CMotion::CMotion()
{
	m_ppModel=NULL;
	m_nNumModel=0;			
	for (int nCnt = 0; nCnt < nMaxMotionInfo; nCnt++)
	{
		m_aInfo[nCnt] = {};
	}
	m_nNum=0;
	m_type=0;
	m_typeBlend = 0;
	m_bLoop=false;
	m_nNumKey=0;
	m_nKey=0;
	m_nCnt=0;
	m_nCntBlend = 0;
	m_bBlend = false;
	m_bFinish = false;
	m_nFrameBlend = 0;
	m_nKeyBlend = 0;
	m_nNumKeyBlend = 0;
	m_nCntMotionBlend = 0;
	m_bSlow = false;
}

//*****************
// デストラクタ
//*****************
CMotion::~CMotion()
{
}

//*******************
// インスタンス生成
//*******************
CMotion* CMotion::Create(CModel** ppModel, int nNumModel,bool bSlow)
{
	CMotion* pMotion;
	pMotion = new CMotion;
	pMotion->m_ppModel = ppModel;
	pMotion->m_nNumModel = nNumModel;
	pMotion->m_bSlow = bSlow;
	pMotion->Init();
	return pMotion;
}

//*******************
// 初期化処理
//*******************
HRESULT CMotion::Init(void)
{

	return S_OK;
}

//*******************
// 終了処理
//*******************
void CMotion::Uninit(void)
{
}

//*******************
// 更新処理
//*******************
void CMotion::Update(void)
{
	//答え格納用
	KEY k_Answer = {};
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	float fPosZ = 0.0f;
	float fRotX = 0.0f;
	float fRotY = 0.0f;
	float fRotZ = 0.0f;


	//計算用
	float fDiffMotionPosX;
	float fDiffMotionPosY;
	float fDiffMotionPosZ;
	float fDiffMotionRotX;
	float fDiffMotionRotY;
	float fDiffMotionRotZ;

	float fPosXCurrent;
	float fPosYCurrent;
	float fPosZCurrent;
	float fRotXCurrent;
	float fRotYCurrent;
	float fRotZCurrent;

	float DiffMotionBlendPosX;
	float DiffMotionBlendPosY;
	float DiffMotionBlendPosZ;
	float DiffMotionBlendRotX;
	float DiffMotionBlendRotY;
	float DiffMotionBlendRotZ;

	float fPosXBlend;
	float fPosYBlend;
	float fPosZBlend;
	float fRotXBlend;
	float fRotYBlend;
	float fRotZBlend;

	float fDiffBlendPosX;
	float fDiffBlendPosY;
	float fDiffBlendPosZ;
	float fDiffBlendRotX;
	float fDiffBlendRotY;
	float fDiffBlendRotZ;

	D3DXVECTOR3 ModelPos, ModelRot;
	m_nNumKey = m_aInfo[m_type].nNumKey;
	m_nNumKeyBlend = m_aInfo[m_typeBlend].nNumKey;


	//全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//現在のキー情報代入(ポイント
		KEY pKey = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel];
		//次のキー情報代入(ポイント
		KEY pKeyNext = m_aInfo[m_type].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel];

		//相対値
		float fRateMotion = m_nCnt / ((float)m_aInfo[m_type].aKeyInfo[m_nKey].nFlame);

		//ブレンドするフレーム
		m_nFrameBlend = nBlendFrame;


		//ブレンドモーション処理
		if (m_bBlend == true)
		{//bBlendがtrueだったら


			//==========
			// 変数宣言
			//==========

			// ブレンド先の情報を代入
			// 現在のキー情報代入(ポイント
			KEY pKeyBlend = m_aInfo[m_typeBlend].aKeyInfo[m_nKeyBlend].aKey[nCntModel];								// 現在のキー

			// 次のキー情報代入(ポイント
			KEY pKeyNextBlend = m_aInfo[m_typeBlend].aKeyInfo[(m_nKeyBlend + 1) % m_nNumKeyBlend].aKey[nCntModel];	// 次のキー

			// 相対値
			float fRateMotionBlend = m_nCntMotionBlend / ((float)m_aInfo[m_typeBlend].aKeyInfo[m_nKeyBlend].nFlame);

			// ブレンドの相対値
			float fRateBlend = m_nCntBlend / ((float)m_nFrameBlend);


			// X軸の移動
			//=============================================================================================
			fDiffMotionPosX = pKeyNext.fPosX - pKey.fPosX;							// 次のキーとの差分
			fPosXCurrent = pKey.fPosX + (fDiffMotionPosX * fRateMotion);
			DiffMotionBlendPosX = pKeyNextBlend.fPosX - pKeyBlend.fPosX;
			fPosXBlend = pKeyBlend.fPosX + (DiffMotionBlendPosX * fRateMotionBlend);
			fDiffBlendPosX = fPosXBlend - fPosXCurrent;								// 差分
			fPosX = fPosXCurrent + (fDiffBlendPosX * fRateBlend);					// 求める値
			// X軸の回転
			//=============================================================================================
			fDiffMotionRotX = Normalize(pKeyNext.fRotX, pKey.fRotX);				// 次のキーとの差分
			fRotXCurrent = pKey.fRotX + (fDiffMotionRotX * fRateMotion);
			DiffMotionBlendRotX = pKeyNextBlend.fRotX - pKeyBlend.fRotX;
			fRotXBlend = pKeyBlend.fRotX + (DiffMotionBlendRotX * fRateMotionBlend);
			fDiffBlendRotX = fRotXBlend - fRotXCurrent;								// 差分
			fRotX = fRotXCurrent + (fDiffBlendRotX * fRateBlend);					// 求める値
			//=============================================================================================

			// Y軸の移動
			//=============================================================================================
			fDiffMotionPosY = pKeyNext.fPosY - pKey.fPosY;							// 次のキーとの差分
			fPosYCurrent = pKey.fPosY + (fDiffMotionPosY * fRateMotion);
			DiffMotionBlendPosY = pKeyNextBlend.fPosY - pKeyBlend.fPosY;
			fPosYBlend = pKeyBlend.fPosY + (DiffMotionBlendPosY * fRateMotionBlend);
			fDiffBlendPosY = fPosYBlend - fPosYCurrent;								// 差分
			fPosY = fPosYCurrent + (fDiffBlendPosY * fRateBlend);					// 求める値
			// Y軸の回転
			//=============================================================================================
			fDiffMotionRotY = Normalize(pKeyNext.fRotY, pKey.fRotY);				// 次のキーとの差分
			fRotYCurrent = pKey.fRotY + (fDiffMotionRotY * fRateMotion);
			DiffMotionBlendRotY = pKeyNextBlend.fRotY - pKeyBlend.fRotY;
			fRotYBlend = pKeyBlend.fRotY + (DiffMotionBlendRotY * fRateMotionBlend);
			fDiffBlendRotY = fRotYBlend - fRotYCurrent;								// 差分
			fRotY = fRotYCurrent + (fDiffBlendRotY * fRateBlend);					// 求める値
			//=============================================================================================

			// Z軸の移動
			//=============================================================================================
			fDiffMotionPosZ = pKeyNext.fPosZ - pKey.fPosZ;							// 次のキーとの差分
			fPosZCurrent = pKey.fPosZ + (fDiffMotionPosZ * fRateMotion);
			DiffMotionBlendPosZ = pKeyNextBlend.fPosZ - pKeyBlend.fPosZ;
			fPosZBlend = pKeyBlend.fPosZ + (DiffMotionBlendPosZ * fRateMotionBlend);
			fDiffBlendPosZ = fPosZBlend - fPosZCurrent;								// 差分
			fPosZ = fPosZCurrent + (fDiffBlendPosZ * fRateBlend);					// 求める値
			// Z軸の回転
			//=============================================================================================
			fDiffMotionRotZ = Normalize(pKeyNext.fRotZ, pKey.fRotZ);				// 次のキーとの差分
			fRotZCurrent = pKey.fRotZ + (fDiffMotionRotZ * fRateMotion);
			DiffMotionBlendRotZ = pKeyNextBlend.fRotZ - pKeyBlend.fRotZ;
			fRotZBlend = pKeyBlend.fRotZ + (DiffMotionBlendRotZ * fRateMotionBlend);
			fDiffBlendRotZ = fRotZBlend - fRotZCurrent;								// 差分
			fRotZ = fRotZCurrent + (fDiffBlendRotZ * fRateBlend);					// 求める値
			//=============================================================================================

		}
		else
		{//ブレンド無し

			// キー情報から位置.向きを算出
			// 次のキーとの差分
			fDiffMotionPosX = (pKeyNext.fPosX) - (pKey.fPosX);
			fDiffMotionPosY = (pKeyNext.fPosY) - (pKey.fPosY);
			fDiffMotionPosZ = (pKeyNext.fPosZ) - (pKey.fPosZ);
			fDiffMotionRotX = Normalize(pKeyNext.fRotX, pKey.fRotX);
			fDiffMotionRotY = Normalize(pKeyNext.fRotY, pKey.fRotY);
			fDiffMotionRotZ = Normalize(pKeyNext.fRotZ, pKey.fRotZ);

			//求める値
			fPosX = pKey.fPosX + fDiffMotionPosX * fRateMotion;
			fPosY = pKey.fPosY + fDiffMotionPosY * fRateMotion;
			fPosZ = pKey.fPosZ + fDiffMotionPosZ * fRateMotion;
			fRotX = pKey.fRotX + fDiffMotionRotX * fRateMotion;
			fRotY = pKey.fRotY + fDiffMotionRotY * fRateMotion;
			fRotZ = pKey.fRotZ + fDiffMotionRotZ * fRateMotion;
		}


		//パーツの位置.向きを設定
		D3DXVECTOR3 offsetPos = m_ppModel[nCntModel]->GetOffSetPos();
		//位置
		ModelPos.x = offsetPos.x+fPosX;//オフセット考慮
		ModelPos.y = offsetPos.y+fPosY;//オフセット考慮
		ModelPos.z = offsetPos.z+fPosZ;//オフセット考慮
		m_ppModel[nCntModel]->SetPos(ModelPos);
		D3DXVECTOR3 offsetRot = m_ppModel[nCntModel]->GetOffSetRot();
		//向き
		ModelRot.x =offsetRot.x+fRotX;//オフセット考慮
		ModelRot.y =offsetRot.y+fRotY;//オフセット考慮
		ModelRot.z =offsetRot.z+fRotZ;//オフセット考慮
		m_ppModel[nCntModel]->SetRot(ModelRot);
	}


	if (m_bBlend == true)
	{//ブレンドあり
		if (m_aInfo[m_typeBlend].bLoop == false)
		{//ループモーションじゃない場合
			if (m_nKeyBlend + 1 != m_nNumKeyBlend)

			{//キー総数に達していない場合

				m_nCntMotionBlend++;//再生フレーム数に達したら現在のキーを1つ進める

			}
			else
			{
				SetMotion(0);
			}
		}

		else
		{//ループモーションの場合

			m_nCntMotionBlend += 1;//カウントモーションを1つ進める

		}

		if (m_nCntMotionBlend >= (m_aInfo[m_typeBlend].aKeyInfo[m_nKeyBlend].nFlame))
		{//フレーム数を超えたら

			//カウントモーションを0にする
			m_nCntMotionBlend = 0;

			//現在のキーを1つ進める
			m_nKeyBlend++;

			if (m_nKeyBlend >= m_nNumKeyBlend)
			{//キー総数に達したら

				//現在のキーを0にする
				m_nKeyBlend = 0;
			}
		}

		//ブレンドカウントを1つ進める
		m_nCntBlend++;


		if (m_nCntBlend >= m_nFrameBlend)
		{//ブレンドのフレーム数に達したら

			//初期化＆通常モーションに情報を代入
			m_nCnt = m_nCntMotionBlend;
			m_nCntBlend = 0;
			m_type = m_typeBlend;
			m_nKey = m_nKeyBlend;
			m_bBlend = false;

		}

		if (m_aInfo[m_type].nNumKey - 1 <= m_nKey && m_aInfo[m_type].bLoop == false)
		{
			SetMotion(0);
			m_bFinish = true;
		}
	}


	else
	{//ブレンドなし

		if (m_aInfo[m_type].bLoop == false)
		{//ループモーションじゃない場合

			if (m_nKey + 1 != m_nNumKey)
			{//キー総数に達していない場合

				m_nCnt++;// モーションカウントを1つ進める

			}
			else
			{
				SetMotion(0);
			}
		}


		else
		{//ループモーションの場合

			m_nCnt++;//モーションカウントを1つ進める
		}



		if (m_nCnt >= (m_aInfo[m_type].aKeyInfo[m_nKey].nFlame))
		{//フレーム数を超えたら

			//カウントモーションを0にする
			m_nCnt = 0;

			//現在のキーを1つ進める
			m_nKey++;

			if (m_nKey >= m_nNumKey)
			{//キー総数に達したら

				//現在のキーを0にする
				m_nKey = 0;

			}
		}
	}
}

//*******************
// 描画処理
//*******************
void CMotion::Draw(void)
{

}

//*******************
// モーションの設定
//*******************
void CMotion::SetMotion(int type)
{
	if (m_type != type)
	{// 現在のモーションとちがっていたとき
		m_typeBlend = type;		// 現在のモーションに設定
		m_bBlend = true;		// ブレンドありに設定
		m_nKeyBlend = 0;		// キーリセット
		m_nCntMotionBlend = 0;	// カウントモーションリセット
		m_bFinish = false;
	}
}

void CMotion::SetInfo(MOTION_INFO motionInfo)
{
	m_aInfo[m_nNum] = motionInfo;
	m_nNum++;
}

//****************
// 正規化
//****************
float CMotion::Normalize(float fData0, float fData1)
{
	float fData = fData0 - fData1;

	if (fData > D3DX_PI)
	{
		fData -= PI_DOUBLE;
	}
	else if (fData < -D3DX_PI)
	{
		fData += PI_DOUBLE;
	}

	return fData;
}