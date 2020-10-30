#include "stdafx.h"
#include "Client.h"

namespace client {

	Client::Client()
	{
		if (Init()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
	}

	Client::~Client()
	{
		UnInit();
	}

	int Client::Init()
	{
		sk::intptr out = 0;
		Global::GetApi()->GetFunction<shared::tf_api_network_tcp_client_setup>(shared::api_network, shared::api_network_tcp_client_setup)(
			Global::SetupGet()->ServerAddr().c_str(),
			out);
		NetApi<sk::network::INetworkApi>(out);
		return 0;
	}

	int Client::UnInit()
	{
		NetApi<sk::network::INetworkApi>()->Release();
		return 0;
	}

	int Client::Open()
	{
		if (m_IsOpen.load()) return 0;

		m_IsOpenT.store(true);
		NetApi<sk::network::INetworkApi>()->Start(this);

		m_IsOpen.store(true);
		return 0;
	}

	int Client::Close()
	{
		if (!m_IsOpen.load()) return 0;

		m_IsOpenT.store(false);
		for (auto it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			it->join();
		}
		m_Threads.clear();
		NetApi<sk::network::INetworkApi>()->Stop();

		m_IsOpen.store(false);
		return 0;
	}

	void Client::OnTcpClientConnect()
	{

	}

	void Client::OnTcpClientDisconnect()
	{

	}

	void Client::OnTcpClientWelcome(const sk::network::Header& head, const sk::packet& pak)
	{
		NetLoginRequest(Global::SetupGet()->LoginField());
	}

	void Client::OnTcpClientRead(const sk::network::Header& head, const sk::packet& pak)
	{
		switch (head.Cmd)
		{
		case sk::network::EnNetCmd::EN_NETCMD_100000030:
		{
			OnNetLoginResult(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100210:
		{
			OnNettSystemStatusInfos(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100220:
		{
			OnNetTargetScreenShot(head, pak);
		}break;
		}///switch
	}

	void Client::OnNetLoginResult(const sk::network::Header& head, const sk::packet& pak)
	{
		auto result = (shared::ClientField*)pak.data();
		if (result && SKAPIRES_SUCCESS(result->Result))
		{
			/*NetStatus(sk::network::EnNetStatus::EN_NETSTATUS_SYNCHRONIZED);*/
			Global::GetObjectNotify()->Push(sk::NotifyPacket((INT64)EnMessageUI::EN_MESSAGE_UI_LOGON_SUCCESS));
		}
		else
		{
			Global::GetObjectNotify()->Push(sk::NotifyPacket((INT64)EnMessageUI::EN_MESSAGE_UI_LOGON_FAILED));
		}
	}

	void Client::OnNettSystemStatusInfos(const sk::network::Header& head, const sk::packet& pak)
	{
		std::vector<shared::tagSystemStatusInfo> infos;
		sk::Helper::OneUnknownParseStruct<shared::tagSystemStatusInfo>(pak.data(),pak.size(), infos);
		for (const auto& info : infos)
		{
			Global::SharedGet()->m_SystemStatusInfoQ.push(info.ID, info);
		}
		Global::GetObjectNotify()->Push(sk::NotifyPacket((INT64)EnMessageUI::EN_MESSAGE_UI_SYSTEM_STATUS_INFO, pak.data(), pak.size()));
	}
	void Client::OnNetTargetScreenShot(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty()) return;
		Global::SharedGet()->m_SystemScreenShutQ.push((shared::SystemID)head.AdditionalTags, pak);
		Global::GetObjectNotify()->Push(sk::NotifyPacket((INT64)EnMessageUI::EN_MESSAGE_UI_SYSTEM_SCREEN_SHOT));
	}

























	int Client::NetLoginRequest(const shared::ClientField& login)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		shared::ClientField client = login;
		client.ID = sk::Helper::TickCountGet<std::chrono::microseconds>();
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100000030;

		std::vector<sk::packet> enpaks;
		NetApi<sk::network::INetworkApi>()->Packet(sk::packet((char*)&login, shared::LENCLIENTFIELD), head, enpaks);
		NetApi<sk::network::INetworkApi>()->Write(enpaks);
		return 0;
	}

	int Client::NetRequestSystemStatusInfos(shared::SystemID id /*= 0*/)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100210;
		std::vector<sk::packet> enpaks;
		if (id)
		{
			NetApi<sk::network::INetworkApi>()->Packet(sk::packet((char*)&id, sizeof(shared::SystemID)), head, enpaks);
		}
		else
		{
			NetApi<sk::network::INetworkApi>()->Packet("", head, enpaks);
		}
		NetApi<sk::network::INetworkApi>()->Write(enpaks);
		return 0;
	}

	int Client::NetRequestTargetScreenShot(const shared::SystemID& id)
	{
		if (id <= 0) return -1;
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100220;
		std::vector<sk::packet> enpaks;
		NetApi<sk::network::INetworkApi>()->Packet(sk::packet((char*)&id, sizeof(shared::SystemID)), head, enpaks);
		NetApi<sk::network::INetworkApi>()->Write(enpaks);
		return 0;
	}
}///namespace client