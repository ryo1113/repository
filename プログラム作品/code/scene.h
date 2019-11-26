//===========================================================================================================
//
// �S�̂̏��� [scene.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//===========================================================================================================
// �C���N���[�h�t�@�C��
//===========================================================================================================
#include "main.h"

//===========================================================================================================
// ���C�u�����̃����N
//===========================================================================================================

//===========================================================================================================
// �}�N����`
//===========================================================================================================
#define MAX_EFFECT			(512)
#define MAX_BULLET			(3)

#define MAX_2D				(512)

//===========================================================================================================
// �N���X��`
//===========================================================================================================
class CScene
{
public:
	enum OBJECT
	{// �ʂŕ`�悷��
		OBJECT_NONE,
		OBJECT_BULLET,
		OBJECT_EFFECT,

		OBJECT_MAX
	};

	enum OBJTYPE
	{// �����蔻��&�`�揇
		OBJTYPE_NONE,
		OBJTYPE_BG,
		OBJTYPE_EFFECT,
		OBJTYPE_BULLET,
		OBJTYPE_EXPLOSION,
		OBJTYPE_ENEMY,
		OBJTYPE_PLAYER,
		OBJTYPE_UI,

		OBJTYPE_MAX
	};

	CScene(OBJTYPE objtype, OBJECT obj = OBJECT_NONE);
	virtual ~CScene();
	virtual void Vertex(VERTEX_2D *pVtx) = 0;

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	static CScene *GetScene(OBJTYPE objtype, int num);
	int GetID();

	void SetObjType(OBJTYPE objtype);
	static OBJTYPE GetObjType(OBJTYPE objtype, int num);

protected:
	void Release(CScene **ppBeloneArray = m_apSceneDrow[0]);

	int m_nDrawIdx;

private:
	static CScene *m_apScene[OBJTYPE_MAX][MAX_2D];
	static CScene *m_apSceneDrow[OBJTYPE_MAX][MAX_2D];

	static int m_nNumAll;

	OBJTYPE m_eObjType;

	int m_nID;			// �����̔ԍ�
};

#endif