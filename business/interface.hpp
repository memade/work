#if !defined(AFX_SK_H__664D8BD5_43D4_4311_BCD5_8F92BC294D2C__HEAD__)
#define AFX_SK_H__664D8BD5_43D4_4311_BCD5_8F92BC294D2C__HEAD__

#include "systemid.hpp"

namespace shared {
#pragma pack(push,1)

	typedef struct tagClientField
	{
		/*SystemID ID;*/
		char Username[32];
		char Password[32];
		/*char Identify[32];*/
		SKAPIRES Result;

		tagClientField()
		{
			memset(this, 0x00, sizeof(*this));
			Result = SKAPIRES::SYSTEM_RET_ERROR;
		}
		void operator=(const tagClientField& obj) { memcpy(this, &obj, sizeof(*this)); }
	}ClientField, AccountField; const size_t LENCLIENTFIELD = sizeof(ClientField);

	typedef struct tagConsoleCmdField
	{
		SystemID ID;
		char Input[128];
		char Explain[256];
		tagConsoleCmdField() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagConsoleCmdField& obj) { memcpy(this, &obj, sizeof(*this)); }
	}ConsoleCmdField; const size_t LENCONSOLECMDFIELD = sizeof(ConsoleCmdField);



	//!签名 : tagRequest
	//!说明 : 通用请求体的包装
	//!注意 : 
	//!日期 : Fri Jul 3 11:45:13 UTC+0800 2020
	typedef struct tagRequest final
	{
		shared::SystemID Cmd;//! 请求码/请求类型
		AccountField Account;//! 账户
		UINT ReqDataSize;//! 请求体数据大小
		CHAR ReqData[1];//! 请求体数据起始
		tagRequest() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagRequest& obj) { memcpy(this, &obj, sizeof(*this)); }
	}Request; const size_t LENREQUEST = sizeof(Request);


	//!签名 : tagRequestAck
	//!说明 : 通用请求ACK
	//!注意 : 
	//!日期 : Fri Jul 3 11:51:35 UTC+0800 2020
	typedef struct tagRequestAck final
	{
		shared::SystemID CmdSub;

		SKAPIRES SKRetcode;

		tagRequestAck() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagRequestAck& obj) { memcpy(this, &obj, sizeof(*this)); }
	}RequestAck; const size_t LENREQUESTACK = sizeof(RequestAck);



	//!签名 : tagRequestRes
	//!说明 : 通用请求RES
	//!注意 : 
	//!日期 : Fri Jul 3 11:51:52 UTC+0800 2020
	typedef struct tagRequestRes final
	{
		shared::SystemID CmdSub;

		SKAPIRES SKRetcode;


		//!说明 : 推送缓冲
		//!注意 : 根据CmdSub转换
		//!日期 : Wed Jul 1 17:45:25 UTC+0800 2020
		CHAR Cache[1024];

		tagRequestRes() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagRequestRes& obj) { memcpy(this, &obj, sizeof(*this)); }
	}RequestRes; const size_t LENREQUESTRES = sizeof(RequestRes);

	typedef struct tagSystemStatusInfo
	{
		SystemID ID;//!系统ID
		int	iCPUUsage;			// CPU使用率
		int	iMemory;			// 可用内存(M)
		INT64	time;				// 本次搜集的本机时间
		double dbDiskRead; //! 磁盘读速率
		double dbDiskWrite;//! 磁盘写速率
		char IPLocal[32];//! 内网IP
		char IPNetwork[32];//! 公网IP
		char SystemVersion[64];//! 系统版本号
		char Hostname[64];//! Hostname
		bool Online;//! 系统在线状态

		tagSystemStatusInfo() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagSystemStatusInfo& obj) { memcpy(this, &obj, sizeof(*this)); }
	}SystemStatusInfo; const size_t LENSYSTEMSTATUSINFO = sizeof(SystemStatusInfo);

	typedef struct tagSystemProcessStatusInfo
	{
		CHAR ImageName[64];
		DWORD PID;//! PID
		DWORD ThreadCount;//! 线程数
		double CPUUsedRate;//! CPU使用的百分比   
		DWORD MemoryUsed;//! 内存占用
		DWORD VirtualMemoryUsed;
		DWORD HandleCount;//! 句柄数
		UINT64 IOReadBytes;//IO读字节数   
		UINT64 IOWriteBytes; //IO写字节数    
		UINT64 IOReadCount;//IO读次数      
		UINT64 IOWriteCount; //IO写次数

		tagSystemProcessStatusInfo() { memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagSystemProcessStatusInfo& obj) { memcpy(this, &obj, sizeof(*this)); }
	}SystemProcessStatusInfo; const size_t LENSYSTEMPROCESSSTATUSINFO = sizeof(SystemProcessStatusInfo);


#pragma pack(pop)

}///namespace shared







///!@ 新生联创®（上海）
///!@ Wed May 27 12:42:10 UTC+0800 2020
///!@ ___www.skstu.com___
#endif///AFX_SK_H__664D8BD5_43D4_4311_BCD5_8F92BC294D2C__HEAD__
