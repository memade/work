#if !defined(AFX_SKSTU_UIPRODUCT_H__F641F_C67BF4B0_5D0641478E5A45A_F092__HEAD__)
#define AFX_SKSTU_UIPRODUCT_H__F641F_C67BF4B0_5D0641478E5A45A_F092__HEAD__

namespace client {

	enum struct EnProductEditUIType : __int64 {
		EN_PRODUCT_EDIT_UI_TYPE_SYSTEM = 0x2,
		EN_PRODUCT_EDIT_UI_TYPE_EXCHANGE = 0x4,
		EN_PRODUCT_EDIT_UI_TYPE_MTFEEDER = 0x6,
		EN_PRODUCT_EDIT_UI_TYPE_MTGATEWAY = 0x8,

		EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_STATUS = 0x9,
		EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON_PROCESS = 0x10,
		EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CAPTURE = 0x11,
		EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON = 0x12,
	};

	class IExchangeNodeUI final : public CListContainerElementUI {
	public:
		void SetName(const sk::stringa&);
		void SetValue(const sk::stringa&);
		void SetStatus(const sk::stringa&);
	};

	class IQuotesNodeUI final : public CHorizontalLayoutUI {
	public:
		IQuotesNodeUI();
		~IQuotesNodeUI();
	public:
		void SetMD(const NewstickerMD&);
		void AppendMD(const std::vector<NewstickerMD>&);
	};

	class INewstickerNews final : public CHorizontalLayoutUI
	{
	public:
		void Push(const sk::packet&, const sk::string&);
		void SetNews(const sk::string&);
	private:
		sk::container::map<sk::packet, sk::string> m_NewsQ;
	};



























	class IProductTreeNodeUI final : public CListContainerElementUI {
	public:
		void SetSystemID(const shared::SystemID&);
		void SetSystemHost(const sk::stringa&);
		void SetSystemStatus(const bool&);
	};


	class IProductControlPanelEditUI final : public CListUI {
	public:
		void Init();
		void Update(const EnProductEditUIType&,std::string addFlag = "");
		void Clear();

	};


	class IProductSystemStatusEditUI final : public CListContainerElementUI {
	public:
		void Set();
		void Update();
		sk::packet Get() const;
	};

	class IProductSystemCaptureEditUI final : public CListContainerElementUI {
	public:
		void Set();
		void Update();
		sk::packet Get() const;
	};

	class IProductSystemConEditUI final : public CListContainerElementUI {
	public:
		void Set();
		void Update(const sk::packet&);
		sk::packet Get();
	};

	class IProductSystemConProcessEditUI final : public CListContainerElementUI {
	public:
		void Set();
		bool Get(sk::packet&);
	};


























	class IProductScreenShotEditUI final : public CListContainerElementUI {
	public:
		void Set(const sk::packet&);
		sk::packet Get() const;
	};










	class IProductSystemEditUI final : public CListContainerElementUI {
	public:
		//void Set(const shared::SystemProduct&);
		//std::shared_ptr<shared::SystemProduct> Get();
	};

	class IProductExchangeEditUI final : public CListContainerElementUI {
	public:
		//void Set(const shared::LPProduct&);
		//std::shared_ptr<shared::LPProduct> Get();
	};

	class IProductMTGatewayEditUI final : public CListContainerElementUI {
	public:
		//void Set(const shared::MTGatewayProduct&, sk::container::set<shared::LPProduct>&);
		//std::shared_ptr<shared::MTGatewayProduct> Get();
	};

	class IProductMTFeederEditUI final : public CListContainerElementUI {
	public:
		//void Set(const shared::MTFeederProduct&);
		//std::shared_ptr<shared::MTFeederProduct> Get();
	};

	class IBridgeTradeRecordEditUI final : public CListContainerElementUI {
	public:
		IBridgeTradeRecordEditUI();
		~IBridgeTradeRecordEditUI();
	public:
		//void Set(const shared::BridgeTradeRecord&);
		//std::shared_ptr<shared::BridgeTradeRecord> Get();
	};

	class IProductEditUI final : public CListUI {
	public:
		void Parse();
		sk::packet Packet();
	};

}///namespace client




/// /*新生联创（上海）**/
/// /*2020年01月12日 0:18:50|820**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_UIPRODUCT_H__F641F_C67BF4B0_5D0641478E5A45A_F092__TAIL__