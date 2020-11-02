#include "stdafx.h"
#include "UIMain.h"
#include "UIProduct.h"

namespace client {

	UIMain::UIMain() {
		Global::GetObjectNotify()->Append(this, __FUNCTION__);
	}
	UIMain::~UIMain() {
		Global::GetObjectNotify()->Remove(this);
	}

	CControlUI* UIMain::CreateControl(LPCTSTR pstrClassName) {

		return nullptr;
	}

	void UIMain::Notify(TNotifyUI& msg) {
		if (msg.sType == DUI_MSGTYPE_CLICK) {
			if (msg.pSender->GetName() == _TEXT("DEF_5367D1B574F2DA1A8BE2CC5A709DA388")) {//! 最小化
				::ShowWindow(GetHWND(), SW_SHOWMINIMIZED);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_C63FEB4ADE18128AE1C765D021677F64")) {//! 关闭系统
				if (StatusLogin()) {
					Logout();
				}
				Close(IDOK);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_37C0AEB747179B19C2ABC7320C215DD9")) {//! 退出登录
				Logout();
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_95DCE79DA85B064019873074CC016F9D")) {//! 登录
				Logon();
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_AA940C6CDB6BEFC36FB6D931F852BCE8")) {//! 刷新服务器列表
				Global::GetCore()->GetClient()->NetRequestSystemStatusInfo(0);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_6FC26413C6EC784AB8BAE49778F47FE5")) {//! Home
				GetCtrl<CTabLayoutUI>(_TEXT("DEF_CA9A1E7FE99314C4693E3920565131F5"))->SelectItem(0);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_08E2CE8F82F40074B1FDA0E93806ACD5")) {//! 
				GetCtrl<CTabLayoutUI>(_TEXT("DEF_CA9A1E7FE99314C4693E3920565131F5"))->SelectItem(1);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_6640BDF922CB6D00D69CE3191C1F91A6")) {//! 
				GetCtrl<CTabLayoutUI>(_TEXT("DEF_CA9A1E7FE99314C4693E3920565131F5"))->SelectItem(2);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_0FBE882C96DC1392F01FECD6E2B80DDB")) {//! 
				GetCtrl<CTabLayoutUI>(_TEXT("DEF_CA9A1E7FE99314C4693E3920565131F5"))->SelectItem(3);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_657D081C484CAFAAB1A6364E4997F51E")) {//! 保存修改产品配置		
				//auto pListEdit = GetCtrl<IProductEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
				//if (!pListEdit) return;
				//auto pak = pListEdit->Packet();
				//if (!pak.empty()) {
				//	//Global::GetCore()->GetClient()->NetSymbolConUpdateSend(pak);
				//}
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_7C67F08158C0C564E126485C5427E9BE")) {//! 确认修改系统配置
				SystemSetupGet();
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_A238AC461A665414D74F8BCEB3F764E9")) {//! 清理交易记录日志
				//Global::GetObjectNotify()->Push(sk::NotifyPacket((long long)EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_TRADERECORD_CLEAR));
			}
		}///DUI_MSGTYPE_CLICK
		else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED) {
			if (msg.pSender->GetName() == _TEXT("DEF_5B0F87A37DD07AC785551865A46C6158")) {

			}

			//! 关于点差的输入限制 -- 只允许输入正整数
			if (msg.pSender->GetName() == _TEXT("DEF_A7FC9AACE8DCF522C6D377BAFAFBD376") ||
				msg.pSender->GetName() == _TEXT("DEF_2D2CF54FB062AF57E4CEC73B1699CBB0") ||
				msg.pSender->GetName() == _TEXT("DEF_A4B1DD4431B12657EF423BA68D84CCBC") ||
				msg.pSender->GetName() == _TEXT("DEF_DF3DB43DBACF9CB65DE9D5383FF5CC53") ||
				msg.pSender->GetName() == _TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9") ||
				msg.pSender->GetName() == _TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC") ||
				msg.pSender->GetName() == _TEXT("DEF_DD0152AC9693302127869DFDB845EACD") ||
				msg.pSender->GetName() == _TEXT("DEF_F63588E183AF8A06F66118F7C1ECE253"))
			{
				auto pEdit = (CEditUI*)msg.pSender;
				sk::string InputStr = pEdit->GetText().GetData();
				//if (!shared::InputCheck::IntegerlsFix(InputStr, USHRT_MAX, 0)) {
				//	pEdit->SetText(InputStr.c_str());
				//	pEdit->SetSel((long)InputStr.length(), (long)InputStr.length());
				//}
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_DD0152AC9693302127869DFDB845EACD"))
			{
#if 0	//! 针对调整条控件的逻辑流程暂屏蔽
				auto pEdit = (CEditUI*)msg.pSender;
				sk::string InputStr = pEdit->GetText().GetData();
				if (!shared::InputCheck::IntegerlsFix(InputStr, 150, 0))
				{
					pEdit->SetText(InputStr.c_str());
					pEdit->SetSel((long)InputStr.length(), (long)InputStr.length());
				}

				IProductMTGatewayEditUI* pParent = (IProductMTGatewayEditUI*)pEdit->GetParent()->GetParent();
				auto asdlfkj = pParent->GetTag64();
				CSliderUI* pSlider = pParent->GetSubCtrl<CSliderUI>(_TEXT("DEF_C0C4D117C713AFE076EF697F27C4F237"));

				if (InputStr.length())
				{
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(InputStr.c_str());
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(_TEXT("0"));

					auto Val = _ttoi(InputStr.c_str());
					if (Val < 0) {
						pSlider->SetTag(1);//! 负数
					}
					else
					{
						pSlider->SetTag(0);//! 正数
					}
					pSlider->SetValue(0);
					pSlider->SetMaxValue(abs(Val));
					pSlider->SetCanSendMove(true);
				}
				else
				{
					pSlider->SetCanSendMove(false);
				}
#endif
			}
			//! MT帐户
			if (msg.pSender->GetName() == _TEXT("DEF_27AEAC0F9FAB35DA6E71E6B6A1210779"))
			{
				auto pEdit = (CEditUI*)msg.pSender;
				sk::string InputStr = pEdit->GetText().GetData();
				//if (!shared::InputCheck::LoginsFix(InputStr)) {
				//	pEdit->SetText(InputStr.c_str());
				//	pEdit->SetSel((long)InputStr.length(), (long)InputStr.length());
				//}
			}

			//! 时间段沟通限定
			if (msg.pSender->GetName() == _TEXT("DEF_F15D715FF21F3992F459BD4AC7C135A0") ||
				msg.pSender->GetName() == _TEXT("DEF_C9AA0A94D77F635F41CAD104C7C8FCA7") ||
				msg.pSender->GetName() == _TEXT("DEF_2F50FE7DB9BACC7F35CA6926C0EDE13C") ||
				msg.pSender->GetName() == _TEXT("DEF_C6465BB70F9EEB9A41DA4ED9119A573A") ||
				msg.pSender->GetName() == _TEXT("DEF_C6F46D1C4AE3828848DDFE312DEAFA28") ||
				msg.pSender->GetName() == _TEXT("DEF_C78F35DDA841A64330346613C34274EF") ||
				msg.pSender->GetName() == _TEXT("DEF_6FC1B276BA7CC92309C0E8E2E90F3FAA") ||
				msg.pSender->GetName() == _TEXT("DEF_D877E89FFE9A73B20C7DA51400BFF14F") ||
				msg.pSender->GetName() == _TEXT("DEF_C26809B5D882985AC69F2DD2543B7406") ||
				msg.pSender->GetName() == _TEXT("DEF_14D9DB7539860A13FFD0D50DF42B690F") ||
				msg.pSender->GetName() == _TEXT("DEF_05D9388B14343824F5DF71B204D875DA") ||
				msg.pSender->GetName() == _TEXT("DEF_55F0125614A9FBCF5D062CBBAE16EF02") ||
				msg.pSender->GetName() == _TEXT("DEF_DC396A58E7D0CA95E010A49BE1370AE3") ||
				msg.pSender->GetName() == _TEXT("DEF_1024DBA72F584A4B9B667E97354CD117")) {

				auto pEdit = (CEditUI*)msg.pSender;
				sk::string InputStr = pEdit->GetText().GetData();
				//if (!shared::InputCheck::TimeFrameFix(InputStr)) {
				//	pEdit->SetText(InputStr.c_str());
				//	pEdit->SetSel((long)InputStr.length(), (long)InputStr.length());
				//}
			}
		}///DUI_MSGTYPE_TEXTCHANGED
		else if (msg.sType == DUI_MSGTYPE_ITEMSELECT) {
			if (msg.pSender->GetName() == _TEXT("DEF_8628E28E9EF8CFA1307E0C8DE875A5A1")) {//! 抛单价格类型
#if 0
				auto pComboOrdType = (CComboUI*)msg.pSender;
				shared::tradeorder::EnOrdType ordType = shared::tradeorder::EnOrdType(sk::StringConvert::WStringToMBytes(pComboOrdType->GetItemAt(pComboOrdType->GetCurSel())->GetUserData().GetData()).c_str()[0]);

				switch (ordType)
				{
				case shared::tradeorder::EnOrdType::EN_ORDER_TYPE_MARKET:
				{
					GetCtrl<CComboUI>(_TEXT("DEF_89052B592B0691BFBD292D4E210BCB1E"))->SetVisible(true);
					GetCtrl<CComboUI>(_TEXT("DEF_7749E33DB5DD08290AF3CE2862426735"))->SetVisible(false);
				}break;
				case shared::tradeorder::EnOrdType::EN_ORDER_TYPE_LIMIT:
				{
					GetCtrl<CComboUI>(_TEXT("DEF_89052B592B0691BFBD292D4E210BCB1E"))->SetVisible(false);
					GetCtrl<CComboUI>(_TEXT("DEF_7749E33DB5DD08290AF3CE2862426735"))->SetVisible(true);
				}break;
				}

				msg.pSender->GetParent()->NeedParentUpdate();
#endif
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_6071F6A479D14DE184CC6BE2394A941C")) {
#if 0
				auto pCombo = (CComboUI*)msg.pSender;
				auto sel = pCombo->GetCurSel();
				if (sel < 0) return;
				auto ctrl = pCombo->GetItemAt(sel);
				if (!ctrl) return;
				auto seltext = ctrl->GetText();
				GetCtrl<CEditUI>(_TEXT("DEF_5B0F87A37DD07AC785551865A46C6158"))->SetText(seltext);

				auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
				if (pList) {
					auto pListNode = (IProductTreeNodeUI*)pList->GetItemAtByTagUINT64(ctrl->GetTagUINT64());
					if (pListNode) {
						pList->SelectItem((int)pListNode->GetTag());
					}
				}
				//ThemeEdit(ctrl->GetTag());
#endif
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391")) {

				auto pListTree = (CListUI*)msg.pSender;
				auto nCurSel = pListTree->GetCurSel();
				if (nCurSel < 0) return;
				auto pSel = pListTree->GetItemAt(nCurSel);
				//Global::GetCore()->GetClient()->NetRequestTargetScreenShot(pSel->GetTagUINT64());
				auto sk = 0;
#if 0

				sk::packet pak;
				auto pListEdit = GetCtrl<IProductEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
				if (!pListEdit) return;
				pListEdit->RemoveAll();
				pListEdit->SetCache(pak);
				pListEdit->Parse();
				pListEdit->NeedParentUpdate();
#endif
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_252C209091A2AC3D370447CFC125F4E3")) {//! 树控
				auto pTreeView = (CTreeViewUI*)msg.pSender;
				GetCtrl<CTabLayoutUI>(_TEXT("DEF_CA9A1E7FE99314C4693E3920565131F5"))->SelectItem(pTreeView->GetCurSel());
			}

		}
		else if (msg.sType == DUI_MSGTYPE_VALUECHANGED_MOVE)
		{
			CSliderUI* pSlider = (CSliderUI*)msg.pSender;
			IProductMTGatewayEditUI* pParent = (IProductMTGatewayEditUI*)pSlider->GetParent()->GetParent();
			if (pSlider->GetMaxValue() == 0 || pSlider->GetMaxValue() == pSlider->GetChangeStep())
			{
				pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(_TEXT("0"));
				pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(_TEXT("0"));
			}
			else
			{
				if (pSlider->GetTag() == 0)
				{
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(tostring(pSlider->GetMaxValue() - pSlider->GetValue()).c_str());
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(tostring(pSlider->GetValue()).c_str());
				}
				else
				{
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(tostring(0 - pSlider->GetMaxValue() + pSlider->GetValue()).c_str());
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(tostring(0 - pSlider->GetValue()).c_str());
				}
			}
		}///DUI_MSGTYPE_VALUECHANGED_MOVE
		else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
		{
			CSliderUI* pSlider = (CSliderUI*)msg.pSender;
			IProductMTGatewayEditUI* pParent = (IProductMTGatewayEditUI*)pSlider->GetParent()->GetParent();
			if (pSlider->GetMaxValue() == 0 || pSlider->GetMaxValue() == pSlider->GetChangeStep())
			{
				pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(_TEXT("0"));
				pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(_TEXT("0"));
			}
			else
			{
				if (pSlider->GetTag() == 0)
				{
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(tostring(pSlider->GetMaxValue() - pSlider->GetValue()).c_str());
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(tostring(pSlider->GetValue()).c_str());
				}
				else
				{
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(tostring(0 - pSlider->GetMaxValue() + pSlider->GetValue()).c_str());
					pParent->GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(tostring(0 - pSlider->GetValue()).c_str());
				}
			}
		}///DUI_MSGTYPE_VALUECHANGED

	/*WindowImplBase::Notify(msg);*/
	}
	LRESULT UIMain::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		UINT iKey = (UINT)wParam;
		if (iKey == VK_RETURN) {
			if (StatusLogin()) {
				auto pEditSearch = GetCtrl<CEditUI>(_TEXT("DEF_5B0F87A37DD07AC785551865A46C6158"));
				if (pEditSearch) {
					pEditSearch->Activate();
					pEditSearch->SetFocus();
				}
			}
		}

		bHandled = true;
		return 0;
	}
	LRESULT UIMain::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		auto mminfo = (MINMAXINFO*)lParam;
		mminfo->ptMaxPosition.x = 0;
		mminfo->ptMaxPosition.y = 0;
		auto width = ::GetSystemMetrics(SM_CXSCREEN);
		auto hegth = ::GetSystemMetrics(SM_CYSCREEN);

		APPBARDATA ad;
		ZeroMemory(&ad, sizeof APPBARDATA);
		ad.cbSize = sizeof APPBARDATA;
		::SHAppBarMessage(ABM_GETTASKBARPOS, &ad);
		switch (ad.uEdge) {
		case 0:
		case 2: {
			mminfo->ptMaxSize.x = width - (ad.rc.right - ad.rc.left);
			mminfo->ptMaxSize.y = hegth - 0;
		}break;
		case 1:
		case 3: {
			mminfo->ptMaxSize.x = width - 0;
			mminfo->ptMaxSize.y = hegth - (ad.rc.bottom - ad.rc.top);
		}break;
		}

		bHandled = false;
		return 0;
	}
	LRESULT UIMain::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(GetHWND());
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(sk::string(GetWindowClassName()).append(_TEXT(".xml")).c_str(), (LPCTSTR)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse 'ui.xml'");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		InitWindow();
		bHandled = TRUE;
		return 0;
	}
	void UIMain::InitWindow() {
		::SetWindowPos(GetHWND(), nullptr, 0, 0, 500, 350, SWP_NOACTIVATE);
		CenterWindow();
		GetCtrl<CTreeViewUI>(_TEXT("DEF_252C209091A2AC3D370447CFC125F4E3"))->SelectItem(0);
	}

	int UIMain::Logon() {
		Global::GetCore()->GetClient()->Open();
		return 0;
	}
	int UIMain::Logout() {
		StatusLogin(false);
		Global::GetCore()->GetClient()->Close();

		::SetWindowPos(GetHWND(), nullptr, 0, 0, 500, 350, SWP_NOACTIVATE);
		CenterWindow();

		GetCtrl<CTabLayoutUI>(_TEXT("DEF_A70C424EF50CEE83AB773256B8C50D41"))->SelectItem(0);

		{//! 清理UI控件相关数据
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			if (pList) {
				pList->RemoveAll();
			}
		}

		return 0;
	}
#if 0
	void UIMain::ThemeEdit(const shared::MarketType& systemProductID) {
		auto targetQ = GetSystemProductTreeObj(systemProductID);
		if (targetQ.empty()) return;
		ThemeSystemProductID(systemProductID);



	}
	sk::container::map<UITreeNodeSYSProduct*, sk::container::set<UITreeNodeLPProduct*>> \
		UIMain::GetSystemProductTreeObj(const shared::MarketType& id) {
		sk::container::map<UITreeNodeSYSProduct*, sk::container::set<UITreeNodeLPProduct*>> result;
		m_UIProductTreeMapQ.iterate(
			[&](const auto& systreeObj, auto& tree, const auto&, auto& itbreak) {
				if (systreeObj->Product().ID == id) {
					itbreak = true;
					result.push(systreeObj, tree);
				}
			});
		return result;
	}
	sk::container::set<UITreeNodeLPProduct*>* UIMain::GetSystemProductTreeObj(const shared::SymbolA& symbol) {
		sk::container::set<UITreeNodeLPProduct*>* result = nullptr;
		m_UIProductTreeMapQ.iterate(
			[&](const auto& systreeObj, auto& tree, const auto&, auto& itbreak) {
				if (systreeObj->Product().Symbol == symbol) {
					itbreak = true;
					result = &tree;
				}
			});
		return result;
	}
#endif
	void UIMain::SystemSetupGet()
	{
#if 0
		shared::BridgeSystemSetup setup;

		//! 系统平台模式
		auto comboSystemPlatformMode = GetCtrl<CComboUI>(_TEXT("DEF_8949506871A4AF501EEA44A91D231E72"));
		if (comboSystemPlatformMode)
		{
			setup.SystemPlatformMode = decltype(setup.SystemPlatformMode)(_tcstol(comboSystemPlatformMode->GetItemAt(comboSystemPlatformMode->GetCurSel())->GetUserData().GetData(), nullptr, 10));
		}

		//! 指定帐户过桥定单组合类型
		auto comboSystemOrderMode = GetCtrl<CComboUI>(_TEXT("DEF_21F2213FFDB0B20283BB2BFDF76DB275"));
		if (comboSystemOrderMode)
		{
			setup.SystemOrderMode = decltype(setup.SystemOrderMode)(_tcstol(comboSystemOrderMode->GetItemAt(comboSystemOrderMode->GetCurSel())->GetUserData().GetData(), nullptr, 10));
		}
		auto editAssignLogins = GetCtrl<CEditUI>(_TEXT("DEF_27AEAC0F9FAB35DA6E71E6B6A1210779"));
		if (editAssignLogins)
		{
			std::vector<sk::stringw> logins;
			sk::Helper::OneUnknownParseW(editAssignLogins->GetText().GetData(), '\\', logins);
			for (const auto& login : logins)
			{
				setup.AppendAssigenLogin(wcstoull(login.c_str(), nullptr, 10));
			}
		}

		//! 网关定单处理类型
		auto comboGatewayOrderTaskProcessingType = GetCtrl<CComboUI>(_TEXT("DEF_F9BF5940C6F5446F545B9EBD839EEC95"));
		if (comboGatewayOrderTaskProcessingType)
		{
			auto userdata = comboGatewayOrderTaskProcessingType->GetItemAt(comboGatewayOrderTaskProcessingType->GetCurSel())->GetUserData().GetData();
			if (userdata)
			{
				setup.GatewayOrderTaskProcessingType = decltype(setup.GatewayOrderTaskProcessingType)(_tcstol(userdata, nullptr, 10));
			}
		}

		//! 网关定单任务超时间间隔
		auto comboGatewayOrderTaskTimeout = GetCtrl<CComboUI>(_TEXT("DEF_B82C33159C313613E3BBEA7E7AAA59AE"));
		if (comboGatewayOrderTaskTimeout)
		{
			setup.GatewayOrderTaskTimeout = _tcstol(comboGatewayOrderTaskTimeout->GetItemAt(comboGatewayOrderTaskTimeout->GetCurSel())->GetUserData().GetData(), nullptr, 10);
		}

		//! 过桥定单任务超时间间隔
		auto comboBridgeTradeOrderTaskTimeout = GetCtrl<CComboUI>(_TEXT("DEF_926A9E5258E650697CDFF64EE52C6859"));
		if (comboBridgeTradeOrderTaskTimeout)
		{
			setup.BridgeTradeOrderTaskTimeout = _tcstol(comboBridgeTradeOrderTaskTimeout->GetItemAt(comboBridgeTradeOrderTaskTimeout->GetCurSel())->GetUserData().GetData(), nullptr, 10);
		}

		//! 交易所报价超时间间隔
		auto comboBridgeMarketDataInactiveTimeout = GetCtrl<CComboUI>(_TEXT("DEF_C120AFB5960DFC72F1253071D90AAE90"));
		if (comboBridgeMarketDataInactiveTimeout)
		{
			setup.BridgeMarketDataInactiveTimeout = _tcstol(comboBridgeMarketDataInactiveTimeout->GetItemAt(comboBridgeMarketDataInactiveTimeout->GetCurSel())->GetUserData().GetData(), nullptr, 10);
		}

		//! 网关定单价格类型有效期类型
		auto comboGatewayOrderPriceType = GetCtrl<CComboUI>(_TEXT("DEF_8628E28E9EF8CFA1307E0C8DE875A5A1"));
		if (comboGatewayOrderPriceType)
		{
			auto userdata = comboGatewayOrderPriceType->GetItemAt(comboGatewayOrderPriceType->GetCurSel())->GetUserData().GetData();
			if (userdata)
			{
				setup.GatewayOrderPriceType = decltype(setup.GatewayOrderPriceType)(sk::StringConvert::WStringToMBytes(userdata)[0]);
			}

		}
		//! 交易定单价格时间类型
		auto comboOrderTimeInForce = GetCtrl<CComboUI>(_TEXT("DEF_89052B592B0691BFBD292D4E210BCB1E"));
		if (comboOrderTimeInForce)
		{
			auto userdata = comboOrderTimeInForce->GetItemAt(comboOrderTimeInForce->GetCurSel())->GetUserData().GetData();
			if (userdata)
			{
				setup.GatewayOrderTimeInForce = decltype(setup.GatewayOrderTimeInForce)(sk::StringConvert::WStringToMBytes(userdata)[0]);
			}
		}

		//! 网关定单成交价格类型
		auto comboGatewayOrderTradePriceFlag = GetCtrl<CComboUI>(_TEXT("DEF_5BE420660E68327B1CB45B6D68B66E0A"));
		if (comboGatewayOrderTradePriceFlag)
		{
			auto userdata = comboGatewayOrderTradePriceFlag->GetItemAt(comboGatewayOrderTradePriceFlag->GetCurSel())->GetUserData().GetData();
			if (userdata)
			{
				setup.GatewayOrderTradePriceFlag = decltype(setup.GatewayOrderTradePriceFlag)(_tcstol(userdata, nullptr, 10));
			}
		}

		//! 聚合报价开关
		if (GetCtrl<COptionUI>(_TEXT("DEF_C7AE6101D6A5DAB2E0DB07649862FF88"))->IsSelected())
		{
			setup.AggregateQuoteSwitch = decltype(setup.AggregateQuoteSwitch)::EN_AGGREGATE_QUOTE_FALG_ENABLE;
		}
		else
		{
			setup.AggregateQuoteSwitch = decltype(setup.AggregateQuoteSwitch)::EN_AGGREGATE_QUOTE_FALG_DISABLE;
		}

		//! 开发者调试模式
		if (GetCtrl<COptionUI>(_TEXT("DEF_6377042096DC8814BC20775A000A0559"))->IsSelected())
		{
			setup.DevelopDebugSwitch = decltype(setup.DevelopDebugSwitch)::EN_DEVELOP_DEBUG_MODE_ENABLE;
		}
		else
		{
			setup.DevelopDebugSwitch = decltype(setup.DevelopDebugSwitch)::EN_DEVELOP_DEBUG_MODE_DISABLE;
		}


		//! 交易工作时间段
		setup.SystemOperatingTimePeriod[1].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_F15D715FF21F3992F459BD4AC7C135A0"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[1].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_C9AA0A94D77F635F41CAD104C7C8FCA7"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[2].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_C6F46D1C4AE3828848DDFE312DEAFA28"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[2].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_C78F35DDA841A64330346613C34274EF"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[3].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_6FC1B276BA7CC92309C0E8E2E90F3FAA"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[3].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_D877E89FFE9A73B20C7DA51400BFF14F"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[4].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_C26809B5D882985AC69F2DD2543B7406"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[4].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_14D9DB7539860A13FFD0D50DF42B690F"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[5].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_05D9388B14343824F5DF71B204D875DA"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[5].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_55F0125614A9FBCF5D062CBBAE16EF02"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[6].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_DC396A58E7D0CA95E010A49BE1370AE3"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[6].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_1024DBA72F584A4B9B667E97354CD117"))->GetText().GetData()), true);
		setup.SystemOperatingTimePeriod[0].TimeBegin = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_2F50FE7DB9BACC7F35CA6926C0EDE13C"))->GetText().GetData()), false);
		setup.SystemOperatingTimePeriod[0].TimeEnd = sk::time::Time::TimePeriodMade(sk::StringConvert::WStringToMBytes(GetCtrl<CEditUI>(_TEXT("DEF_C6465BB70F9EEB9A41DA4ED9119A573A"))->GetText().GetData()), true);

		for (size_t i = 0; i < _countof(setup.SystemOperatingTimePeriod); ++i)
		{
			if (setup.SystemOperatingTimePeriod[i].TimeBegin > setup.SystemOperatingTimePeriod[i].TimeEnd)
			{
				MessageBox(Global::GetCore()->GetUIMain()->GetHWND(), _TEXT("交易时间段设置不允许开始时间大于结束时间情况."), _TEXT("错误"), MB_TOPMOST);
				return;
			}
		}

		if (!Global::GetCore()->GetClient()->NetBridgeSystemSetupSend(setup))
		{
			MessageBox(Global::GetCore()->GetUIMain()->GetHWND(), _TEXT("更新完成."), _TEXT("完成"), MB_TOPMOST);
		}
		else
		{
			MessageBox(Global::GetCore()->GetUIMain()->GetHWND(), _TEXT("更新失败."), NULL, MB_TOPMOST);
		}
#endif
	}


	void UIMain::SystemSetupSet()
	{
#if 0
		const shared::BridgeSystemSetup setup = Global::SetupGet()->BridgeSystemSetup();

		//! 系统平台模式
		auto comboSystemPlatformMode = GetCtrl<CComboUI>(_TEXT("DEF_8949506871A4AF501EEA44A91D231E72"));
		if (comboSystemPlatformMode)
		{
			for (int i = 0; i < comboSystemPlatformMode->GetCount(); ++i)
			{
				if ((UINT)setup.SystemPlatformMode == \
					_tcstol(comboSystemPlatformMode->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboSystemPlatformMode->SelectItem(i);
					break;
				}
			}
		}

		//! 指定帐户过桥定单组合类型
		auto comboSystemOrderMode = GetCtrl<CComboUI>(_TEXT("DEF_21F2213FFDB0B20283BB2BFDF76DB275"));
		if (comboSystemOrderMode)
		{
			for (int i = 0; i < comboSystemOrderMode->GetCount(); ++i)
			{
				if ((UINT)setup.SystemOrderMode == \
					_tcstol(comboSystemOrderMode->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboSystemOrderMode->SelectItem(i);
					break;
				}
			}
		}
		auto editAssignLogins = GetCtrl<CEditUI>(_TEXT("DEF_27AEAC0F9FAB35DA6E71E6B6A1210779"));
		if (editAssignLogins)
		{
			sk::stringw strAssignLogins;
			for (size_t i = 0; i < _countof(setup.AssigenLogin); ++i)
			{
				if (setup.AssigenLogin[i] > 0)
				{
					strAssignLogins.append(tostringw(setup.AssigenLogin[i])).append(L"\\");
				}
			}
			if (!strAssignLogins.empty())
			{
				editAssignLogins->SetText(strAssignLogins.c_str());
			}
		}

		//! 网关定单处理类型
		auto comboGatewayOrderTaskProcessingType = GetCtrl<CComboUI>(_TEXT("DEF_F9BF5940C6F5446F545B9EBD839EEC95"));
		if (comboGatewayOrderTaskProcessingType)
		{
			for (int i = 0; i < comboGatewayOrderTaskProcessingType->GetCount(); ++i)
			{
				if (UINT(setup.GatewayOrderTaskProcessingType) == \
					_tcstol(comboGatewayOrderTaskProcessingType->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboGatewayOrderTaskProcessingType->SelectItem(i);
					break;
				}
			}
		}

		//! 网关定单超时间间隔
		auto comboGatewayOrderTaskTimeout = GetCtrl<CComboUI>(_TEXT("DEF_B82C33159C313613E3BBEA7E7AAA59AE"));
		if (comboGatewayOrderTaskTimeout)
		{
			for (int i = 0; i < comboGatewayOrderTaskTimeout->GetCount(); ++i)
			{
				if (setup.GatewayOrderTaskTimeout == \
					_tcstol(comboGatewayOrderTaskTimeout->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboGatewayOrderTaskTimeout->SelectItem(i);
					break;
				}
			}
		}

		//! 过桥定单任务超时间间隔
		auto comboBridgeTradeOrderTaskTimeout = GetCtrl<CComboUI>(_TEXT("DEF_926A9E5258E650697CDFF64EE52C6859"));
		if (comboBridgeTradeOrderTaskTimeout)
		{
			for (int i = 0; i < comboBridgeTradeOrderTaskTimeout->GetCount(); ++i)
			{
				if (setup.BridgeTradeOrderTaskTimeout == \
					_tcstol(comboBridgeTradeOrderTaskTimeout->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboBridgeTradeOrderTaskTimeout->SelectItem(i);
					break;
				}
			}
		}

		//! 交易所报价超时间间隔
		auto comboBridgeMarketDataInactiveTimeout = GetCtrl<CComboUI>(_TEXT("DEF_C120AFB5960DFC72F1253071D90AAE90"));
		if (comboBridgeMarketDataInactiveTimeout)
		{
			for (int i = 0; i < comboBridgeMarketDataInactiveTimeout->GetCount(); ++i)
			{
				if (setup.BridgeMarketDataInactiveTimeout == \
					_tcstol(comboBridgeMarketDataInactiveTimeout->GetItemAt(i)->GetUserData().GetData(), nullptr, 10))
				{
					comboBridgeMarketDataInactiveTimeout->SelectItem(i);
					break;
				}
			}
		}

		//! 交易定单价格类型
		auto comboGatewayOrderPriceType = GetCtrl<CComboUI>(_TEXT("DEF_8628E28E9EF8CFA1307E0C8DE875A5A1"));
		if (comboGatewayOrderPriceType)
		{
			for (int i = 0; i < comboGatewayOrderPriceType->GetCount(); ++i)
			{
				auto userdata = comboGatewayOrderPriceType->GetItemAt(i)->GetUserData().GetData();
				if (setup.GatewayOrderPriceType == \
					decltype(setup.GatewayOrderPriceType)(sk::StringConvert::WStringToMBytes(userdata)[0]))
				{
					comboGatewayOrderPriceType->SelectItem(i);
					break;
				}
			}
		}

		//! 交易定单价格时间类型
		auto comboGatewayOrderTimeInForce = GetCtrl<CComboUI>(_TEXT("DEF_89052B592B0691BFBD292D4E210BCB1E"));
		if (comboGatewayOrderTimeInForce)
		{
			for (int i = 0; i < comboGatewayOrderTimeInForce->GetCount(); ++i)
			{
				auto userdata = comboGatewayOrderTimeInForce->GetItemAt(i)->GetUserData().GetData();
				if (setup.GatewayOrderTimeInForce == \
					decltype(setup.GatewayOrderTimeInForce)(sk::StringConvert::WStringToMBytes(userdata)[0]))
				{
					comboGatewayOrderTimeInForce->SelectItem(i);
					break;
				}
			}
		}

		//! 网关定单成交价格类型
		auto comboGatewayOrderTradePriceFlag = GetCtrl<CComboUI>(_TEXT("DEF_5BE420660E68327B1CB45B6D68B66E0A"));
		if (comboGatewayOrderTradePriceFlag)
		{
			for (int i = 0; i < comboGatewayOrderTradePriceFlag->GetCount(); ++i)
			{
				auto userdata = comboGatewayOrderTradePriceFlag->GetItemAt(i)->GetUserData().GetData();
				if (setup.GatewayOrderTradePriceFlag == \
					decltype(setup.GatewayOrderTradePriceFlag)(_tcstol(userdata, nullptr, 10)))
				{
					comboGatewayOrderTradePriceFlag->SelectItem(i);
					break;
				}
			}
		}

		//! 交易工作时间段
		for (size_t wday = 0; wday < _countof(shared::BridgeSystemSetup::SystemOperatingTimePeriod); ++wday)
		{
			switch (wday)
			{
			case 1:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_F15D715FF21F3992F459BD4AC7C135A0"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_C9AA0A94D77F635F41CAD104C7C8FCA7"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 2:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_C6F46D1C4AE3828848DDFE312DEAFA28"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_C78F35DDA841A64330346613C34274EF"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 3:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_6FC1B276BA7CC92309C0E8E2E90F3FAA"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_D877E89FFE9A73B20C7DA51400BFF14F"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 4:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_C26809B5D882985AC69F2DD2543B7406"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_14D9DB7539860A13FFD0D50DF42B690F"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 5:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_05D9388B14343824F5DF71B204D875DA"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_55F0125614A9FBCF5D062CBBAE16EF02"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 6:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_DC396A58E7D0CA95E010A49BE1370AE3"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_1024DBA72F584A4B9B667E97354CD117"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			case 0:
			{
				GetCtrl<CEditUI>(_TEXT("DEF_2F50FE7DB9BACC7F35CA6926C0EDE13C"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeBegin)).c_str());
				GetCtrl<CEditUI>(_TEXT("DEF_C6465BB70F9EEB9A41DA4ED9119A573A"))->\
					SetText(sk::StringConvert::MBytesToTString(sk::time::Time::TimePeriodUnMade(setup.SystemOperatingTimePeriod[wday].TimeEnd)).c_str());
			}break;
			}
		}


		//! 聚合报价开关
		if (setup.AggregateQuoteSwitch == decltype(setup.AggregateQuoteSwitch)::EN_AGGREGATE_QUOTE_FALG_ENABLE)
		{
			GetCtrl<COptionUI>(_TEXT("DEF_C7AE6101D6A5DAB2E0DB07649862FF88"))->Selected(true);//!启用
		}
		else
		{
			GetCtrl<COptionUI>(_TEXT("DEF_F2B04474B2C0AA89BFCC9782FFDB0055"))->Selected(true);//!禁用
		}

		//! 开发调试模式
		if (setup.DevelopDebugSwitch == decltype(setup.DevelopDebugSwitch)::EN_DEVELOP_DEBUG_MODE_ENABLE)
		{
			GetCtrl<COptionUI>(_TEXT("DEF_6377042096DC8814BC20775A000A0559"))->Selected(true);//!启用
		}
		else
		{
			GetCtrl<COptionUI>(_TEXT("DEF_0D56334536ED73507BDCCAA975D24F51"))->Selected(true);//!禁用
		}
#endif
	}










	void UIMain::OnNotifyLow(const sk::NotifyPacket* pNotifyPak)
	{
#if 0
		switch (sk::network::EnNetCmd(pNotifyPak->cmd))
		{
		case EnMessageUI::EN_MESSAGE_UI_SYSTEM_STATUS_INFO:
		{
			if (Global::SharedGet()->m_SystemStatusInfoQ.empty()) break;
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			if (!pList) break;
			pList->RemoveAll();
			CDialogBuilder builder;
			Global::SharedGet()->m_SystemStatusInfoQ.iterate(
				[&](const auto& ID, auto& info)
				{
					auto pListLine = (IProductTreeNodeUI*)builder.Create(_TEXT("UIProductTreeNode.xml"));
					pList->Add(pListLine);
					pListLine->SetTag((int)0);
					pListLine->SetTagUINT64(info.ID);
					pListLine->SetSystemID(info.ID);
					pListLine->SetSystemIP(info.IPAddr);
					pListLine->SetSystemVersion(info.SystemVersion);
					pListLine->SetSystemCPUUsage(info.iCPUUsage);
				});
		}break;
		case EnMessageUI::EN_MESSAGE_UI_SYSTEM_SCREEN_SHOT:
		{
			auto pListTree = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			auto nCurSel = pListTree->GetCurSel();
			if (nCurSel < 0) return;
			auto pSel = pListTree->GetItemAt(nCurSel);
			Global::SharedGet()->m_SystemScreenShutQ.search(
				pSel->GetTagUINT64(),
				[&](const auto& id, auto& stream)
				{
					auto pListEdit = GetCtrl<IProductEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
					if (!pListEdit) return;
					pListEdit->RemoveAll();
					pListEdit->SetCache(stream);
					pListEdit->Parse();
					pListEdit->NeedParentUpdate();
				});
		}break;
		case EnMessageUI::EN_MESSAGE_UI_DISCONNECT:
		{
		}break;
		case EnMessageUI::EN_MESSAGE_UI_CONNECT:
		{
		}break;
		case EnMessageUI::EN_MESSAGE_UI_LOGON_SUCCESS:
		{
			MINMAXINFO MMI = { 0 };
			::SendMessage(GetHWND(), WM_GETMINMAXINFO, 0, (LPARAM)&MMI);
			::SetWindowPos(GetHWND(), HWND_TOP, 0, 0, MMI.ptMaxSize.x, MMI.ptMaxSize.y, SWP_NOACTIVATE);
			CenterWindow();
			GetCtrl<CTabLayoutUI>(_TEXT("DEF_A70C424EF50CEE83AB773256B8C50D41"))->SelectItem(1);
			StatusLogin(true);
		}break;
		case EnMessageUI::EN_MESSAGE_UI_LOGON_FAILED:
		{
			MessageBox(NULL, _TEXT("与服务器建立联系失败!"), NULL, MB_TOPMOST);
			Global::GetCore()->GetClient()->Close();
		}break;
#if 0
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_BRIDGESYSTEMTIME_UPDATE:
		{
			auto editTime = GetCtrl<CEditUI>(_TEXT("DEF_FC5908D5D9AC711A438DE33A445AE75E"));
			if (editTime)
			{
				//auto serverTime = Global::SetupGet()->BridgeSystemTime();
				//struct tm _tm;
				//gmtime_s(&_tm, &serverTime);
				//TCHAR timestr[100] = { 0 };
				//_tcsftime(timestr, 100, sk::global_sk_ftime_utc_format, &_tm);
				//editTime->SetText(timestr);
				//editTime->NeedParentUpdate();
			}
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_BRIDGESYSTEMSETUP_UPDATE:
		{
			SystemSetupSet();
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_STATUS_CONNECT: {

		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_STATUS_DISCONNECT: {
			if (StatusLogin()) {
				Logout();
			}
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_TRADERECORD_CLEAR: {
			//Global::SharedGet()->m_TradeRecordQ.clear();
			//auto pList = GetCtrl<CListUI>(_TEXT("DEF_69F087E67D2D28F7B24D16DB549F9D7A"));
			//if (!pList) { break; }
			//pList->RemoveAll();
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_TRADERECORD_INIT: {
#if 0
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_69F087E67D2D28F7B24D16DB549F9D7A"));
			if (!pList) { break; }
			pList->RemoveAll();
			Global::SharedGet()->m_TradeRecordQ.iterate(
				[&](const auto& timeUpdate, shared::BridgeTradeRecord& logField)
				{
					auto pListElem = new CListTextElementUI();
					pList->Add(pListElem);
					//!@ 时间
					pListElem->SetText(0 * 2, sk::StringConvert::MBytesToWString(sk::time::Time::TimestampToTimeStr(logField.TimeUpdate)).c_str());
					//!@ MT品种
					pListElem->SetText(1 * 2, sk::StringConvert::MBytesToWString(logField.Symbol.symbol).c_str());
					//!@ MT请求量
					pListElem->SetText(2 * 2, sk::Helper::FloatToStr(logField.Order.OrderQty, logField.Digits, false).c_str());
					//!@ MT确认量
					pListElem->SetText(3 * 2, sk::Helper::FloatToStr(logField.Order.TradeQty, logField.Digits, false).c_str());
					//!@ MT请求订单价
					pListElem->SetText(4 * 2, sk::Helper::FloatToStr(logField.Order.OrderPx, logField.Digits, false).c_str());
					//!@ LP成交均价
					pListElem->SetText(5 * 2, sk::Helper::FloatToStr(logField.Order.TradeAvgPx, logField.Digits, false).c_str());
					//!@ LP成交最价
					pListElem->SetText(6 * 2, sk::Helper::FloatToStr(logField.Order.TradeLastPx, logField.Digits, false).c_str());
					//!@ 多空
					pListElem->SetText(7 * 2, logField.Order.MTHedgeKey.OrdSide == shared::tradeorder::EnOrdSide::EN_ORDER_SIDE_BUY ? L"多单" : L"空单");
					//!@ 系统单号
					pListElem->SetText(8 * 2, sk::Log::Format(L"{:X}", logField.Order.OrderID).c_str());
					//!@ 交易所产品ID
					pListElem->SetText(9 * 2, sk::Log::Format(L"{:X}", logField.Order.ExchangeProdID).c_str());
					//!@ 网关所产品ID
					pListElem->SetText(10 * 2, sk::Log::Format(L"{:X}", logField.Order.MTHedgeKey.GatewayProductID).c_str());
					//!@ Login
					pListElem->SetText(11 * 2, tostringw(logField.Order.MTHedgeKey.Login).c_str());
					//!@ 系统耗时
					pListElem->SetText(12 * 2, tostringw(logField.TimeProcessBridge).c_str());
					//!@ 订单结果
					pListElem->SetText(13 * 2, sk::StringConvert::MBytesToWString(SKAPIRES_DESC(logField.Order.SKRetcode)).c_str());

					pList->NeedParentUpdate();
				});

			if (pList->GetCount()) {
				pList->SelectItem(0);
			}
#endif
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_TRADERECORD_UPDATE: {
#if 0
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_69F087E67D2D28F7B24D16DB549F9D7A"));
			if (!pList) break;
			auto updates = \
				Global::SharedGet()->m_TradeRecordQ.search(
					[&](const shared::BridgeTradeRecord& logField)->bool
					{
						if (logField.TimeUpdate > (INT64)pList->GetTagUINT64())
						{
							return true;
						}
						return false;
					});

			if (updates)
			{
				for (const auto& logField : *updates)
				{
					pList->SetTagUINT64(logField.TimeUpdate);
					auto pListElem = new CListTextElementUI();
					pList->AddAt(pListElem, 0);
					{
						//!@ 时间
						pListElem->SetText(0 * 2, sk::StringConvert::MBytesToWString(sk::time::Time::TimestampToTimeStr(logField.TimeUpdate)).c_str());
						//!@ MT品种
						pListElem->SetText(1 * 2, sk::StringConvert::MBytesToWString(logField.Symbol.symbol).c_str());
						//!@ MT请求量
						pListElem->SetText(2 * 2, sk::Helper::FloatToStr(logField.Order.OrderQty, logField.Digits, false).c_str());
						//!@ MT确认量
						pListElem->SetText(3 * 2, sk::Helper::FloatToStr(logField.Order.TradeQty, logField.Digits, false).c_str());
						//!@ MT请求订单价
						pListElem->SetText(4 * 2, sk::Helper::FloatToStr(logField.Order.OrderPx, logField.Digits, false).c_str());
						//!@ LP成交均价
						pListElem->SetText(5 * 2, sk::Helper::FloatToStr(logField.Order.TradeAvgPx, logField.Digits, false).c_str());
						//!@ LP成交最价
						pListElem->SetText(6 * 2, sk::Helper::FloatToStr(logField.Order.TradeLastPx, logField.Digits, false).c_str());
						//!@ 多空
						pListElem->SetText(7 * 2, logField.Order.MTHedgeKey.OrdSide == shared::tradeorder::EnOrdSide::EN_ORDER_SIDE_BUY ? L"多单" : L"空单");
						//!@ 系统单号
						pListElem->SetText(8 * 2, sk::Log::Format(L"{:X}", logField.Order.OrderID).c_str());
						//!@ 交易所产品ID
						pListElem->SetText(9 * 2, sk::Log::Format(L"{:X}", logField.Order.ExchangeProdID).c_str());
						//!@ 网关所产品ID
						pListElem->SetText(10 * 2, sk::Log::Format(L"{:X}", logField.Order.MTHedgeKey.GatewayProductID).c_str());
						//!@ Login
						pListElem->SetText(11 * 2, tostringw(logField.Order.MTHedgeKey.Login).c_str());
						//!@ 系统耗时
						pListElem->SetText(12 * 2, tostringw(logField.TimeProcessBridge).c_str());
						//!@ 订单结果
						pListElem->SetText(13 * 2, sk::StringConvert::MBytesToWString(SKAPIRES_DESC(logField.Order.SKRetcode)).c_str());

						pList->NeedParentUpdate();
					}
				}
			}
			pList->UnSelectAllItems();
			if (pList->GetCount()) {
				pList->SelectItem(0);
			}
#endif
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_NETWORK_SYMBOLCON_REQUEST_ACK: {
#if 0
			if (pNotifyPak->len <= 0) {
				MessageBox(NULL, _TEXT("修改失败"), NULL, MB_TOPMOST);
			}
			else {
				shared::SystemProduct sysProd;
				sk::container::set<shared::LPProduct> lpProds;
				sk::container::set<shared::MTFeederProduct> mtFeederProds;
				sk::container::set<shared::MTGatewayProduct> mtGatewayProds;
				if (!shared::ProductCon::DeconBuffer(sk::packet(pNotifyPak->data, pNotifyPak->len), sysProd, lpProds, mtFeederProds, mtGatewayProds)) {
					Global::SharedGet()->m_SystemSymbolConQ.push(sysProd.ID, sk::packet(pNotifyPak->data, pNotifyPak->len));
					auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
					if (!pList) break;
					if (pList->GetCurSel() >= 0) {
						pList->SelectItem(pList->GetCurSel());
					}
				}

				MessageBox(NULL, _TEXT("修改完成"), NULL, MB_TOPMOST);
			}
#endif
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_PRODUCT_INIT: {
#if 0
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			if (!pList) break;
			pList->RemoveAll();
			CDialogBuilder builder;
			Global::SharedGet()->m_SystemProductQ.iterate(
				[&](const auto& sysProdID, shared::SystemProduct& sysProdObj) {
					auto pListLine = (IProductTreeNodeUI*)builder.Create(_TEXT("UIProductTreeNode.xml"));
					pList->Add(pListLine);
					pListLine->SetTag((int)sysProdObj.Decimals);
					pListLine->SetTagUINT64(sysProdID);
					pListLine->SetSymbol(sk::StringConvert::MBytesToTString(sysProdObj.Symbol.symbol));
				});
#endif
		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_MARKETDATA_CACHE_GETTO: {

		}break;
		case EnLocalMessage::EN_LOCAL_MESSAGE_SYSTEM_SYMBOLCON_GETTO: {
#if 0
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			if (!pList) break;
			Global::SharedGet()->m_SystemProductQ.iterate(
				[&](const auto& sysid, auto& sysCon)
				{
					auto pListNode = (IProductTreeNodeUI*)pList->GetItemAtByTagUINT64(sysid);
					if (pListNode) {
						pListNode->SetTag(sysCon.Decimals);
					}
				});
			if (pList->GetCount()) {
				pList->SelectItem(0);
			}
			GetCtrl<CButtonUI>(_TEXT("DEF_AA940C6CDB6BEFC36FB6D931F852BCE8"))->SetEnabled(true);
#endif
		}break;
#endif
		}///switch (EnLocalMessage(pNotifyPak->cmd))
#endif
	}

	void UIMain::OnNotifyHigh(const sk::NotifyPacket* pNotifyPak)
	{
#if 0
		switch (EnMessageUI(pNotifyPak->cmd))
		{
		case EnLocalMessage::EN_LOCAL_MESSAGE_MARKETDATA_UPDATE: {


		}break;
		}
#endif
	}

}///namespace client