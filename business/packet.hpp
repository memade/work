#if !defined(AFX_SK_H__1D33CCCA_E91B_4216_ACA0_AB9582353811__HEAD__)
#define AFX_SK_H__1D33CCCA_E91B_4216_ACA0_AB9582353811__HEAD__

#include "../common/gpl/rapidjson/reader.h"
#include "../common/gpl/rapidjson/document.h"
#include "../common/gpl/rapidjson/rapidjson.h"
#include "../common/gpl/rapidjson/stringbuffer.h"
#include "../common/gpl/rapidjson/writer.h"
#include "../common/gpl/rapidjson/stream.h"
using namespace rapidjson;

namespace sk {

	const char JsonCmdFlag[] = "Cmd";
	const char JsonBodyFlag[] = "Body";
	const char JsonAccountFlag[] = "Account";
	const char JsonUsernameFlag[] = "Username";
	const char JsonPasswordFlag[] = "Password";
	const char JsonServiceFlag[] = "Service";
	using JsonAccounts = std::map<std::string, std::string>;
	using JsonBody = std::multimap<std::string, std::string>;
	using JsonBodys = std::vector<JsonBody>;
	class Packet final
	{
	public:
		Packet()
		{

		}
		~Packet()
		{

		}
	public:
		static std::string Made(const unsigned long long& cmd, const JsonAccounts& account, const JsonBodys& body)
		{
			rapidjson::Document JsonDoc;
			JsonDoc.SetObject();
			rapidjson::Document::AllocatorType& JsonAllocator = JsonDoc.GetAllocator();

			//!@cmd
			JsonDoc.AddMember(rapidjson::Value().SetString(JsonCmdFlag, JsonAllocator).Move(), rapidjson::Value().SetString(sk::Helper::IntegerToHexString<decltype(cmd)>(cmd).c_str(), JsonAllocator).Move(), JsonAllocator);
			//!@account
			rapidjson::Value jsonAccountObj(rapidjson::kObjectType);
			for (auto it = account.begin(); it != account.end(); ++it)
			{
				jsonAccountObj.AddMember(
					rapidjson::Value().SetString(it->first.c_str(), JsonAllocator).Move(),
					rapidjson::Value().SetString(it->second.c_str(), JsonAllocator).Move(),
					JsonAllocator);
			}
			JsonDoc.AddMember(rapidjson::Value().SetString(JsonAccountFlag, JsonAllocator).Move(), jsonAccountObj, JsonAllocator);
			//!@body
			rapidjson::Value bodys(rapidjson::kArrayType);
			for (auto it1 = body.begin(); it1 != body.end(); ++it1)
			{
				rapidjson::Value body(rapidjson::kObjectType);
				for (auto it2 = it1->begin(); it2 != it1->end(); ++it2)
				{
					body.AddMember(
						rapidjson::Value().SetString(it2->first.c_str(), JsonAllocator).Move(),
						rapidjson::Value().SetString(it2->second.c_str(), JsonAllocator).Move(),
						JsonAllocator);
				}
				bodys.PushBack(body, JsonAllocator);
			}
			JsonDoc.AddMember(rapidjson::Value().SetString(JsonBodyFlag, JsonAllocator).Move(), bodys, JsonAllocator);

			rapidjson::StringBuffer JsonBuffer;
			if (JsonDoc.IsObject()) {
				rapidjson::Writer<StringBuffer> JsonWriter(JsonBuffer);
				JsonDoc.Accept(JsonWriter);
			}
			return JsonBuffer.GetString();
		}

		static int MadeUn
		(
			__in const std::string& buffer, 
			__out unsigned long long& cmd, 
			__out JsonAccounts& account,
			__out JsonBodys& bodys
		)
		{
			rapidjson::Document JsonDoc;
			if (JsonDoc.Parse(buffer.c_str()).HasParseError())
				return -1;
			if (JsonDoc.MemberCount() <= 0)
				return -1;
			if (!JsonDoc.HasMember(JsonCmdFlag))
				return -1;

			auto& valCmd = JsonDoc[JsonCmdFlag];
			if (!valCmd.IsString())
				return -1;
			cmd = sk::Helper::HexStringToInteger<unsigned long long>(valCmd.GetString());
			
			auto& ayAccount = JsonDoc[JsonAccountFlag];
			if (!ayAccount.IsObject())
				return -1;

			for (auto it = ayAccount.MemberBegin(); it != ayAccount.MemberEnd(); ++it)
			{
				if (it->name.IsNull() || it->value.IsNull()) continue;
				if (!it->name.IsString() || !it->value.IsString()) continue;

				account.insert({ {it->name.GetString(),it->value.GetString()} });
			}

			auto& ayBody = JsonDoc[JsonBodyFlag];
			if (!ayBody.IsArray())
				return -1;
			for (auto ay = ayBody.Begin(); ay != ayBody.End(); ++ay)
			{
				if (ay->IsNull()) continue;

				JsonBody body;
				for (auto node = ay->MemberBegin(); node != ay->MemberEnd(); ++node)
				{
					if (node->name.IsNull() || node->value.IsNull()) continue;
					if (!node->name.IsString() || !node->value.IsString()) continue;

					body.insert({ {node->name.GetString(),node->value.GetString()} });
				}
				if (!body.empty())
				{
					bodys.emplace_back(body);
				}			
			}
			return 0;
		}
	};




}///namespace sk







///!@ 新生联创®（上海）
///!@ Fri Jul 3 15:07:58 UTC+0800 2020
///!@ ___www.skstu.com___
#endif///AFX_SK_H__1D33CCCA_E91B_4216_ACA0_AB9582353811__HEAD__
