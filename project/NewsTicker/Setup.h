#if !defined(AFX_SKSTU_SETUP_H__D616C_0CDB1D23_F11B32E9A17F765_36F3__HEAD__)
#define AFX_SKSTU_SETUP_H__D616C_0CDB1D23_F11B32E9A17F765_36F3__HEAD__

namespace client {

	class Setup final {
	public:
		Setup(const sk::stringa&);
		~Setup();
	public:
		const sk::stringa ServerAddr() const { return m_ServerAddr; }
		const sk::stringa ApiPath() const { return m_ApiPath; }
		const sk::stringa ObjPath() const { return m_ObjPath; }
		const shared::ClientField& LoginField() const { return m_LoginField; }
		const sk::stringa& CapturePath() const { return m_CapturePath; }
	private:
		int Init();
		int UnInit();
	private:
		sk::stringa m_ServerAddr;
		sk::stringa m_ApiPath;

		sk::stringa m_ObjPath;
		shared::ClientField m_LoginField;
		sk::stringa m_CapturePath;
	private:
		std::mutex m_mutex;
	};





}///namespace client





/// /*新生联创（上海）**/
/// /*2020年01月11日 20:56:10|862**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_SETUP_H__D616C_0CDB1D23_F11B32E9A17F765_36F3__TAIL__
