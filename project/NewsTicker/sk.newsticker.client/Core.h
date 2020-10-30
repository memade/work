#if !defined(AFX_SKSTU_CORE_H__258E5_B966CDB5_90C75C14186D00B_5E90__HEAD__)
#define AFX_SKSTU_CORE_H__258E5_B966CDB5_90C75C14186D00B_5E90__HEAD__

namespace client {

	class Core final : public sk::object::IObject {
	public:
		Core();
		~Core();
	private:
		int Init() override final;
		int UnInit() override final;
	public:
		int Open() override final;
		int Close() override final;

		UIClient* GetClient() const { return m_pClient; }
	private:
		UIClient* m_pClient = nullptr;
		void ProcessT();
		void MsgBoxT();
	public:
		sk::container::queue<sk::MsgBoxPak> m_MsgBoxPakQ;
	};


}///namespace client




/// /*新生联创（上海）**/
/// /*2019年12月28日 18:24:32|948**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CORE_H__258E5_B966CDB5_90C75C14186D00B_5E90__TAIL__