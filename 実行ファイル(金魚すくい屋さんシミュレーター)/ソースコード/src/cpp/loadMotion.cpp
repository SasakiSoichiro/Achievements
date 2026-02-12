//***************************************
// 
// モーション読み込み処理[loadMotion.cpp]
// Author:Sasaki Soichiro
// 
//***************************************

//******************
// インクルード
//******************
#include "loadMotion.h"
#include "game.h"
#include <fstream>
#include <string>
#include <sstream>

//*****************
// コンストラクタ
//*****************
CLoadMotion::CLoadMotion()
{
}

//*****************
// デストラクタ
//*****************
CLoadMotion::~CLoadMotion()
{
}

//************************************
// モーションの読み込み
//************************************
void CLoadMotion::LoadMotion(const char* pMotionScript, Load& load)
{
	if (pMotionScript == nullptr)
	{
		return;
	}
	std::ifstream file(pMotionScript);
	std::string line;
	std::string word;

	while (std::getline(file, line))
	{
		line = ignoreComment(line);

		// 空だったら次の行
		if (line.empty())continue;

		std::istringstream iss(line);
		iss >> word;

		if (word == "SCRIPT")
		{// 次の行に行く
			continue;
		}
		else if (word == "NUM_MODEL")
		{
			// モデル数を読み取る
			iss >> word >> load.nNumModel;
			// モデルファイルを読み込む
			LoadModelFile(file, load);
		}
		else if (word == "CHARACTERSET")
		{
			// キャラクターセット
			LoadCharacterSet(file, load);
		}
		else if (word == "MOTIONSET")
		{
			// モーションのインスタンス生成
			if (load.pMotion == nullptr)load.pMotion = CMotion::Create(load.ppModel, load.nNumModel, false);
			// モーションセット
			LoadMotionSet(file, load);
		}
		else if (word == "END_SCRIPT")
		{// テキストファイルの読み取り終了
			break;
		}
	}
}

//**************************
// コメントを無視する処理
//**************************
std::string CLoadMotion::ignoreComment(const std::string& line)
{
	// #を読み取る
	size_t ignore = line.find('#');

	// #を読み取ったら
	if (ignore != std::string::npos)
	{
		// コメント以降切り捨て
		return line.substr(0, ignore);
	}

	// コメントがなかったらそのまま
	return line;
}

//***************************
// モデルファイルの読み込み
//***************************
void CLoadMotion::LoadModelFile(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word, equal, fileName;

	// モデルの数分ループ
	for (int nCnt = 0; nCnt < load.nNumModel;)
	{
		std::getline(file, line);

		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		std::istringstream iss(line);

		iss >> word >> equal >> fileName;

		if (word == "MODEL_FILENAME")
		{// 読み取ったファイルのモデルのインスタンス生成
			load.ppModel[nCnt] = CModel::Create(VECTOR3_NULL, fileName.c_str());
			nCnt++;
		}
	}
}

//******************************
// キャラクターセットの読み込み
//******************************
void CLoadMotion::LoadCharacterSet(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_CHARACTERSET")
		{// キャラクターセット終了
			return;
		}
		else if (word == "NUM_PARTS")
		{// パーツ数読み取り
			iss >> word >> load.nNumModel;
		}
		else if (word == "PARTSSET")
		{// パーツセット
			LoadPartsSet(file, load);
		}
	}
}

//******************************
// パーツセットの読み込み
//******************************
void CLoadMotion::LoadPartsSet(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word;

	int nIdx = 0, nParent = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_PARTSSET")
		{// パーツセット終了
			return;
		}
		else if (word == "INDEX")
		{// インデックス
			iss >> word >> nIdx;
		}
		else if (word == "PARENT")
		{// 親
			iss >> word >> nParent;
			// -1じゃなかったら親を設定する
			if (nParent != -1)load.ppModel[nIdx]->SetParent(load.ppModel[nParent]);
		}
		else if (word == "POS")
		{// 位置
			D3DXVECTOR3 pos = VECTOR3_NULL;
			iss >> word >> pos.x >> pos.y >> pos.z;
			load.ppModel[nIdx]->SetOffSetPos(pos);
			load.ppModel[nIdx]->SetPos(pos);
		}
		else if (word == "ROT")
		{// 向き
			D3DXVECTOR3 rot = VECTOR3_NULL;
			iss >> word >> rot.x >> rot.y >> rot.z;
			load.ppModel[nIdx]->SetRot(rot);
		}
	}
}

//**********************
// モーションの読み込み
//**********************
void CLoadMotion::LoadMotionSet(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word;

	CMotion::MOTION_INFO Info = {};
	int nCntKeySet = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_MOTIONSET")
		{// パーツセット終了
			load.pMotion->SetInfo(Info);
			return;
		}
		else if (word == "LOOP")
		{// ループの有無
			bool bLoop;
			iss >> word >> bLoop;
			Info.bLoop = bLoop;
		}
		else if (word == "NUM_KEY")
		{// キー数
			int nNumKey;
			iss >> word >> nNumKey;
			Info.nNumKey = nNumKey;
		}
		else if (word == "KEYSET")
		{// キーセット
			LoadKeySet(file, Info.aKeyInfo[nCntKeySet]);
			nCntKeySet++;
		}
	}
}

//**********************
// キーセットの読み込み
//**********************
void CLoadMotion::LoadKeySet(std::ifstream& file, CMotion::KEY_INFO& keyInfo)
{
	std::string line;
	std::string word;

	int nCntKey = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_KEYSET")
		{// パーツセット終了
			return;
		}
		else if (word == "FRAME")
		{// フレーム
			int nFrame;
			iss >> word >> nFrame;
			keyInfo.nFlame = nFrame;
		}
		else if (word == "KEY")
		{// キー
			LoadKey(file, keyInfo.aKey[nCntKey]);
			nCntKey++;
		}
	}
}

//**********************
// キーの読み込み
//**********************
void CLoadMotion::LoadKey(std::ifstream& file, CMotion::KEY& key)
{
	std::string line;
	std::string word;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_KEY")
		{// パーツセット終了
			return;
		}
		else if (word == "POS")
		{// 位置
			D3DXVECTOR3 pos = VECTOR3_NULL;
			iss >> word >> pos.x >> pos.y >> pos.z;
			key.fPosX = pos.x;
			key.fPosY = pos.y;
			key.fPosZ = pos.z;
		}
		else if (word == "ROT")
		{// 向き
			D3DXVECTOR3 rot = VECTOR3_NULL;
			iss >> word >> rot.x >> rot.y >> rot.z;
			key.fRotX = rot.x;
			key.fRotY = rot.y;
			key.fRotZ = rot.z;
		}
	}
}


//*****************
// コンストラクタ
//*****************
CLoadMotionS::CLoadMotionS()
{
}

//*****************
// デストラクタ
//*****************
CLoadMotionS::~CLoadMotionS()
{
}

//************************************
// モーションの読み込み
//************************************
void CLoadMotionS::LoadMotion(const char* pMotionScript, Load& load)
{
	if (pMotionScript == nullptr)
	{
		return;
	}
	std::ifstream file(pMotionScript);
	std::string line;
	std::string word;
	int nCntMotion = 0;

	while (std::getline(file, line))
	{
		line = ignoreComment(line);

		// 空だったら次の行
		if (line.empty())continue;

		std::istringstream iss(line);
		iss >> word;

		if (word == "SCRIPT")
		{// 次の行に行く
			continue;
		}
		else if (word == "NUM_MODEL")
		{
			// モデル数を読み取る
			iss >> word >> load.nNumModel;
			// モデルファイルを読み込む
			LoadModelFile(file, load);
		}
		else if (word == "CHARACTERSET")
		{
			// キャラクターセット
			LoadCharacterSet(file, load);
		}
		else if (word == "MOTIONSET")
		{
			// モーションセット
			LoadMotionSet(file, load, nCntMotion);
			nCntMotion++;
		}
		else if (word == "END_SCRIPT")
		{// テキストファイルの読み取り終了
			break;
		}
	}
}

//**************************
// コメントを無視する処理
//**************************
std::string CLoadMotionS::ignoreComment(const std::string& line)
{
	// #を読み取る
	size_t ignore = line.find('#');

	// #を読み取ったら
	if (ignore != std::string::npos)
	{
		// コメント以降切り捨て
		return line.substr(0, ignore);
	}

	// コメントがなかったらそのまま
	return line;
}

//***************************
// モデルファイルの読み込み
//***************************
void CLoadMotionS::LoadModelFile(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word, equal, fileName;

	// モデルの数分ループ
	for (int nCnt = 0; nCnt < load.nNumModel;)
	{
		std::getline(file, line);

		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		std::istringstream iss(line);

		iss >> word >> equal >> fileName;

		if (word == "MODEL_FILENAME")
		{// 読み取ったファイルのモデルのインスタンス生成
			load.ModelInfo[nCnt].modelName = fileName.c_str();
			nCnt++;
		}
	}
}

//******************************
// キャラクターセットの読み込み
//******************************
void CLoadMotionS::LoadCharacterSet(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_CHARACTERSET")
		{// キャラクターセット終了
			return;
		}
		else if (word == "NUM_PARTS")
		{// パーツ数読み取り
			iss >> word >> load.nNumModel;
		}
		else if (word == "PARTSSET")
		{// パーツセット
			LoadPartsSet(file, load);
		}
	}
}

//******************************
// パーツセットの読み込み
//******************************
void CLoadMotionS::LoadPartsSet(std::ifstream& file, Load& load)
{
	std::string line;
	std::string word;

	int nIdx = 0, nParent = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_PARTSSET")
		{// パーツセット終了
			return;
		}
		else if (word == "INDEX")
		{// インデックス
			iss >> word >> nIdx;
		}
		else if (word == "PARENT")
		{// 親
			iss >> word >> nParent;
			// -1じゃなかったら親を設定する
			if (nParent != -1)load.ModelInfo[nIdx].nParent = nParent;
		}
		else if (word == "POS")
		{// 位置
			D3DXVECTOR3 pos = VECTOR3_NULL;
			iss >> word >> pos.x >> pos.y >> pos.z;
			load.ModelInfo[nIdx].offsetPos=pos;
			load.ModelInfo[nIdx].pos = pos;
		}
		else if (word == "ROT")
		{// 向き
			D3DXVECTOR3 rot = VECTOR3_NULL;
			iss >> word >> rot.x >> rot.y >> rot.z;
			load.ModelInfo[nIdx].rot = rot;
		}
	}
}

//**********************
// モーションの読み込み
//**********************
void CLoadMotionS::LoadMotionSet(std::ifstream& file, Load& load, int nMotionCnt)
{
	std::string line;
	std::string word;

	CMotion::MOTION_INFO Info = {};
	int nInfoNum = 0;
	int nCntKeySet = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_MOTIONSET")
		{// パーツセット終了
			load.MotionInfo[nMotionCnt] = Info;
			return;
		}
		else if (word == "LOOP")
		{// ループの有無
			bool bLoop;
			iss >> word >> bLoop;
			Info.bLoop = bLoop;
		}
		else if (word == "NUM_KEY")
		{// キー数
			int nNumKey;
			iss >> word >> nNumKey;
			Info.nNumKey = nNumKey;
		}
		else if (word == "KEYSET")
		{// キーセット
			LoadKeySet(file, Info.aKeyInfo[nCntKeySet]);
			nCntKeySet++;
		}
	}
}

//**********************
// キーセットの読み込み
//**********************
void CLoadMotionS::LoadKeySet(std::ifstream& file, CMotion::KEY_INFO& keyInfo)
{
	std::string line;
	std::string word;

	int nCntKey = 0;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_KEYSET")
		{// パーツセット終了
			return;
		}
		else if (word == "FRAME")
		{// フレーム
			int nFrame;
			iss >> word >> nFrame;
			keyInfo.nFlame = nFrame;
		}
		else if (word == "KEY")
		{// キー
			LoadKey(file, keyInfo.aKey[nCntKey]);
			nCntKey++;
		}
	}
}

//**********************
// キーの読み込み
//**********************
void CLoadMotionS::LoadKey(std::ifstream& file, CMotion::KEY& key)
{
	std::string line;
	std::string word;

	while (std::getline(file, line))
	{
		// コメントを無視する
		line = ignoreComment(line);
		// 空だったら次の行
		if (line.empty())continue;

		// 単語を読み取る処理
		std::istringstream iss(line);
		iss >> word;

		if (word == "END_KEY")
		{// パーツセット終了
			return;
		}
		else if (word == "POS")
		{// 位置
			D3DXVECTOR3 pos = VECTOR3_NULL;
			iss >> word >> pos.x >> pos.y >> pos.z;
			key.fPosX = pos.x;
			key.fPosY = pos.y;
			key.fPosZ = pos.z;
		}
		else if (word == "ROT")
		{// 向き
			D3DXVECTOR3 rot = VECTOR3_NULL;
			iss >> word >> rot.x >> rot.y >> rot.z;
			key.fRotX = rot.x;
			key.fRotY = rot.y;
			key.fRotZ = rot.z;
		}
	}
}
