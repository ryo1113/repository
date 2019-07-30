//======================================================================================================================
//
// ���C������ [main.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"

#ifdef _DEBUG
// �f�o�b�N�����̎�����

#endif

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define CLASS_NAME		"WindowClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Windows"			// �E�C���h�E�̃L���v�V������

//======================================================================================================================
// �\���̒�`
//======================================================================================================================

//======================================================================================================================
// �v���g�^�C�v�錾
//======================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void Uninit(void);
void Update(void);
void Draw(void);

//MODE g_Mode = MODE_TITLE;		//���݂̃��[�h
//MODE g_ModeOld = MODE_TITLE;		//�O�̃��[�h

//======================================================================================================================
// �O���[�o���ϐ�
//======================================================================================================================
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^(�`��ɕK�v)

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
#ifdef _DEBUG
int CRenderer::m_nCountFPS = 0;
#endif

//======================================================================================================================
// ����������
//======================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect =	{0,	0, SCREEN_WIDTH, SCREEN_HEIGHT};

	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwExecLastTime;		// �Ō�ɏ�����������	

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�C���h�E���쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	CManager *pManager;
	pManager = new CManager;

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))		// FALSE�Ńt�����
	{
		return -1;
	}

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ 0 ��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W�������烋�[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���

			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				CRenderer::m_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;		// ��������������ۑ�

				// �X�V����
				pManager->Update();

				// �`�揈��
				pManager->Draw();

				dwFrameCount++;			// �J�E���g�����Z
			}
		}
	}
	
	if (pManager != NULL)
	{
		// �I������
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//======================================================================================================================
// �E�C���h�E�v���V�[�W��
//======================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//======================================================================================================================
// ����������
//======================================================================================================================
void Init(void)
{

}


//======================================================================================================================
// �I������
//======================================================================================================================
void Uninit(void)
{	

}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void Update(void)
{

}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void Draw(void)
{

}

//======================================================================================================================
// ���[�h�̐ݒ�
//======================================================================================================================

