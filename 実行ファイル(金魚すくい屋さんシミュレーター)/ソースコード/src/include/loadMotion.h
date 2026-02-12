//***************************************
//
//	モーション読み込み処理 [loadMotion.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _LOADMOTION_H_// このマクロ定義がされていなかったら
#define _LOADMOTION_H_// 二重インクルード防止のマクロ定義
//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include <string>
#include "motion.h"

//******************
// 前方宣言
//******************
class CModel;

//**********************
// モーション読み込み
//**********************
class CLoadMotion
{
public:
	typedef struct
	{
		CModel** ppModel;
		int nNumModel;
		CMotion* pMotion;
	}Load;
	CLoadMotion();	// コンストラクタ
	~CLoadMotion();	// デストラクタ
	static void LoadMotion(const char* pMotionScript, Load& load);
private:
	static std::string ignoreComment(const std::string& line);
	static void LoadModelFile(std::ifstream& file, Load& load);
	static void LoadCharacterSet(std::ifstream& file, Load& load);
	static void LoadPartsSet(std::ifstream& file, Load& load);
	static void LoadMotionSet(std::ifstream& file, Load& load);
	static void LoadKeySet(std::ifstream& file, CMotion::KEY_INFO& keyInfo);
	static void LoadKey(std::ifstream& file, CMotion::KEY& key);
};

//**********************
// モーション読み込み
//**********************
class CLoadMotionS
{
public:
	typedef struct
	{
		std::string modelName;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 offsetPos;
		D3DXVECTOR3 rot;
		int nParent;
	}Model;
	typedef struct
	{
		Model ModelInfo[20];
		int nNumModel;
		CMotion::MOTION_INFO MotionInfo[15];
	}Load;
	CLoadMotionS();		// コンストラクタ
	~CLoadMotionS();	// デストラクタ
	static void LoadMotion(const char* pMotionScript, Load& load);
private:
	static std::string ignoreComment(const std::string& line);
	static void LoadModelFile(std::ifstream& file, Load& load);
	static void LoadCharacterSet(std::ifstream& file, Load& load);
	static void LoadPartsSet(std::ifstream& file, Load& load);
	static void LoadMotionSet(std::ifstream& file, Load& load, int nMotionCnt);
	static void LoadKeySet(std::ifstream& file, CMotion::KEY_INFO& keyInfo);
	static void LoadKey(std::ifstream& file, CMotion::KEY& key);
};


#endif