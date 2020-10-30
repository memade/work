﻿#if !defined(AFX_SKSTU_MEMMAP_H__69A72_E43315CA_4CBE9360B5A7B7E_2EF3__HEAD__)
#define AFX_SKSTU_MEMMAP_H__69A72_E43315CA_4CBE9360B5A7B7E_2EF3__HEAD__


#include "../common/File.hpp"

namespace shared {

	enum struct EnNodeChangedFlag : int {
		EN_NODE_CHANGED_FLAG_NORMAL = 0,
		EN_NODE_CHANGED_FLAG_PUSH = 1,
		EN_NODE_CHANGED_FLAG_POP = 2,
	};
	//!@ 注意！定长数据
	template<typename KEY, typename OBJ>
	class Memmap final {
	public:
		typedef struct Node final {
			sk::uint64 IdentifyHead;
			KEY m_Key;
			OBJ m_Obj;
			EnNodeChangedFlag ChangedFlag;
			sk::uint64 IdentifyTail;

			Node() { memset(this, 0x00, sizeof(*this)); IdentifyHead = 0xFAC9C2D0; IdentifyTail = 0xB4B4AAC1; }
			Node(const KEY& key, const OBJ& obj) {
				IdentifyHead = 0xFAC9C2D0;
				IdentifyTail = 0xB4B4AAC1;
				ChangedFlag = EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_NORMAL;
				m_Key = key;
				m_Obj = obj;
			}
			bool Verify() const { return IdentifyHead == 0xFAC9C2D0 && IdentifyTail == 0xB4B4AAC1; }
		}MemmapNode; const size_t LENMEMMAPNODE = sizeof(MemmapNode);
	public:
		Memmap(const sk::stringa& mapfilePathname, const sk::stringa& mapfileShared, bool isClear = false) :
			m_IsClear(isClear),
			m_mapfilePathname(mapfilePathname),
			m_mapfileShared(mapfileShared)
		{
			if (Init()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
			if (Open()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }

		}
		~Memmap()
		{
			Close();
			UnInit();
		}
	private:
		int Init()
		{
			sk::Helper::CreateDirectoryA(m_mapfilePathname.c_str());
			m_pMapFile = new sk::file::MapFile(m_mapfilePathname.c_str(), m_mapfileShared.c_str(), m_IsClear);
			return 0;
		}
		int UnInit()
		{
			SK_DELETE_PTR(m_pMapFile);
			return 0;
		}
		int Open()
		{
			if (!m_pMapFile->Open()) { return -1; }
			const auto pbuf = m_pMapFile->Read();
			const auto nbuf = m_pMapFile->GetDataLength();
			if (nbuf == 0) return 0;
			if (nbuf % LENMEMMAPNODE != 0) { return -1; }
			std::vector<Node> nodes;
			sk::Helper::OneUnknownParseStruct<Node>(pbuf, nbuf, nodes);
			if (nodes.empty()) return -1;
			for (size_t count = 0, pos = 0; count < nodes.size(); ++count, pos += LENMEMMAPNODE) {
				m_Data.push(nodes[count].m_Key, nodes[count]);
				m_DataPos.push(nodes[count].m_Key, pos);
			}
			return 0;
		}
		int Close()
		{
			if (m_FinishFlag.load())
			{
				m_pMapFile->Clear();
				m_Data.iterate(
					[&](const auto&, auto& obj)
					{
						m_pMapFile->Write((char*)&obj, (unsigned long)LENMEMMAPNODE);
					});
			}
			m_pMapFile->Close();
			return 0;
		}
	public:
		bool FinishFlag() const { return m_FinishFlag.load(); }
		void FinishFlag(const bool& flag) { m_FinishFlag.store(flag); }

		std::shared_ptr<sk::container::map<KEY, OBJ>>
			Map()
		{
			std::shared_ptr<sk::container::map<KEY, OBJ>> result;
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_Data.empty()) return result;
			result = std::make_shared<sk::container::map<KEY, OBJ>>();
			m_Data.iterate(
				[&](const auto&, auto& node)
				{
					result->push(node.m_Key, node.m_Obj);
				});
			return result;
		}

		size_t Size()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_Data.size();
		}

		bool Empty()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_Data.empty();
		}

		auto Begin()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_Data.begin();
		}

		auto End()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_Data.end();
		}

		enum class EnPushedResult : int {
			EN_PUSHED_RESULT_DEFAULT = 0,
			EN_PUSHED_RESULT_REPLACE = 1,
			EN_PUSHED_RESULT_CREATE = 2,
		};
		EnPushedResult Push(const KEY& key, const OBJ& obj)
		{
			EnPushedResult result = EnPushedResult::EN_PUSHED_RESULT_DEFAULT;
			std::lock_guard<std::mutex> lock(m_mutex);
			Node node(key, obj);
			node.ChangedFlag = EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_NORMAL;
			m_Data.push(node.m_Key, node);
			if (!m_DataPos.search(
				key,
				[&](const auto& key, auto& pos)
				{
					m_pMapFile->Write((unsigned long)pos, (char*)&node, (unsigned long)LENMEMMAPNODE);
					result = EnPushedResult::EN_PUSHED_RESULT_REPLACE;
				})) {
				auto freePos = m_FreePos.pop();
				if (freePos) {
					m_pMapFile->Write((unsigned long)(*freePos), (char*)&node, (unsigned long)LENMEMMAPNODE);
					m_DataPos.push(node.m_Key, *freePos);
				}
				else {
					m_pMapFile->Write((char*)&node, (unsigned long)LENMEMMAPNODE);
					m_DataPos.push(node.m_Key, m_pMapFile->GetDataLength());
				}
				result = EnPushedResult::EN_PUSHED_RESULT_CREATE;
			}
			return result;
		}

		void Clear()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.clear();
			m_DataPos.clear();
		}

		std::shared_ptr<OBJ> Pop(const KEY& key)
		{
			std::shared_ptr<OBJ> result;
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_Data.empty()) return result;
			m_Data.pop(
				key,
				[&](const auto&, auto& obj)
				{
					result = std::make_shared<OBJ>(obj.m_Obj);
				});
			m_DataPos.pop(
				key,
				[&](const auto&, auto& obj)
				{
					m_FreePos.push(obj);
				});
			return result;
		}
		std::shared_ptr<OBJ> Search(const KEY& key)
		{
			std::shared_ptr<OBJ> result;
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.search(
				key,
				[&](const auto&, auto& node)
				{
					result = std::make_shared<OBJ>(node.m_Obj);
				});
			return result;
		}
		bool Search(const KEY& key,const std::function<void(OBJ&,EnNodeChangedFlag&)>& cbSearch)
		{
			bool result = false;
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.search(
				key,
				[&](const auto&, auto& node)
				{
					cbSearch(node.m_Obj, node.ChangedFlag);
					result = true;
				});
			return result;
		}
		bool Pushpush(const KEY& key, const std::function<void(OBJ&, EnNodeChangedFlag&)>& cbSearch)
		{
			bool result = false;
			std::lock_guard<std::mutex> lock(m_mutex);
			if (!m_Data.search(
				key,
				[&](const auto&, auto& node)
				{
					cbSearch(node.m_Obj, node.ChangedFlag);
					node.ChangedFlag = EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_PUSH;
					result = true;
				}))
			{
				Node node(key, OBJ());
				cbSearch(node.m_Obj, node.ChangedFlag);
				node.ChangedFlag = EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_PUSH;
				m_Data.push(key, node);
			}
			return result;
		}
		std::shared_ptr<sk::container::map<KEY, OBJ>>
			Search(const std::function<bool(OBJ&, EnNodeChangedFlag&)>& cbCompare)
		{
			std::shared_ptr<sk::container::map<KEY, OBJ>> result;
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.iterate(
				[&](const auto& key, auto& obj, const auto&, auto& itbreak)
				{
					if (cbCompare(obj.m_Obj, obj.ChangedFlag)) {
						if (!result) result = std::make_shared<sk::container::map<KEY, OBJ>>();
						result->push(obj.m_Key, obj.m_Obj);
					}
				});
			return result;
		}

		std::shared_ptr<sk::container::map<KEY, OBJ>>
			Search(const std::function<bool(OBJ&)>& cbCompare)
		{
			std::shared_ptr<sk::container::map<KEY, OBJ>> result;
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.iterate(
				[&](const auto& key, auto& obj, const auto&, auto& itbreak)
				{
					if (cbCompare(obj.m_Obj)) {
						if (!result) result = std::make_shared<sk::container::map<KEY, OBJ>>();
						result->push(obj.m_Key, obj.m_Obj);
					}
				});
			return result;
		}

		void Iterate(const std::function<void(const KEY&, OBJ&, bool&,EnNodeChangedFlag&)>& cbit) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.iterate([&](const auto& key, auto& node, const auto&, auto& itbreak) {
				cbit(node.m_Key, node.m_Obj, itbreak, node.ChangedFlag);
				});
		}

		void Iterate(const std::function<void(const KEY&, OBJ&, bool&)>& cbit) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.iterate([&](const auto& key, auto& node, const auto&, auto& itbreak) {
				cbit(node.m_Key, node.m_Obj, itbreak);
				});
		}

		void Iterate(const std::function<void(const KEY&, OBJ&)>& cbit) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_Data.iterate([&](const auto& key, auto& node) {
				cbit(node.m_Key, node.m_Obj);
				});
		}

		void Finish() {
			std::lock_guard<std::mutex> lock(m_mutex);
			std::vector<KEY> dels;
			m_Data.iterate([&](const auto& key, auto& node)
				{
					switch (node.ChangedFlag)
					{
					case EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_NORMAL: 
					{

					}break;
					case EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_PUSH: 
					{
						node.ChangedFlag = EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_NORMAL;
						if (!m_DataPos.search(
							key,
							[&](const auto& key, auto& pos)
							{
								m_pMapFile->Write((unsigned long)pos, (char*)&node, (unsigned long)LENMEMMAPNODE);
							})) {
							auto freePos = m_FreePos.pop();
							if (freePos) {
								m_pMapFile->Write((unsigned long)(*freePos), (char*)&node, (unsigned long)LENMEMMAPNODE);
								m_DataPos.push(node.m_Key, *freePos);
							}
							else {
								m_pMapFile->Write((char*)&node, (unsigned long)LENMEMMAPNODE);
								m_DataPos.push(node.m_Key, m_pMapFile->GetDataLength());
							}
						}
					}break;
					case EnNodeChangedFlag::EN_NODE_CHANGED_FLAG_POP: 
					{
						dels.emplace_back(key);
					}break;
					default:
					{

					}break;
					}
				});

			for (const auto& del : dels)
			{
				m_Data.pop(
					del,
					[&](const auto& key, auto&)
					{
						m_DataPos.pop(
							key,
							[&](const auto&, auto& obj)
							{
								m_FreePos.push(obj);
							});
					});
			}
		}
	private:
		bool m_IsClear = false;
		//!说明 : 映影对象在析构时是否同步到磁盘的标志
		//!注意 : 
		//!日期 : Sat Jun 6 12:00:47 UTC+0800 2020
		std::atomic_bool m_FinishFlag = false;
		sk::stringa m_mapfilePathname;
		sk::stringa m_mapfileShared;

		std::mutex m_mutex;
		sk::file::MapFile* m_pMapFile = nullptr;
		sk::container::map<KEY, Node> m_Data;/*数据*/
		sk::container::queue<size_t> m_FreePos;/*空闲偏移*/
		sk::container::map<KEY, size_t> m_DataPos;/*数据偏移*/
	};








}///namesapce shared



/// /*新生联创（上海）**/
/// /*2020年01月26日 16:8:44|715**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_MEMMAP_H__69A72_E43315CA_4CBE9360B5A7B7E_2EF3__TAIL__


