#if !defined(AFX_SK_H__85A81332_37CC_488C_BD47_AB67654102FE__HEAD__)
#define AFX_SK_H__85A81332_37CC_488C_BD47_AB67654102FE__HEAD__


namespace client {

	typedef std::function<int()> tf_sync_request;
	typedef std::function<void(const bool&, const sk::network::Header&, const sk::packet&)> tf_sync_callback;

	class NetSyncSession final
	{
	public:
		NetSyncSession(
			const sk::network::EnNetCmd& cmd,
			const time_t& timeout,
			const tf_sync_callback& callback) :
			sync_cmd(cmd),
			sync_finish(false),
			sync_timeout(timeout),
			sync_callback(callback)
		{
		}
	private:
		bool sync_finish;
		sk::network::EnNetCmd sync_cmd;
		time_t sync_timeout;
		tf_sync_callback sync_callback;

		sk::packet result_pak;
		sk::network::Header result_head;
	public:
		void operator=(const NetSyncSession& obj)
		{
			sync_cmd = obj.sync_cmd;
			sync_timeout = (obj.sync_timeout);
			sync_callback = obj.sync_callback;
			sync_finish = obj.sync_finish;
		}
		const sk::network::EnNetCmd& cmd() const { return sync_cmd; }
		time_t timeout() const { return sync_timeout; }
		const sk::packet& pak_result() const { return result_pak; }
		void pak_result(const sk::packet& val) { result_pak = val; }
		const sk::network::Header& head_result() const { return result_head; }
		void head_result(const sk::network::Header& val) { result_head = val; }
		void finish() const { sync_callback(true, result_head, result_pak); }
		void failed() const { sync_callback(false, sk::network::Header(), ""); }
	};

	class NetSync final
	{
	public:
		NetSync();
		~NetSync();
	public:
		bool Request(const NetSyncSession& session,const tf_sync_request&);
		bool Response(const sk::network::Header& head, const sk::packet& pak);
	private:
		std::shared_ptr<std::mutex> m_mutex;
		std::shared_ptr<sk::thread::ThreadPool> pThreadpool;
		sk::container::map<sk::network::EnNetCmd, bool> m_NetSyncResultQ;
		sk::container::map<sk::network::EnNetCmd, NetSyncSession> m_NetSyncTaskQ;
	};



}///namespace client







///!@ 新生联创®（上海）
/// ///!@ Thu Aug 13 13:20:20 UTC+0800 2020
/// ///!@ ___www.skstu.com___
#endif///AFX_SK_H__85A81332_37CC_488C_BD47_AB67654102FE__HEAD__