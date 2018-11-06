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

	//检查是否创建成功
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
	//异步IO结构
	BOOL fConnected;
	OVERLAPPED op;
	ZeroMemory(&op, sizeof(OVERLAPPED));

	//创建一个事件内核对象
	op.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//等待一个客户端进行连接
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
	//当有客户端进行连接时，事件变成有信号的状态
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

// 客户端的连接
int CPipe::Open()
{
	if(!WaitNamedPipe(m_strPipeName, 0))
		return -1;

	m_hNamedPipe = CreateFile(m_strPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "连接失败\n";
		return -2;
	}

	cout<<"连接成功\n";
	return 0;
}
