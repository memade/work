#include "stdafx.h"
#include "UIClient.h"

namespace client {

	void UIClient::OnTcpClientConnect()
	{
		auto sk = 0;
	}

	void UIClient::OnTcpClientDisconnect()
	{
		auto sk = 0;
	}

	void UIClient::OnTcpClientWelcome(const sk::network::Header& head, const sk::packet& pak)
	{
		NetLoginRequest(Global::SetupGet()->LoginField());
	}

	void UIClient::OnTcpClientRead(const sk::network::Header& head, const sk::packet& pak)
	{
		sk::network::Header head_(head);

		//!@ 为了配合同步请求登录任务
		if (sk::network::EnNetCmd::EN_NETCMD_100000060 == head.Cmd ||
			sk::network::EnNetCmd::EN_NETCMD_100000070 == head.Cmd)
		{
			head_.Cmd = sk::network::EnNetCmd::EN_NETCMD_100000030;
		}

		if (Global::SharedGet()->m_SyncObj.Response(head_, pak))
		{
			return;
		}

		switch (head.Cmd)
		{
		case sk::network::EnNetCmd::EN_NETCMD_100000010:
		{
			OnNetLoginResult(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100000030:
		{
			
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100000040:
		{

		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100000050:
		{

		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100000060:
		{

		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100000070:
		{
			auto sk = 0;
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100205:
		{
			OnNetSystemClientObjs(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100210:
		{
			OnNetSystemStatusInfo(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100220:
		{
			OnNetSystemCapture(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100230:
		{
			OnNetSystemCon(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100240:
		{
			//OnNetSystemPoweroffAck(head, pak);
			OnNetServerMessage(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100250:
		{
			OnNetSystemRebootAck(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100260:
		{
			OnNetSystemLogoffAck(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100270:
		{
			OnNetSystemProcessStatus(head, pak);
		}break;
		case sk::network::EnNetCmd::EN_NETCMD_100100280:
		{
			OnNetSystemProcessKillAck(head, pak);
		}break;
		default:
		{
			break;
		}
		}///switch


	}


	void UIClient::MsgBox(HWND hWnd, const sk::string& Text, const sk::string& CaptionText, const UINT& uType)
	{
		Global::GetCore()->m_MsgBoxPakQ.push(sk::MsgBoxPak(Text, CaptionText, uType, hWnd));
	}



	void UIClient::OnNetLoginResult(const sk::network::Header& head, const sk::packet& pak)
	{
		auto result = (shared::ClientField*)pak.data();
		if (result && SKAPIRES_SUCCESS(result->Result))
		{
			NetRequestNewStickerMD();
			NetRequestNewStickerHttpUrl();
			NetRequestNewStickerNews();


			auto sk = 0;
		}
		else
		{
			auto sk = 0;
		}
	}

	void UIClient::OnNetSystemClientObjs(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;
		Global::SharedGet()->m_SystemClientObjQ.clear();
		std::vector<shared::ClientField> clients;
		sk::Helper::OneUnknownParseStruct<shared::ClientField>(pak.data(), pak.size(), clients);
		for (const auto& client : clients)
		{
			//Global::SharedGet()->m_SystemClientObjQ.push(client.ID, client);
		}

		OnUISystemClientObjsUpdate();
	}

	void UIClient::OnNetServerMessage(const sk::network::HEADER& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;

		sk::container::map<sk::packet, sk::container::map<sk::packet, sk::packet>> out;
		if (!Shared::JsonParseGlobal(pak, out) || out.empty())
		{
			Global::Log(sk::EnLogType::EN_LOG_TYPE_ERROR)->info("json data parse error,({})", pak.c_str());
			return;
		}

		sk::packet DataType = "";
		out[JsonKeyGlobal_Property].begin(
			[&](const auto&, auto& type)
			{
				DataType = type;
			});

		if (DataType == "1")
		{
			Global::SharedGet()->m_NewstickerMDQ.push(out[JsonKeyGlobal_Data]);
		}
		else if (DataType == "2")
		{
			Global::SharedGet()->m_NewstickerNewsQ.push(out[JsonKeyGlobal_Data]);
		}
		else if (DataType == "3")
		{
			Global::SharedGet()->m_NewstickerUrlQ.push(out[JsonKeyGlobal_Data]);
		}
		else
		{
			Global::Log(sk::EnLogType::EN_LOG_TYPE_TRACE)->info("{}", pak);
		}
	}


	void UIClient::OnNetSystemCon(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;
		Global::SharedGet()->m_SystemConCacheQ.push(head.DataExtern.ExternDataU64, pak);
		OnUISystemConUpdate();
	}
	void UIClient::OnNetSystemStatusInfo(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;
		auto info = (shared::SystemStatusInfo*)pak.data();
		Global::SharedGet()->m_SystemStatusInfoQ.push(head.DataExtern.ExternDataU64, *info);
		OnUISystemStatusUpdate();
		OnUISystemClientObjsUpdate(info);
	}
	void UIClient::OnNetSystemCapture(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;
		Global::SharedGet()->m_SystemCaptureCacheQ.push(head.DataExtern.ExternDataU64, pak);
		OnUISystemCaptureUpdate();
	}
	void UIClient::OnNetSystemProcessStatus(const sk::network::Header& head, const sk::packet& pak)
	{
		if (pak.empty())
			return;
		auto info = (shared::SystemProcessStatusInfo*)pak.data();
		Global::SharedGet()->m_SystemProcessStatusInfoCacheQ.pushpush(
			head.DataExtern.ExternDataU64,
			[&](auto& proObj) {proObj.push(info->ImageName, *info); });
		OnUISystemConProcessUpdate(info->ImageName);
		
	}
	void UIClient::OnNetSystemPoweroffAck(const sk::network::Header& head, const sk::packet& pak)
	{

	}
	void UIClient::OnNetSystemLogoffAck(const sk::network::Header& head, const sk::packet& pak)
	{

	}
	void UIClient::OnNetSystemRebootAck(const sk::network::Header& head, const sk::packet& pak)
	{

	}
	void UIClient::OnNetSystemProcessKillAck(const sk::network::Header& head, const sk::packet& pak)
	{

	}

















	int UIClient::NetLoginRequest(const shared::ClientField& login)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		//shared::ClientField client = login;
		//client.ID = sk::Helper::TickCountGet<std::chrono::microseconds>();
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100000070;

		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet(sk::packet((char*)&login, shared::LENCLIENTFIELD), head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}

	int UIClient::NetRequestNewStickerMD() const
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100210;

		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}

	int UIClient::NetRequestNewStickerNews() const
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100220;

		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}

	int UIClient::NetRequestNewStickerHttpUrl() const
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100230;

		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}


	int UIClient::NetRequestSystemClientObjs()
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100205;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemStatusInfo(const shared::SystemID& id)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100210;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemPoweroff(const shared::SystemID& id)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100240;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemReboot(const shared::SystemID& id)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100250;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemLogoff(const shared::SystemID& id)
	{
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100260;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemKillProcess(const shared::SystemID& id, const sk::packet& image)
	{
		if (id <= 0 || image.empty())
			return -1;
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100280;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet(image, head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}
	int UIClient::NetRequestSystemCon(const int& flag, const shared::SystemID& id, const sk::packet& pak)
	{
		if (id <= 0)
			return -1;
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100230;
		head.DataExtern.ExternData64 = flag;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet(pak, head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}

	int UIClient::NetRequestSystemCapture(const shared::SystemID& id)
	{
		if (id <= 0) return -1;
		if (!NetApi<sk::network::INetworkApi>() || NetApi<sk::network::INetworkApi>()->NetStatus() < sk::network::EnNetStatus::EN_NETSTATUS_CONNECTED)
			return -1;
		sk::network::Header head;
		head.Cmd = sk::network::EnNetCmd::EN_NETCMD_100100220;
		head.DataExtern.ExternDataU64 = id;
		sk::packet enpak;
		NetApi<sk::network::INetworkApi>()->Packet("", head, enpak);
		NetApi<sk::network::INetworkApi>()->Write(enpak);
		return 0;
	}


}///namespace client