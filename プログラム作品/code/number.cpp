//======================================================================================================================
//
// �����`�揈��[number.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"

#include "number.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = {};

// �R���X�g���N�^
CNumber::CNumber() : CScene::CScene(OBJTYPE_UI)
{

}

// �f�X�g���N�^
CNumber::~CNumber()
{

}

HRESULT CNumber::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CNumber::Unload()
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//======================================================================================================================
// ����
//======================================================================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CNumber *pNum;

	pNum = new CNumber;

	pNum->m_pos = pos;
	pNum->m_size = size;

	pNum->Init();

	return pNum;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CNumber::Init()
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nNumber = 0;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ԁA�΁A��	�A�A���t�@�l
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f + 0.5f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//======================================================================================================================
// �I������
//======================================================================================================================
void CNumber::Uninit()
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	this->Release();
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CNumber::Update()
{

}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CNumber::Draw()
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void CNumber::SetNum(int num)
{
	m_nNumber = num;

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f + 0.5f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos += pos;

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

void CNumber::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

D3DXCOLOR CNumber::GetColor()
{
	return m_col;
}

