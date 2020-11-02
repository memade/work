#if !defined(AFX_SKSTU_Shared_H__71054_EF40B005_502F65870A2A6B4_2685__HEAD__)
#define AFX_SKSTU_Shared_H__71054_EF40B005_502F65870A2A6B4_2685__HEAD__

namespace client 
{

	const char JsonKeyGlobal_Data[] = "data";
	const char JsonKeyGlobal_Property[] = "property";
	const char JsonKeyGlobal_Time[] = "time";
	const char JsonKeyNews_Content[] = "content";
	const char JsonKeyNews_Time[] = "time";
	const char JsonKeyWeb_Url[] = "url";
	const char JsonKeyMD_Date[] = "Date";
	const char JsonKeyMD_PreClosePrice[] = "PreClosePrice";
	const char JsonKeyMD_TradeCodeOnLine[] = "TradeCodeOnLine";
	const char JsonKeyMD_TradePrice[] = "TradePrice";
	const char JsonKeyMD_UpPercent[] = "UpPercent";

	typedef struct tagNewstickerMD
	{
		wchar_t Date[64];
		wchar_t PreClosePrice[32];
		wchar_t TradeCodeOnLine[32];
		wchar_t TradePrice[32];
		wchar_t UpPercent[32];

		tagNewstickerMD() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagNewstickerMD& obj) { memcpy(this, &obj, sizeof(*this)); }
	}NewstickerMD;




	class Shared {
	public:
		Shared() 
		{
		}
		~Shared() {}
	public:
		NetSync m_SyncObj;

		//!签名 : m_SystemClientObjQ
		//!说明 : 系统目标客户端
		//!注意 : 
		//!日期 : Sun Aug 16 03:36:27 UTC+0800 2020
		sk::container::map<shared::SystemID, shared::ClientField> m_SystemClientObjQ;
		//!签名 : m_SystemStatusInfoQ
		//!说明 : 系统状态缓冲
		//!注意 : 
		//!日期 : Wed Aug 12 21:21:31 UTC+0800 2020
		sk::container::map<shared::SystemID, shared::SystemStatusInfo> m_SystemStatusInfoQ;
		//!签名 : m_SystemScreenShutQ
		//!说明 : 系统截屏缓冲
		//!注意 : 
		//!日期 : Wed Aug 12 21:19:14 UTC+0800 2020
		sk::container::map<shared::SystemID, sk::packet> m_SystemCaptureCacheQ;
		//!签名 : m_SystemConQ
		//!说明 : 系统配置缓冲
		//!注意 : 
		//!日期 : Thu Aug 13 07:19:25 UTC+0800 2020
		sk::container::map<shared::SystemID, sk::packet> m_SystemConCacheQ;

		//!签名 : m_SystemProcessStatusInfoCacheQ
		//!说明 : 
		//!注意 : 
		//!日期 : Sun Aug 16 22:51:44 UTC+0800 2020
		sk::container::map<shared::SystemID, sk::container::map<sk::stringa, shared::SystemProcessStatusInfo>> m_SystemProcessStatusInfoCacheQ;





		sk::container::queue<sk::container::map<sk::packet, sk::packet>> m_NewstickerMDQ;
		sk::container::queue<sk::container::map<sk::packet, sk::packet>> m_NewstickerNewsQ;
		sk::container::queue<sk::container::map<sk::packet, sk::packet>> m_NewstickerUrlQ;

		sk::container::map<sk::packet, sk::string> m_NewstickerNewsCacheQ;
		sk::container::map<sk::packet, sk::string> m_NewstickerUrlCacheQ;
		sk::container::map<sk::string, NewstickerMD> m_NewstickerMDCacheQ;

		static bool JsonParseGlobal(const sk::packet& in, sk::container::map<sk::packet, sk::container::map<sk::packet,sk::packet>>& out)
		{
			if (in.empty())
				return false;
			rapidjson::Document doc;
			rapidjson::Document::AllocatorType& Allocator = doc.GetAllocator();
			if (doc.Parse(in.c_str()).HasParseError()) { return false; }

			if (!doc.HasMember(rapidjson::Value().SetString(JsonKeyGlobal_Property, Allocator).Move()))
			{
				return false;
			}
			if (!doc.HasMember(rapidjson::Value().SetString(JsonKeyGlobal_Data, Allocator).Move()))
			{
				return false;
			}

			auto Property = doc.FindMember(rapidjson::Value().SetString(JsonKeyGlobal_Property, Allocator).Move());
			if (!Property->value.IsString())
				return false;
			sk::container::map<sk::packet, sk::packet> Propertys;
			Propertys.push(JsonKeyGlobal_Property, Property->value.GetString());
			out.push(JsonKeyGlobal_Property, Propertys);

			auto Data = doc.FindMember(rapidjson::Value().SetString(JsonKeyGlobal_Data, Allocator).Move());
			if (!Data->value.IsObject())
				return false;

			sk::container::map<sk::packet, sk::packet> DataVals;
			for (auto it = Data->value.MemberBegin(); it != Data->value.MemberEnd(); ++it)
			{
				if (!it->name.IsString() || !it->value.IsString())
					return false;
				DataVals.push(it->name.GetString(), it->value.GetString());
			}
			if (DataVals.empty())
				return false;
			out.push(JsonKeyGlobal_Data, DataVals);
			return true;
		}


		static sk::packet MadeSystemSetup(const sk::container::set<shared::SystemID>& inQuoteSourceSel)
		{
#if 0
			sk::packet result;
			if (inQuoteSourceSel.empty())
				return result;
			rapidjson::Document doc;
			doc.SetObject();
			rapidjson::Document::AllocatorType& JsonAllocator = doc.GetAllocator();
			rapidjson::Value Actions(rapidjson::kArrayType);
			Actions.SetArray();
			inQuoteSourceSel.iterate(
				[&](const auto& exchange, auto& itbreak)
				{
					rapidjson::Value Node(rapidjson::kObjectType);
					Node.SetObject();
					Node.AddMember(
						rapidjson::Value().SetString(JsonKeyQuoteSourceLP, JsonAllocator).Move(),
						exchange,
						JsonAllocator);
					Actions.PushBack(Node, JsonAllocator);
				});
			doc.AddMember(rapidjson::Value().SetString(JsonKeyQuoteSource, JsonAllocator).Move(), Actions, JsonAllocator);
			rapidjson::StringBuffer JsonBuffer;
			if (doc.IsObject()) {
				rapidjson::Writer<rapidjson::StringBuffer> JsonWriter(JsonBuffer);
				doc.Accept(JsonWriter);
			}
			return JsonBuffer.GetString();
#endif
			return  "";
		}

		static void UnMadeSystemSetup(const sk::packet& in, sk::container::set<shared::SystemID>& out)
		{
#if 0
			if (in.empty())
				return;
			rapidjson::Document doc;
			rapidjson::Document::AllocatorType& Allocator = doc.GetAllocator();
			if (doc.Parse(in.c_str()).HasParseError()) { return; }
			if (!doc.HasMember(rapidjson::Value().SetString(JsonKeyQuoteSource, Allocator).Move()))
			{
				return;
			}
			auto JsonKeyQuoteSourceObj = doc.FindMember(rapidjson::Value().SetString(JsonKeyQuoteSource, Allocator).Move());
			if (!JsonKeyQuoteSourceObj->value.IsArray())
				return;
			for (auto itAynode = JsonKeyQuoteSourceObj->value.Begin(); itAynode != JsonKeyQuoteSourceObj->value.End(); ++itAynode)
			{
				if (!itAynode->HasMember(rapidjson::Value().SetString(JsonKeyQuoteSourceLP, Allocator).Move()))
				{
					return;
				}
				auto val = itAynode->FindMember(rapidjson::Value().SetString(JsonKeyQuoteSourceLP, Allocator).Move());
				if (val->value.IsNumber() && !val->value.IsDouble())
				{
					out.push(val->value.GetUint64());
				}
			}
#endif
		}

	};



}





/// /*新生联创（上海）**/
/// /*2019年12月30日 11:51:48|304**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_Shared_H__71054_EF40B005_502F65870A2A6B4_2685__TAIL__