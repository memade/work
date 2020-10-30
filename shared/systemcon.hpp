#if !defined(AFX_SKSTU_PRODUCTSCON_H__6FDE3_EB259BA0_B036DA5A84A18AD_62B1__HEAD__)
#define AFX_SKSTU_PRODUCTSCON_H__6FDE3_EB259BA0_B036DA5A84A18AD_62B1__HEAD__

namespace shared {
#pragma pack(push,1)
	typedef struct tagSystemConProcessGuard final
	{
		bool enable;
		unsigned int interval;

		tagSystemConProcessGuard()
		{
			memset(this, 0x00, sizeof(*this));
			enable = true;
			interval = 60;
		}
		void operator=(const tagSystemConProcessGuard& obj) { memcpy(this, &obj, sizeof(*this)); }
	}SystemConProcessGuard;

	typedef struct tagSystemConProcessCheck final
	{
		bool enable;
		unsigned int interval;

		tagSystemConProcessCheck()
		{
			memset(this, 0x00, sizeof(*this));
			enable = true;
			interval = 60;
		}
		void operator=(const tagSystemConProcessCheck& obj) { memcpy(this, &obj, sizeof(*this)); }
	}SystemConProcessCheck;

	typedef struct tagSystemConObj final
	{
		unsigned int CaptureInterval;//! 截屏时间间隔
		unsigned int ProcessInfoInterval;//! 进程信息查询时间间隔
		unsigned int SystemInfoInterval; //! 系统信息查询时间间隔

		tagSystemConObj()
		{
			memset(this, 0x00, sizeof(*this));
			CaptureInterval = 60;
			ProcessInfoInterval = 180;
			SystemInfoInterval = 60;
		}
		void operator=(const tagSystemConObj& obj) { memcpy(this, &obj, sizeof(*this)); }
	}SystemConObj;

	typedef struct tagSystemConProcess final
	{
		CHAR Pathname[_MAX_PATH];
		CHAR Imagename[_MAX_FNAME];

		SystemConProcessGuard Guard;
		SystemConProcessCheck Check;

		tagSystemConProcess()
		{
			Pathname[0] = 0;
			Imagename[0] = 0;
		}
		void operator=(const tagSystemConProcess& obj) { memcpy(this, &obj, sizeof(*this)); }
		bool operator<(const tagSystemConProcess& obj) const
		{
			return sk::SafeCmpA(Pathname, obj.Pathname, _countof(Pathname)) < 0;
		}
		bool Verify() const
		{
			return Pathname[0] && Imagename[0] && (Guard.interval > 0 && Guard.interval < MAXUINT) && (Check.interval > 0 && Check.interval < MAXUINT);
		}
	}SystemConProcess;
#pragma pack(pop)

	class SystemCon final
	{
	public:
		SystemCon()
		{
		}
		~SystemCon()
		{
			std::ofstream of(PathnameGet(), std::ios::out | std::ios::trunc);
			if (of.is_open()) {
				of << os.str();
				of.close();
			}
		}
		void operator=(const SystemCon& obj)
		{
			id = obj.id;
			path = obj.path;
			process = obj.process;
		}
	public:
		void Init()
		{
			if (id <= 0) return;
			EncodeBegin();
			EncodeObj(obj);
			process.iterate(
				[&](auto& obj, bool& itbreak)
				{
					EncodeProcess(obj);
				});
			EncodeEnd();
		}
		void Finish()
		{
			std::ofstream of(PathnameGet(), std::ios::out | std::ios::trunc);
			if (of.is_open()) {
				of << os.str();
				of.close();
			}			
		}
		void EncodeBegin()
		{
			os << R"(<?xml version="1.0" encoding="ANSI" ?>)" << std::endl;
			os << R"(<SystemCon )"
				<< R"(ID=)"
				<< R"(")"
				<< sk::Log::Format("{:X}", id)
				<< R"(")"
				<< R"(>)" << std::endl;
		}
		void EncodeEnd()
		{
			os << R"(</SystemCon>)" << std::endl;
		}
		void EncodeObj(const SystemConObj& obj)
		{
			os << "\t"
				<< R"(<Obj )"
				<< R"(CaptureInterval=")" << obj.CaptureInterval << R"(" )"
				<< R"(ProcessInfoInterval=")" << obj.ProcessInfoInterval << R"(" )"
				<< R"(SystemInfoInterval=")" << obj.SystemInfoInterval << R"(" )"
				<< R"(/>)"
				<< std::endl;
		}
		void EncodeProcess(const SystemConProcess& process)
		{
			if (process.Pathname[0] == 0) return;
			os << "\t"
				<< R"(<Process )"
				<< R"(Pathname=")" << process.Pathname << R"(" )"
				<< R"(Imagename=")" << process.Imagename << R"(" )"
				<< R"(>)"
				<< std::endl;

			os << "\t\t"//Guard
				<< R"(<Guard )"
				<< R"(Enable=")"
				<< (process.Guard.enable ? R"(true" )" : R"(false" )")
				<< R"(Interval=")"
				<< process.Guard.interval
				<< R"("/>)"
				<< std::endl;

			os << "\t\t"//Check
				<< R"(<Check )"
				<< R"(Enable=")"
				<< (process.Guard.enable ? R"(true" )" : R"(false" )")
				<< R"(Interval=")"
				<< process.Guard.interval
				<< R"("/>)"
				<< std::endl;

			os << "\t"
				<< R"(</Process>)"
				<< std::endl;
		}
	public:
		sk::packet BufferGet()
		{
			Init();
			return os.str();
		}
		static bool Encode(
			const shared::SystemID ID,
			const shared::SystemConObj& ObjCon,
			const sk::container::set<shared::SystemConProcess>& Process,
			const sk::packet& out)
		{


			return true;
		}
		bool Decode(sk::packet& out)
		{
			return true;
		}
		static bool EncodeBuf(
			const shared::SystemID& in_id,
			const sk::container::set<SystemConProcess>& in_process,
			sk::packet& out_buffer
		)
		{
			return true;
		}
		static bool DecodeBuf(
			const sk::packet& buffer,
			shared::SystemID& out_id,
			shared::SystemConObj& out_obj,
			sk::container::set<SystemConProcess>& out_process)
		{
			if (buffer.empty()) return false;
			std::stringstream gangplank;
			std::iostream isetting(gangplank.rdbuf());
			isetting.write(buffer.data(), buffer.size());
			auto xmlfile = std::make_shared<rapidxml::file<>>(isetting);
			auto xmldoc = std::make_shared<rapidxml::xml_document<>>();
			try {
				xmldoc->parse<0>(xmlfile->data());
			}
			catch (rapidxml::parse_error& err) {
				auto what = err.what();
				return false;
			}

			auto beginNode = xmldoc->first_node("SystemCon");
			//! 系统产品结构
			auto first_attribute = beginNode->first_attribute();
			while (first_attribute) {
				if (!strcmp("ID", first_attribute->name())) {
					out_id = std::stoll(first_attribute->value(), nullptr, 16);
				}
				first_attribute = first_attribute->next_attribute();
			}

			auto nextNode = beginNode->first_node();
			while (nextNode) {
				if (!strcmp("Process", nextNode->name())) {
					SystemConProcess process;

					auto first_attribute = nextNode->first_attribute();
					while (first_attribute) {
						if (!strcmp("Pathname", first_attribute->name())) {
							sk::SafeCopyA(process.Pathname, first_attribute->value(), _countof(process.Pathname));
						}
						else if (!strcmp("Imagename", first_attribute->name())) {
							sk::SafeCopyA(process.Imagename, first_attribute->value(), _countof(process.Imagename));
						}
						first_attribute = first_attribute->next_attribute();
					}

					auto nextProcessNode = nextNode->first_node();
					while (nextProcessNode) {
						if (!strcmp("Guard", nextNode->name())) {
							auto first_attribute = nextProcessNode->first_attribute();
							while (first_attribute) {
								if (!strcmp("Enable", first_attribute->name())) {
									process.Guard.enable = false;
									if (!sk::SafeCmpA("true", first_attribute->value(), first_attribute->value_size()))
									{
										process.Guard.enable = true;
									}
								}
								else if (!strcmp("Interval", first_attribute->name())) {
									process.Guard.interval = strtol(first_attribute->value(), nullptr, 10);
								}
								first_attribute = first_attribute->next_attribute();
							}
						}
						else if (!strcmp("Check", nextNode->name())) {
							auto first_attribute = nextProcessNode->first_attribute();
							while (first_attribute) {
								if (!strcmp("Enable", first_attribute->name())) {
									process.Check.enable = false;
									if (!sk::SafeCmpA("true", first_attribute->value(), first_attribute->value_size()))
									{
										process.Check.enable = true;
									}
								}
								else if (!strcmp("Interval", first_attribute->name())) {
									process.Check.interval = strtol(first_attribute->value(), nullptr, 10);
								}
								first_attribute = first_attribute->next_attribute();
							}
						}
						nextProcessNode = nextProcessNode->next_sibling();
					}
					if (process.Verify())
					{
						out_process.push(process);
					}
					else
					{
						xmldoc->clear();
						return false;
					}
				}
				else if (!strcmp("Obj", nextNode->name())) {
					auto first_attribute = nextNode->first_attribute();
					while (first_attribute) {
						if (!strcmp("CaptureInterval", first_attribute->name())) {		
							auto val = strtol(first_attribute->value(), nullptr, 10);
							if (val >= 10 && val <= MAXINT)
							{
								out_obj.CaptureInterval = val;
							}
						}
						else if (!strcmp("ProcessInfoInterval", first_attribute->name())) {
							auto val = strtol(first_attribute->value(), nullptr, 10);
							if (val >= 10 && val <= MAXINT)
							{
								out_obj.ProcessInfoInterval = val;
							}
						}
						else if (!strcmp("SystemInfoInterval", first_attribute->name())) {
							auto val = strtol(first_attribute->value(), nullptr, 10);
							if (val >= 10 && val <= MAXINT)
							{
								out_obj.SystemInfoInterval = val;
							}
						}
						first_attribute = first_attribute->next_attribute();
					}
				}
				nextNode = nextNode->next_sibling();
			}///if (!strcmp("Process", nextNode->name()))

			xmldoc->clear();
			return true;
		}
	public:
		sk::stringa PathnameGet() const { return sk::Log::Format("{}\\{:X}.con", path, id); }
		void ID(const shared::SystemID& in) { id = in; }
		void Obj(const shared::SystemConObj& obj_) { obj = obj_; }
		const shared::SystemConObj& Obj() const { return obj; }
		const shared::SystemID& ID() const { return id; }
		void Path(const sk::stringa& in) { path = in; }
		const sk::stringa& Path() const { return path; }
	public:
		sk::container::set<SystemConProcess> process;
	private:
		std::string path;
		SystemConObj obj;
		shared::SystemID id = 0;
		std::ostringstream os;
	};
#if 0
	class ProductCon final {
	private:
		sk::stringa file;
		std::ostringstream os;
	public:
		static sk::stringa ProductConFileName(const shared::SystemProduct& sysProd) {
			return sk::Log::Format("{:X}.{}", sysProd.ID, sysProd.Symbol.symbol);
		}
		ProductCon(const sk::stringa& filePath, const shared::SystemProduct& sysProd) :
			file(filePath) {
			os << R"(<?xml version="1.0" encoding="ANSI" ?>)" << std::endl;
			os << R"(<SystemProduct)"
				<< R"( ID=")" << sk::Log::Format("{:X}", sysProd.ID) << R"(")"
				<< R"( Symbol=")" << sysProd.Symbol.symbol << R"(")"
				<< R"( Decimals=")" << tostringa(sysProd.Decimals) << R"(")"
				<< R"(>)"
				<< std::endl;
		}
		~ProductCon() {
			if (!file.empty()) {
				std::ofstream of(file, std::ios::out | std::ios::trunc);
				if (of.is_open()) {
					of << os.str();
					of.close();
				}
			}
		}
		sk::stringa BufferGet() const { return os.str(); }
	public:
		void EnconEnd() {
			os << R"(</SystemProduct>)" << std::endl;
		}
		void EnconProd(const shared::LPProduct& lpProd) {
			os << '\t'
				<< R"(<LPProduct)"
				<< R"( ID=")" << sk::Log::Format("{:X}", lpProd.ID) << R"(")"
				<< R"( Symbol=")" << lpProd.Symbol.symbol << R"(")"
				<< R"( SystemSymbol=")" << lpProd.SystemSymbol.symbol << R"(")"
				<< R"( QuotedCCY=")" << lpProd.QuotedCCY.currency << R"(")"
				<< R"( ContractSize=")" << tostringa(lpProd.ContractSize) << R"(")"
				<< R"( TickSize=")" << tostringa(lpProd.TickSize) << R"(")"
				<< R"( MinOrderSize=")" << tostringa(lpProd.MinOrderSize) << R"(")"
				<< R"( MaxOrderSize=")" << tostringa(lpProd.MaxOrderSize) << R"(")"
				<< R"( StepOrderSize=")" << tostringa(lpProd.StepOrderSize) << R"(")"
				<< R"(/>)"
				<< std::endl;
		}
		void EnconProd(const shared::MTFeederProduct& feederProd) {
			os << '\t'
				<< R"(<FeederProduct)"
				<< R"( ID=")" << sk::Log::Format("{:X}", feederProd.ID) << R"(")"
				<< R"( Symbol=")" << feederProd.Symbol.symbol << R"(")"
				<< R"( SpreadAsk=")" << tostringa(feederProd.SpreadAsk) << R"(")"
				<< R"( SpreadBid=")" << tostringa(feederProd.SpreadBid) << R"(")"
				<< R"( SpreadTotal=")" << tostringa(feederProd.SpreadTotal) << R"(")"
				<< R"( EnableQuote=")" << (feederProd.EnableQuote ? "true" : "false") << R"(")"
				<< R"(/>)"
				<< std::endl;
		}
		void EnconProd(const shared::MTGatewayProduct& gatewayProd) {
			os << '\t'
				<< R"(<GatewayProduct)"
				<< R"( ID=")" << sk::Log::Format("{:X}", gatewayProd.ID) << R"(")"
				<< R"( Symbol=")" << gatewayProd.Symbol.symbol << R"(")"
				<< R"( ContractSize=")" << tostringa(gatewayProd.ContractSize) << R"(")"
				<< R"( SpreadAsk=")" << tostringa(gatewayProd.SpreadAsk) << R"(")"
				<< R"( SpreadBid=")" << tostringa(gatewayProd.SpreadBid) << R"(")"
				<< R"( SpreadTotal=")" << tostringa(gatewayProd.SpreadTotal) << R"(")"
				<< R"( Digits=")" << tostringa(gatewayProd.Digits) << R"(")"
				<< R"( Point=")" << tostringa(gatewayProd.Point) << R"(")"
				<< R"( AutoAdjustPositionSpreadRange=")" << tostringa(gatewayProd.AutoAdjustPositionSpreadRange) << R"(")"
				<< R"( AssignOrderLP=")" << sk::Log::Format("{:X}", gatewayProd.AssignOrderLP) << R"(")"
				<< R"( EnableTrade=")" << (gatewayProd.EnableTrade ? "true" : "false") << R"(")"
				<< R"(/>)"
				<< std::endl;
		}

		static
			int DeconBuffer
			(
				__in const sk::packet& buffer,
				__out shared::SystemProduct& sysProd,
				__out sk::container::set<shared::LPProduct>& lpProds,
				__out sk::container::set<shared::MTFeederProduct>& mtFeederProds,
				__out sk::container::set<shared::MTGatewayProduct>& mtGatewayProds
			)
		{
			memset(&sysProd, 0x00, shared::LENSYSTEMPRODUCT);
			lpProds.clear();
			mtFeederProds.clear();
			mtGatewayProds.clear();
			if (buffer.empty()) return -1;
			std::stringstream gangplank;
			std::iostream isetting(gangplank.rdbuf());
			isetting.write(buffer.data(), buffer.size());
			auto xmlfile = std::make_shared<rapidxml::file<>>(isetting);
			auto xmldoc = std::make_shared<rapidxml::xml_document<>>();
			try {
				xmldoc->parse<0>(xmlfile->data());
			}
			catch (rapidxml::parse_error& err) {
				auto what = err.what();
				return -1;
			}

			auto beginNode = xmldoc->first_node("SystemProduct");
			//! 系统产品结构
			auto first_attribute = beginNode->first_attribute();
			while (first_attribute) {
				if (!strcmp("ID", first_attribute->name())) {
					sysProd.ID = shared::SystemIDType::SystemProductType(std::stoll(first_attribute->value(), nullptr, 16));
				}
				else if (!strcmp("Symbol", first_attribute->name())) {
					sysProd.Symbol = first_attribute->value();
				}
				else if (!strcmp("Decimals", first_attribute->name())) {
					sysProd.Decimals = std::stoi(first_attribute->value());
				}
				first_attribute = first_attribute->next_attribute();
			}

			auto nextNode = beginNode->first_node();
			while (nextNode) {
				if (!strcmp("LPProduct", nextNode->name())) {
					shared::LPProduct lpProd;
					auto first_attribute = nextNode->first_attribute();
					while (first_attribute) {
						if (!strcmp("ID", first_attribute->name())) {
							lpProd.ID = std::stoll(first_attribute->value(), nullptr, 16);
						}
						else if (!strcmp("Symbol", first_attribute->name())) {
							lpProd.Symbol = first_attribute->value();
						}
						else if (!strcmp("SystemSymbol", first_attribute->name())) {
							lpProd.SystemSymbol = first_attribute->value();
						}
						else if (!strcmp("QuotedCCY", first_attribute->name())) {
							lpProd.QuotedCCY = first_attribute->value();
						}
						else if (!strcmp("ContractSize", first_attribute->name())) {
							lpProd.ContractSize = std::stoi(first_attribute->value());
						}
						else if (!strcmp("TickSize", first_attribute->name())) {
							lpProd.TickSize = std::stod(first_attribute->value());
						}
						else if (!strcmp("MinOrderSize", first_attribute->name())) {
							lpProd.MinOrderSize = std::stod(first_attribute->value());
						}
						else if (!strcmp("MaxOrderSize", first_attribute->name())) {
							lpProd.MaxOrderSize = std::stod(first_attribute->value());
						}
						else if (!strcmp("StepOrderSize", first_attribute->name())) {
							lpProd.StepOrderSize = std::stod(first_attribute->value());
						}
						first_attribute = first_attribute->next_attribute();
					}///while
					if (lpProd.ID > 0)
					{
						lpProds.push(lpProd);
					}
				}
				else if (!strcmp("FeederProduct", nextNode->name())) {
					shared::MTFeederProduct mtFeederProd;
					auto first_attribute = nextNode->first_attribute();
					while (first_attribute) {
						if (!strcmp("ID", first_attribute->name())) {
							mtFeederProd.ID = std::stoll(first_attribute->value(), nullptr, 16);
						}
						else if (!strcmp("Symbol", first_attribute->name())) {
							mtFeederProd.Symbol = first_attribute->value();
						}
						else if (!strcmp("SpreadAsk", first_attribute->name())) {
							mtFeederProd.SpreadAsk = std::stoi(first_attribute->value());
						}
						else if (!strcmp("SpreadBid", first_attribute->name())) {
							mtFeederProd.SpreadBid = std::stoi(first_attribute->value());
						}
						else if (!strcmp("SpreadTotal", first_attribute->name())) {
							mtFeederProd.SpreadTotal = std::stoi(first_attribute->value());
						}
						else if (!strcmp("EnableQuote", first_attribute->name())) {
							mtFeederProd.EnableQuote = false;
							if (!strcmp("true", first_attribute->value())) {
								mtFeederProd.EnableQuote = true;
							}
						}
						first_attribute = first_attribute->next_attribute();
					}///while

					if (mtFeederProd.ID > 0)
					{
						mtFeederProds.push(mtFeederProd);
					}
				}
				else if (!strcmp("GatewayProduct", nextNode->name())) {
					shared::MTGatewayProduct mtGatewayProd;
					auto first_attribute = nextNode->first_attribute();
					while (first_attribute) {
						if (!strcmp("ID", first_attribute->name())) {
							mtGatewayProd.ID = std::stoll(first_attribute->value(), nullptr, 16);
						}
						else if (!strcmp("Symbol", first_attribute->name())) {
							mtGatewayProd.Symbol = first_attribute->value();
						}
						else if (!strcmp("SpreadAsk", first_attribute->name())) {
							mtGatewayProd.SpreadAsk = std::stoi(first_attribute->value());
						}
						else if (!strcmp("SpreadBid", first_attribute->name())) {
							mtGatewayProd.SpreadBid = std::stoi(first_attribute->value());
						}
						else if (!strcmp("SpreadTotal", first_attribute->name())) {
							mtGatewayProd.SpreadTotal = std::stoi(first_attribute->value());
						}
						else if (!strcmp("ContractSize", first_attribute->name())) {
							mtGatewayProd.ContractSize = std::stoi(first_attribute->value());
						}
						else if (!strcmp("Digits", first_attribute->name())) {
							mtGatewayProd.Digits = std::stoi(first_attribute->value());
						}
						else if (!strcmp("Point", first_attribute->name())) {
							mtGatewayProd.Point = std::stoi(first_attribute->value());
						}
						else if (!strcmp("AutoAdjustPositionSpreadRange", first_attribute->name())) {
							mtGatewayProd.AutoAdjustPositionSpreadRange = std::stoi(first_attribute->value());
						}
						else if (!strcmp("AssignOrderLP", first_attribute->name())) {
							mtGatewayProd.AssignOrderLP = std::stoll(first_attribute->value(), nullptr, 16);
						}
						else if (!strcmp("EnableTrade", first_attribute->name())) {
							mtGatewayProd.EnableTrade = false;
							if (!strcmp("true", first_attribute->value())) {
								mtGatewayProd.EnableTrade = true;
							}
						}
						first_attribute = first_attribute->next_attribute();
					}///while

					if (mtGatewayProd.ID > 0)
					{
						mtGatewayProds.push(mtGatewayProd);
					}
				}

				nextNode = nextNode->next_sibling();
			}

			xmldoc->clear();
			return 0;
		}

	};
#endif


}///namespace shared




/// /*新生联创（上海）**/
/// /*2019年12月1日 12:11:21|569**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_PRODUCTSCON_H__6FDE3_EB259BA0_B036DA5A84A18AD_62B1__TAIL__