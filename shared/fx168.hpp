#if !defined(AFX_SKSTU_MARKETDATA_H__44768_01AC0E09_075A5DEBC0E8D2E_6297__HEAD__)
#define AFX_SKSTU_MARKETDATA_H__44768_01AC0E09_075A5DEBC0E8D2E_6297__HEAD__

#include "systemid.hpp"

namespace shared {
#pragma pack(push,1)
	namespace fx168 {
		typedef long long DataTime;


		const unsigned char packet_head = 0xFF;
		const unsigned char packet_tail = 0x00;

		typedef struct tagCode final
		{
			SystemID Source;

			tagCode() { memset(this, 0x00, sizeof(*this)); }
			tagCode(const tagCode& obj) { *this = obj; }
			tagCode(const char* obj) { if (obj)*this = obj; else memset(this, 0x00, sizeof(*this)); }
			tagCode(const SystemID& obj) { *this = obj; }
			void operator=(const tagCode& obj) { Source = obj.Source; }
			void operator=(const SystemID& obj) { Source = obj; }
			void operator=(const char* obj) { memset(this, 0x00, sizeof(*this)); if (obj) { memcpy(&Source, obj, __min(sizeof(Source), strlen(obj))); } }
			bool operator<(const tagCode& obj) const { return Source < obj.Source; }
			bool operator<(const SystemID& obj) const { return Source < obj; }
			SystemID src() const { return Source; }
			const char* str() const { return  reinterpret_cast<const char*>(&Source); }
			bool verify() const { return Source > 0 && Source < 0xFFFFFFFFFFFFFFFF; }
		}Code; const size_t LENCODE = sizeof(Code);

		
		//!签名 : tagDataPeriod
		//!说明 : 表示数据时间段
		//!注意 : 精确到秒
		//!日期 : Fri Jun 5 00:00:31 UTC+0800 2020
		typedef struct tagDataPeriod final
		{
			__int64 From;
			__int64 To;
			tagDataPeriod() { memset(this, 0x00, sizeof(*this)); }
			tagDataPeriod(const __int64& from, const __int64& to) { From = from; To = to; }
			void operator=(const tagDataPeriod& obj) { memcpy(this, &obj, sizeof(*this)); }
		}DataPeriod;

		enum struct EnDataType : unsigned char
		{
			EN_DATA_TYPE_m = 'm',//!成交价(带交易所简称)
			EN_DATA_TYPE_n = 'n',//!成交价
			EN_DATA_TYPE_o = 'o',//!修补成交价
			EN_DATA_TYPE_p = 'p',//!开盘、最高、最低、昨收
			EN_DATA_TYPE_q = 'q',//!买一、卖一
			EN_DATA_TYPE_r = 'r',//!买三、买二、卖二、卖三
			EN_DATA_TYPE_s = 's',//!买五、买四、卖四、卖五
			EN_DATA_TYPE_t = 't',//!总成交量、总成交金额(持仓量)
			EN_DATA_TYPE_u = 'u',//!结算价、均价
			EN_DATA_TYPE_v = 'v',//!清盘信号

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_m,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_v,
		}; const size_t ENDATATYPE_MEMBER_TOTAL_COUNT = 10;

		enum struct EnDataTypeIndex_m : int
		{
			EN_DATA_TYPE_INDEX_TIME = 0,//! 成交时间
			EN_DATA_TYPE_INDEX_ISN = 1,//! 公司内码
			EN_DATA_TYPE_INDEX_BPX1 = 2,//! 成交价(买一价)
			EN_DATA_TYPE_INDEX_SPX1 = 3,//! 卖一价
			EN_DATA_TYPE_INDEX_QTY = 4,//! 成交量(单笔)
			EN_DATA_TYPE_INDEX_AMOUNT = 5,//! 成交金额或持仓量(单笔)
			EN_DATA_TYPE_INDEX_TQTY = 6,//! 总成交量
			EN_DATA_TYPE_INDEX_TAMOUNT = 7,//! 总成交金额或总持仓量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_TIME,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_TAMOUNT,
		}; const size_t ENDATATYPEINDEX_m_MEMBER_TOTAL_COUNT = 8;

		enum struct EnDataTypeIndex_n : int
		{
			EN_DATA_TYPE_INDEX_ISN = 0,//! 公司内码
			EN_DATA_TYPE_INDEX_TIME = 1,//! 成交时间
			EN_DATA_TYPE_INDEX_BPX1 = 2,//! 成交价(买一价)
			EN_DATA_TYPE_INDEX_SPX1 = 3,//! 卖一价
			EN_DATA_TYPE_INDEX_QTY = 4,//! 成交量(单笔)
			EN_DATA_TYPE_INDEX_AMOUNT = 5,//! 成交金额或持仓量(单笔)
			EN_DATA_TYPE_INDEX_TQTY = 6,//! 总成交量
			EN_DATA_TYPE_INDEX_TAMOUNT = 7,//! 总成交金额或总持仓量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_ISN,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_TAMOUNT,
		}; const size_t ENDATATYPEINDEX_n_MEMBER_TOTAL_COUNT = 8;

		enum struct EnDataTypeIndex_o : int
		{
			EN_DATA_TYPE_INDEX_ISN = 0,//! 公司内码
			EN_DATA_TYPE_INDEX_BPX = 1,//! 成交价

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_ISN,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_BPX,
		}; const size_t ENDATATYPEINDEX_o_MEMBER_TOTAL_COUNT = 2;

		enum struct EnDataTypeIndex_p : int
		{
			EN_DATA_TYPE_INDEX_ISN = 0,//! 公司内码
			EN_DATA_TYPE_INDEX_OPEN = 1,//! 开盘
			EN_DATA_TYPE_INDEX_HIGH = 2,//! 最高
			EN_DATA_TYPE_INDEX_LOW = 3,//! 最低
			EN_DATA_TYPE_INDEX_CLOSE = 4,//! 昨收

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_ISN,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_CLOSE,
		}; const size_t ENDATATYPEINDEX_p_MEMBER_TOTAL_COUNT = 5;

		enum struct EnDataTypeIndex_q : int
		{
			EN_DATA_TYPE_INDEX_ISN = 0,//! 公司内码
			EN_DATA_TYPE_INDEX_BPX1 = 1,//! 买一价
			EN_DATA_TYPE_INDEX_BQTY1 = 2,//! 买一量
			EN_DATA_TYPE_INDEX_SPX1 = 3,//! 卖一价
			EN_DATA_TYPE_INDEX_SQTY1 = 4,//! 卖一量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_ISN,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_SQTY1,
		}; const size_t ENDATATYPEINDEX_q_MEMBER_TOTAL_COUNT = 5;

		enum struct EnDataTypeIndex_r : int
		{
			EN_DATA_TYPE_INDEX_BPX3 = 0,//! 买三价
			EN_DATA_TYPE_INDEX_BQTY3 = 1,//! 买三量
			EN_DATA_TYPE_INDEX_BPX2 = 2,//! 买二价
			EN_DATA_TYPE_INDEX_BQTY2 = 3,//! 买二量
			EN_DATA_TYPE_INDEX_SPX2 = 4,//! 卖二价
			EN_DATA_TYPE_INDEX_SQTY2 = 5,//! 卖二量
			EN_DATA_TYPE_INDEX_SPX3 = 6,//! 卖三价
			EN_DATA_TYPE_INDEX_SQTY3 = 7,//! 卖三量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_BPX3,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_SQTY3,
		}; const size_t ENDATATYPEINDEX_r_MEMBER_TOTAL_COUNT = 8;

		enum struct EnDataTypeIndex_s : int
		{
			EN_DATA_TYPE_INDEX_BPX5 = 0,//! 买五价
			EN_DATA_TYPE_INDEX_BQTY5 = 1,//! 买五量
			EN_DATA_TYPE_INDEX_BPX4 = 2,//! 买四价
			EN_DATA_TYPE_INDEX_BQTY4 = 3,//! 买四量
			EN_DATA_TYPE_INDEX_SPX4 = 4,//! 卖四价
			EN_DATA_TYPE_INDEX_SQTY4 = 5,//! 卖四量
			EN_DATA_TYPE_INDEX_SPX5 = 6,//! 卖五价
			EN_DATA_TYPE_INDEX_SQTY5 = 7,//! 卖五量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_BPX5,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_SQTY5,
		}; const size_t ENDATATYPEINDEX_s_MEMBER_TOTAL_COUNT = 8;

		enum struct EnDataTypeIndex_t : int
		{
			EN_DATA_TYPE_INDEX_TQTY = 0,//! 总成交量
			EN_DATA_TYPE_INDEX_TAMOUNT = 1,//! 总成交金额或总持仓量

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_TQTY,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_TAMOUNT,
		}; const size_t ENDATATYPEINDEX_t_MEMBER_TOTAL_COUNT = 2;

		enum struct EnDataTypeIndex_u : int
		{
			EN_DATA_TYPE_INDEX_SETPX = 0,//! 结算价
			EN_DATA_TYPE_INDEX_AVGPX = 1,//! 均价(暂时未用)

			EN_DATA_TYPE_FIRST = EN_DATA_TYPE_INDEX_SETPX,
			EN_DATA_TYPE_LAST = EN_DATA_TYPE_INDEX_AVGPX,
		}; const size_t ENDATATYPEINDEX_u_MEMBER_TOTAL_COUNT = 2;

		enum struct EnDataTypeIndex_v : int
		{
			//! 清盘数据（无）
		}; const size_t ENDATATYPEINDEX_v_MEMBER_TOTAL_COUNT = 0;

		typedef struct tagHeartBeat final
		{

			tagHeartBeat() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagHeartBeat& obj) { memcpy(this, &obj, sizeof(*this)); }
		}HeartBeat; const size_t LENHEARBEAT = sizeof(HeartBeat);

#if 0
		typedef struct tagISN  final
		{
			char source[20];

			tagISN() { memset(this, 0x00, sizeof(*this)); }
			tagISN(const char* obj) { *this = obj; }
			tagISN(const tagISN& obj) { *this = obj; }
			bool operator<(const tagISN& obj) const { for (size_t i = 0; i < _countof(source); ++i) { if (source[i] != obj.source[i])return source[i] < obj.source[i]; }return false; }
			void operator=(const tagISN& obj) { memset(this, 0x00, sizeof(*this)); for (size_t i = 0; i < _countof(source); ++i) { if (obj.source[i] == 0) break; source[i] = obj.source[i]; } }
			void operator=(const char* obj) { if (obj) { memset(this, 0x00, sizeof(*this)); for (size_t i = 0; i < _countof(source); ++i) { if (obj[i] == 0) break; source[i] = obj[i]; } } }
			bool empty() const { return source[0] == 0; }
		}ISN;
		typedef struct tagCode final
		{
			char TradeCodeOnLine[6];

			tagCode() { memset(this, 0x00, sizeof(*this)); }
			tagCode(const char* obj) { *this = obj; }
			tagCode(const tagCode& obj) { *this = obj; }
			bool operator<(const tagCode& obj) const { for (size_t i = 0; i < _countof(TradeCodeOnLine); ++i) { if (TradeCodeOnLine[i] != obj.TradeCodeOnLine[i])return TradeCodeOnLine[i] < obj.TradeCodeOnLine[i]; }return false; }
			void operator=(const tagCode& obj) { memset(this, 0x00, sizeof(*this)); for (size_t i = 0; i < _countof(TradeCodeOnLine); ++i) { if (obj.TradeCodeOnLine[i] == 0) break; TradeCodeOnLine[i] = obj.TradeCodeOnLine[i]; } }
			void operator=(const char* obj) { if (obj) { memset(this, 0x00, sizeof(*this)); for (size_t i = 0; i < _countof(TradeCodeOnLine); ++i) { if (obj[i] == 0) break; TradeCodeOnLine[i] = obj[i]; } } }
		}Code; const size_t LENCODE = sizeof(Code);
#endif


		//!签名 : 
		//!说明 : 
		//!注意 : 
		//!日期 : Sat Jun 6 20:54:00 UTC+0800 2020
		typedef struct tagDaySyncInfo final
		{
			long long TimestampSyncDataDay;//! 同步的最后一节日线收盘数据Key
			long long TimestampSyncDataMin30;//! 同步的最后一节30分钟线收盘数据Key
			long long TimestampSyncDataMin01;//! 同步的最后一节1分钟线收盘数据Key

			tagDaySyncInfo() { memset(this,0x00,sizeof(*this)); }
			void operator=(const tagDaySyncInfo& obj) { memcpy(this,&obj,sizeof(*this)); }
		}DaySyncInfo;


		typedef struct tagCodeInfo final
		{
			tagCode Code;
			SystemID TradeID;
			SystemID ExchangeID;//! NewExchangeID
			SystemID ExchangeCodeID;//! 
			char TradeCodeOnLine[6];
			char TradeCode[20];
			char TradePinYinIndexCode[30];
			char TradeName[50];
			unsigned short DecimalDigits;
			double PerTradeVol;
			char Currency[6];
			unsigned short TradeTimeTableStart1;
			unsigned short TradeTimeTableEnd1;
			unsigned short TradeTimeTableStart2;
			unsigned short TradeTimeTableEnd2;
			unsigned short TradeTimeTableStart3;
			unsigned short TradeTimeTableEnd3;
			unsigned short TradeTimeTableStart4;
			unsigned short TradeTimeTableEnd4;
			unsigned short SunriseTime;
			unsigned short DayLineMakeTime;
			char FlagMakeAvgPrice;
			char FlagMarket;
			char DaylightSavingTimeFlag;
			char MeasureUnit[20];
			double MinChangeVal;
			//!说明 : 数据库以外附加的用来描述本地同步等情况的成员
			//!注意 : 
			//!日期 : Sat Jun 6 19:40:42 UTC+0800 2020
			DaySyncInfo SyncInfo;


			tagCodeInfo() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagCodeInfo& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagCodeInfo& obj) const { return Code < obj.Code; }
		}CodeInfo,Instrument; const size_t LENCODEINFO = sizeof(CodeInfo);
		const size_t LENINSTRUMENT = LENCODEINFO;

		typedef struct tagExchangeInfo final
		{
			SystemID ExchangeID;//!@ sql 'NewExchangeID'
			SystemID ExchangeCode;
			char ExchangeNameCN[32];
			char ExchangeNameEN[32];

			tagExchangeInfo() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagExchangeInfo& obj) { memcpy(this, &obj, sizeof(*this)); }
		}ExchangeInfo; const size_t LENEXCHANGEINFO = sizeof(ExchangeInfo);



		typedef struct tagMarketData final
		{
			SystemID ID;//! Code
			DataTime Time;//! 数据时间
			int Digits;//! 小数位数
			unsigned char ExchangeID;//!交易所代码(10~252)
			unsigned char Type;//! 报价更新类型
			double Qty;//! 成交量(单笔)
			double TQty;//! 成交量(总)
			double Amount;//! 成交额(单笔)
			double TAmount;//! 成交额(总)
			double SetPx;//! 结算价
			double AvgPx;//! 均价
			double High;//! 最高价
			double Low;//! 最低价
			double Open;//! 开盘价
			double Close;//! 昨日收盘价
			double Bpx1;//! 买一价
			double Bpx2;//! 买二价
			double Bpx3;//! 买三价
			double Bpx4;//! 买四价
			double Bpx5;//! 买五价
			double Spx1;//! 卖一价
			double Spx2;//! 卖二价
			double Spx3;//! 卖三价
			double Spx4;//! 卖四价
			double Spx5;//! 卖五价
			double Bqty1;//! 买一量
			double Bqty2;//! 买二量
			double Bqty3;//! 买三量
			double Bqty4;//! 买四量
			double Bqty5;//! 买五量
			double Sqty1;//! 卖一量
			double Sqty2;//! 卖二量
			double Sqty3;//! 卖三量
			double Sqty4;//! 卖四量
			double Sqty5;//! 卖五量
			char Reserved[38];
			tagMarketData() { memset(this, 0x00, sizeof(*this)); Digits = 1; }
			void operator=(const tagMarketData& obj) { memcpy(this, &obj, sizeof(*this)); }
		}MarketData, MD, * PMD; const size_t LENMARKETDATA = sizeof(MD);

		typedef struct tagMDDataDay
		{
			DataTime Date;
			double OpenPrice;
			double HighPrice;
			double LowPrice;
			double ClosePrice;
			double Amount;
			double Hold;

			tagMDDataDay() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMDDataDay& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagMDDataDay& obj) const { return Date < obj.Date; }
		}MDDataDay; const size_t LENMDDATADAY = sizeof(MDDataDay);

		typedef struct tagMDDataMin01
		{
			DataTime Date;
			double OpenPrice;
			double HighPrice;
			double LowPrice;
			double ClosePrice;
			double AvgPrice;
			double Amount;
			double Hold;

			tagMDDataMin01() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMDDataMin01& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagMDDataMin01& obj) const { return Date < obj.Date; }
		}MDDataMin01; const size_t LENMDDATAMIN01 = sizeof(MDDataMin01);

		typedef struct tagMDDataMin30
		{
			DataTime Date;
			double OpenPrice;
			double HighPrice;
			double LowPrice;
			double ClosePrice;
			double Amount;
			double Hold;

			tagMDDataMin30() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMDDataMin30& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagMDDataMin30& obj) const { return Date < obj.Date; }
		}MDDataMin30; const size_t LENMDDATAMIN30 = sizeof(MDDataMin30);

		typedef struct tagMDDataDensity
		{
			double TradePrice;
			double Amount;

			tagMDDataDensity() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMDDataDensity& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagMDDataDensity& obj) const { return TradePrice < obj.TradePrice; }
		}MDDataDensity; const size_t LENMDDATADENSITY = sizeof(MDDataDensity);

		typedef struct tagMDDataTik
		{
			unsigned long long ID;
			DataTime Date;
			double TradePrice;
			double Amount;
			double Hold;

			tagMDDataTik() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMDDataTik& obj) { memcpy(this, &obj, sizeof(*this)); }
			bool operator<(const tagMDDataTik& obj) const { return ID < obj.ID; }
		}MDDataTik; const size_t LENMDDATATIK = sizeof(MDDataTik);









		//!签名 : 
		//!说明 : 订阅路由包
		//!注意 : 
		//!日期 : Wed Jul 1 13:17:49 UTC+0800 2020
		typedef struct tagSubscibeRoute final
		{

			tagSubscibeRoute() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagSubscibeRoute& obj) { memcpy(this, &obj, sizeof(*this)); }
		}SubscibeRoute; const size_t LENSUBSCIBEROUTE = sizeof(SubscibeRoute);





	}///namespace fx168





#pragma pack(pop)
}///namespace shared




/// /*新生联创（上海）**/
/// /*2020年01月9日 16:40:39|145**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_MARKETDATA_H__44768_01AC0E09_075A5DEBC0E8D2E_6297__TAIL__