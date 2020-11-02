#if !defined(AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__HEAD__)
#define AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__HEAD__

namespace client {

	class Client final : public sk::object::IObject, public sk::network::INetworkSpi {
	public:
		Client();
		~Client();
	private:
		int Init() override final;
		int UnInit() override final;
	public:
		int Open() override final;
		int Close() override final;
	private:
		void OnNetLoginResult(const sk::network::Header&, const sk::packet&);
		void OnNettSystemStatusInfos(const sk::network::Header&, const sk::packet&);
		void OnNetTargetScreenShot(const sk::network::Header&, const sk::packet&);
	public:
		int NetLoginRequest(const shared::ClientField&);
		int NetRequestSystemStatusInfos(shared::SystemID id = 0);
		int NetRequestTargetScreenShot(const shared::SystemID&);
	protected:
		void OnTcpClientConnect() override final;
		void OnTcpClientDisconnect() override final;
		void OnTcpClientRead(const sk::network::Header&, const sk::packet&) override final;
		void OnTcpClientWelcome(const sk::network::Header&, const sk::packet&) override final;
	};





}///namespace client




/// /*新生联创（上海）**/
/// /*2019年12月28日 22:7:14|570**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__TAIL__