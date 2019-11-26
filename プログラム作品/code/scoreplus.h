//======================================================================================================================
//
// �X�R�A�v���XUI����[scoreplus.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _SCOREPLUS_H_
#define _SCOREPLUS_H_

class CNumber;

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define MAX_PLUSDIGIT			(5)

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CScorePlus
{
public:
	CScorePlus();
	~CScorePlus();

	static CScorePlus *Create(D3DXVECTOR3 pos, D3DXCOLOR col, int plus, int count = 50);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	int GetCount() { return m_nCount; }

private:
	CNumber *m_Num[MAX_PLUSDIGIT];
	int m_nPlusNum;
	int m_Digit;

	int m_nCount;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;

	D3DXCOLOR m_col;
};

#endif