#include "stdafx.h"
#include "Pipe.h"
#include <iostream>
using std::cout;
#include <windows.h>


CPipe::CPipe( CString strPipeName)
	:m_strPipeName(strPipeName)
	,m_hNamedPipe(NULL)
{
}


CPipe::~CPipe()
{
	if (m_hNamedPipe)
	{
		Close();
	}
}


BOOL CPipe::Create()
{
	m_hNamedPipe = CreateNamedPipe(m_strPipeName, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE, 1, 1024, 1024,0 , NULL);

	//����Ƿ񴴽��ɹ�
	if (m_hNamedPipe == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL CPipe::Connect()
{
	//�첽IO�ṹ
	BOOL fConnected;
	OVERLAPPED op;
	ZeroMemory(&op, sizeof(OVERLAPPED));

	//����һ���¼��ں˶���
	op.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//�ȴ�һ���ͻ��˽�������
	fConnected = ConnectNamedPipe(m_hNamedPipe, &op) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	if (!fConnected)
		return FALSE;
	//if (fConnected)
	//{
	//	// The client is connected.
	//}
	//else
	//{
	//	// The client could not connect, so close the pipe.
	//	return FALSE;
	//}
	//���пͻ��˽�������ʱ���¼�������źŵ�״̬
	if (WaitForSingleObject(op.hEvent, INFINITE) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CPipe::Close()
{
	if (m_hNamedPipe)
	{
		DisconnectNamedPipe(m_hNamedPipe);
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
	}
}

// �ͻ��˵�����
int CPipe::Open()
{
	if(!WaitNamedPipe(m_strPipeName, 0))
		return -1;

	m_hNamedPipe = CreateFile(m_strPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "����ʧ��\n";
		return -2;
	}

	cout<<"���ӳɹ�\n";
	return 0;
}
