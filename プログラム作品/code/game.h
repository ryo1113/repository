//======================================================================================================================
//
// �Q�[������ [game.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"

class CWord;
class CNumber;
class CPlayer;
class CScorePlus;

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define BULLET_TYPE			(2)

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CGame : public CModeBase
{
public:
	CGame() {}
	~CGame() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void ScorePlusUI(D3DXVECTOR3 pos, D3DXCOLOR col, int plus);
	static void PlusUIDelete();
protected:

private:

	enum GAMESTATE
	{
		GAMESTATE_PREPARATION,		// ����
		GAMESTATE_NORMAL,
		GAMESTATE_MAX
	};

	void BulletTypeDisplay();
	void EnemySpring();

	static list<CScorePlus*> m_Scoreplus;
	CWord *m_pWord[BULLET_TYPE];

	CWord *m_pBg;
	CNumber *m_pNumber;
	CPlayer *m_Player;

	GAMESTATE m_State;
	int nCount;
};

#endif