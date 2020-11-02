#if !defined(AFX_SK_H__E2D7FA3A_EE5B_45A0_8D89_E263E3A7627B__HEAD__)
#define AFX_SK_H__E2D7FA3A_EE5B_45A0_8D89_E263E3A7627B__HEAD__

namespace shared {

#pragma pack(push,1)
	typedef unsigned long long SystemID;

	enum struct EnServiceObjectFlag : SystemID
	{
		EN_SERVICE_OBJECT_FLAG_UNDEFINED = 0x00000000000,
		EN_SERVICE_OBJECT_FLAG_GUEST = 0x10000000000,
		EN_SERVICE_OBJECT_FLAG_ADMIN = 0x20000000000,
		EN_SERVICE_OBJECT_FLAG_MTDATAFEED = 0x30000000000,
		EN_SERVICE_OBJECT_FLAG_MTGATEWAY = 0x40000000000,
		EN_SERVICE_OBJECT_FLAG_LPMARKETDATA = 0x50000000000,
		EN_SERVICE_OBJECT_FLAG_LPTRADEORDER = 0x60000000000,
		EN_SERVICE_OBJECT_FLAG_DEVELOPER = 0x70000000000,
#if 0
		EN_SERVICE_OBJECT_FLAG_MDDATASOURCE_REALTIME = 0x80000000000,
		EN_SERVICE_OBJECT_FLAG_MDDATASOURCE_HISTORY = 0x90000000000,
#endif
		EN_SERVICE_OBJECT_FLAG_CLIENT = 0xA0000000000,
		EN_SERVICE_OBJECT_FLAG_MANAGER_CLIENT = 0xB0000000000,
		EN_SERVICE_OBJECT_FLAG_MANAGER_VIEW = 0xC0000000000,
	};

	enum struct EnServiceObjectExchangeFlag : SystemID
	{
		EN_SERVICE_OBJECT_EXCHANGE_FLAG_MDREALTIME = 0x100000000,
		EN_SERVICE_OBJECT_EXCHANGE_FLAG_MDHISTORY = 0x200000000,
	};
#pragma pack(pop)
	class SystemIDType final
	{
	public:
		//!@ left bit '9' '10' 
		//!@ 交易所/LP ID
		static SystemID ExchangeType(const SystemID& id)
		{
			return id & 0xFF00000000;
		}
		//! left bit '11' -- 网络客户端类型/上下文类型/服务对象类型
		static SystemID ServiceObjectType(const SystemID& id)
		{
			return id & 0xF0000000000;
		}

		//! left bit '12' '13' '14' '15'
		//!@ 映射/聚合后的产品ID
		//!@ 桥系统组件唯一ID
		//!@ 分发后的产品ID主键
		static SystemID SystemProductType(const SystemID& id)
		{
			return id & 0xFFFF00000000000;
		}

		//! left bit '1' '2' '3' '4' '5' '6' '7' '8'
		//!@ 交易所产品ID
		static SystemID ExchangeProductType(const SystemID& id)
		{
			return id & 0xFFFFFFFF;
		}

		//!签名 : ExchangeProductTypeReset
		//!说明 : 产品ID清理
		//!注意 : 一般用与由网关产品ID转换到交易所产品ID
		//!日期 : Mon May 25 06:37:14 UTC+0800 2020
		static SystemID ExchangeProductTypeReset(SystemID& id)
		{
			id = id >> 8 * 4 << 8 * 4;
			return id;
		}


		//!签名 : IDMade
		//!说明 : 生成系统定单ID 定单ID= 当前毫秒时间戳<<2bid | 交易所ID>>8bit
		//!注意 : 1.系统定单ID的Hex字符串占用13个字节 2.定单ID最大值‭2559999999999999‬‬(0x9184E729FFFFF);
		//!日期 : Sun May 24 13:46:03 UTC+0800 2020
		static SystemID IDMade(__in const SystemID& ExchangeID,__in const SystemID& MTReqID)
		{
			return (shared::SystemIDType::ExchangeType(ExchangeID) >> 4 * 8) | (MTReqID << 2 * 4);
				/*(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << 2 * 4);*/
				
		}
		
		//!签名 : IDMadeUn
		//!说明 : 解析系统定单ID
		//!注意 : 
		//!日期 : Sun May 24 13:57:01 UTC+0800 2020
		static SystemID IDMadeUn(__in const SystemID& OrderID, __out SystemID& ExchangeID, __out SystemID& MTReqID)
		{
			ExchangeID = (OrderID & 0xFF) << 8 * 4;
			MTReqID = OrderID >> 2 * 4;
			return ExchangeID;
		}
#if 0	
		//!签名 : IDMadeUn
		//!说明 : 解析系统定单ID
		//!注意 : 
		//!日期 : Sun May 24 13:58:40 UTC+0800 2020
		static SystemID IDMadeUn(__in const SystemID& OrderID, __out SystemID& ExchangeID, __out sk::stringa& OrderGMTTime)
		{
			OrderGMTTime.clear();
			ExchangeID = (OrderID & 0xFF) << 8 * 4;
			__int64 OrderTimestamp = OrderID >> 2 * 4;		
			OrderTimestamp = (OrderTimestamp - OrderTimestamp % 1000000) / 1000000;
			char timestr[32] = { 0 };
			struct tm _tm;
			gmtime_s(&_tm, &OrderTimestamp);
			OrderGMTTime.append(timestr, strftime(timestr, _countof(timestr), sk::global_sk_ftime_utc_format_a, &_tm));
			return ExchangeID;
		}
#endif
		
		//!签名 : IDToStr
		//!说明 : 系统定单ID生成HEX字符串
		//!注意 : 系统定单ID生成HEX字符串占用13个字节
		//!日期 : Sun May 24 13:58:56 UTC+0800 2020
		static std::string IDToStr(const SystemID& OrderID)
		{
#if 0
			std::string result;
			std::array<char, sizeof(SystemID) * 8> ay;
			if (auto [p, ec] = std::to_chars(ay.data(), ay.data() + ay.size(), OrderID, 16);
				ec == std::errc()) {
				result.append(ay.data(), p - ay.data());
				std::transform(result.begin(), result.end(), result.begin(),[](auto c) { return std::toupper(c); });			
			}
			return result;
#endif
			return "";
		}
		static std::wstring IDToWStr(const SystemID& OrderID)
		{
#if 0
			std::string result;
			std::array<char, sizeof(SystemID) * 8> ay;
			if (auto [p, ec] = std::to_chars(ay.data(), ay.data() + ay.size(), OrderID, 16);
				ec == std::errc()) {
				result.append(ay.data(), p - ay.data());
				std::transform(result.begin(), result.end(), result.begin(), [](auto c) { return std::toupper(c); });
			}
			return sk::StringConvert::MBytesToWString(result);
#endif
			return L"";
		}

		//!签名 : StrToID
		//!说明 : HEX字符串转换为系统定单ID类型
		//!注意 : 系统定单ID类型为 UINT64/SystemID 类型
		//!日期 : Sun May 24 13:59:58 UTC+0800 2020
		static SystemID StrToID(const std::string& OrderID)
		{
			SystemID result = 0;
#if 0

			if (auto [p, ec] = std::from_chars(OrderID.data(), OrderID.data() + OrderID.size(), result, 16);
				ec == std::errc())
			{
			}
#endif
			return result;
		}
		static SystemID WStrToID(const std::wstring& OrderID)
		{
			std::string source = sk::StringConvert::WStringToMBytes(OrderID);
			SystemID result = 0;
#if 0

			if (auto [p, ec] = std::from_chars(source.data(), source.data() + source.size(), result, 16);
				ec == std::errc())
			{
			}
			return result;
#endif
			return 0;
		}
	};
}///namespace shared








///!@ 新生联创®（上海）
///!@ Sun May 24 08:02:14 UTC+0800 2020
///!@ ___www.skstu.com___
#endif///AFX_SK_H__E2D7FA3A_EE5B_45A0_8D89_E263E3A7627B__HEAD__

