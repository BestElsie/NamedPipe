class CPipe
{
public:
	CPipe(CString strPipeName);
	~CPipe();

	BOOL	Create();
	BOOL	Connect();
	int		Open();
	void	Close();

	template<typename T>
	BOOL	Read(T &content, size_t nSize)
	{
		DWORD dwRead;
		return ReadFile(m_hNamedPipe, &content, nSize, &dwRead, NULL);
	}

	template<typename T>
	BOOL	Write(T &content, size_t nSize)
	{
		DWORD dwWrite;
		return WriteFile(m_hNamedPipe, &content, nSize, &dwWrite, NULL);
	}
private:
	CString	m_strPipeName;
	HANDLE	m_hNamedPipe;
};


