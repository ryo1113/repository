//===========================================================================================================
//
// ���� [scene2D.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//===========================================================================================================
// �C���N���[�h�t�@�C��
//===========================================================================================================
#include "main.h"
#include "scene.h"

//===========================================================================================================
// ���C�u�����̃����N
//===========================================================================================================

//===========================================================================================================
// �}�N����`
//===========================================================================================================

//===========================================================================================================
// �N���X��`
//===========================================================================================================
class CScene2D : public CScene
{
public:
	CScene2D(OBJTYPE objtype = OBJTYPE_NONE, OBJECT obj = OBJECT_NONE);
	virtual ~CScene2D();

	static CScene2D *Create();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetCollar(D3DXCOLOR collar);

	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScale(D3DXVECTOR3 scale);
	void SetSize(D3DXVECTOR3 size);
	void BindMoveTex(D3DXVECTOR3 *pMTex);

	void Vertex(VERTEX_2D *pVtx);

	bool HitShapeCollision(CScene2D *scene2D);

protected:

	D3DXVECTOR3 MoveVector(D3DXVECTOR3 vector, float fSpeed);
	D3DXVECTOR3 Move(float fAngle, float fSpeed);
	D3DXVECTOR3 MoveMouse(D3DXVECTOR3 pos);

	bool UnlookJudge();
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVertex();
	void SetVertexTex(VERTEX_2D *pVtx);
	void SetVertexTex_Scroll(VERTEX_2D *pVtx);

	void SetTextureAnimationTex(int nHeight, int nWide);
	bool SetTextureAnimation(int nFrame, bool bLoop);

	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetScale();
	float GetLength();

	void Rot();

private:
	void Marginalize(float *fot);

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scale;

	// ��]�Ǘ�
	D3DXVECTOR3 m_rotDifference;				// �������������ƍ��̍�
	D3DXVECTOR3 m_rotDirection;					// ��������

	float m_fLength;							// ���a

	D3DXCOLOR m_col;

	int m_nHeight;
	int m_nWide;

	// �A�j���[�V�����Ǘ�
	int m_nCounterAnim;
	int m_nPatternAnim;

	D3DXVECTOR3 *pMoveTex;
};

#endif