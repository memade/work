#include "stdafx.h"
#include "NetSync.h"

namespace client {

	NetSync::NetSync()
	{
		m_mutex = std::make_shared<std::mutex>();
		pThreadpool = std::make_shared<sk::thread::ThreadPool>(64);
	}

	NetSync::~NetSync()
	{

	}

	bool NetSync::Request(const NetSyncSession& session, const tf_sync_request& request_cb)
	{
		bool result = false;
		std::lock_guard<std::mutex> lock(*m_mutex);
		if (m_NetSyncTaskQ.push_insert_cb(
			session.cmd(),
			session,
			[&](const NetSyncSession& session)
			{
				if (0 == request_cb())
				{
					m_NetSyncResultQ.push(session.cmd(), false);
				}
			})) {

			auto thread_param_session = session;
			pThreadpool->enqueue(
				[this, thread_param_session]()
				{
					auto future_result = std::async(
						[&]()
						{
							bool finish = false;
							bool timeout = false;
							time_t old = sk::Helper::TickCountGet<std::chrono::milliseconds>();
							do
							{
								m_NetSyncResultQ.search_clear(
									thread_param_session.cmd(),
									[&](auto& success, bool& itclear)
									{
										itclear = success;
										finish = success;
									});
								if (finish || (sk::Helper::TickCountGet<std::chrono::milliseconds>() - old >= thread_param_session.timeout()))
								{
									break;
								}
								std::this_thread::sleep_for(std::chrono::milliseconds(10));
							} while (1);
						});
					auto status = future_result.wait_for(std::chrono::milliseconds(thread_param_session.timeout())) == std::future_status::ready;
					m_NetSyncTaskQ.pop(
						thread_param_session.cmd(),
						[&](const auto&, auto& _session)
						{
							status ? _session.finish() : _session.failed();
						});
				});
		}

		return result;
	}

	bool NetSync::Response(const sk::network::Header& head, const sk::packet& pak)
	{
		bool result = false;
		std::lock_guard<std::mutex> lock(*m_mutex);
		m_NetSyncResultQ.search(
			head.Cmd,
			[&](const auto&, auto& success)
			{
				m_NetSyncTaskQ.search(head.Cmd,
					[&](const auto&, auto& session)
					{
						session.head_result(head);
						session.pak_result(pak);
						success = true;
					});
				result = success;
			});
		return result;
	}
}///namespace client