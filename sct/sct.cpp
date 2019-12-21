
#include <deque>
#include "DownLoadCapFile.h"
#include "sct.h"
#include "SCTConfig.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include "LibLsp/lsp/general/exit.h"
#include "LibLsp/lsp/general/initialized.h"
#include "LibLsp/lsp/windows/MessageNotify.h"
#include "LibLsp/lsp/JavaExtentions/addOverridableMethods.h"
#include "LibLsp/lsp/language/language.h"
#include "LibLsp/JsonRpc/Condition.h"


namespace lsp {
	class Log;
}

using namespace std;
using lsp::Log;



MethodType sct_DownLoadCapFile::kMethodType = "sct/download_cap";

MethodType sct_Transmit::kMethodType = "sct/transmit";

MethodType sct_Connect::kMethodType = "sct/connect";

MethodType sct_Disconnect::kMethodType = "sct/disconnect";

MethodType sct_InstalllApplet::kMethodType = "sct/install_applet";

MethodType sct_gp_auth::kMethodType = "sct/gp_auth";

MethodType sct_SetWindowsPos::kMethodType = "sct/set_windows_pos";

MethodType sct_SetWindowsVisible::kMethodType = "sct/set_windows_visible";


MethodType sct_NotifyJcvmOutput::kMethodType = "sct/notify_jcvm_output";

MethodType sct_Launch::kMethodType = "sct/launch";

MethodType sct_SetProtocol::kMethodType = "sct/set_protocol";

MethodType sct_CheckBeforeLaunch::kMethodType = "sct/check_before_launch";

MethodType sct_GetCardInfo::kMethodType = "sct/get_card_info";

MethodType sct_NotifyDisconnect::kMethodType = "sct/notify_disconnect";
MethodType sct_TerminateLaunch::kMethodType = "sct/terminate_launch";
MethodType sct_initialize::kMethodType = "sct/initialize";


 SCTConfig* SCTConfig::newInstance(const wstring& file_path, string& error)
{
 	 if(!boost::filesystem::exists(file_path))
 	 {
		 error = "file no exists.";
		 return nullptr;
 	 }
	 using namespace rapidjson;
	 using namespace std;
	 std::unique_ptr<SCTConfig>  sct = std::make_unique<SCTConfig>();
	 try
	 {
		 std::wifstream ifs(file_path);
		 WIStreamWrapper isw(ifs);

		 Document d;
		 d.ParseStream(isw);

		
		 JsonReader reader{ &d };
		 Reflect(reader, *sct.get());
	 }
 	catch (std::exception& e)
 	{
		string  temp = "Reflect failed. exception info:";
			
		temp +=	e.what();
		error = temp;
		sct.get()->broken = true;
		sct.get()->error = temp;	
 	}
	return sct.release();
}


SmartCardTool::SmartCardTool():  m_cmdPort(0), m_eventPort(0), m_curProtocol(SctProtocol::T01), log(nullptr)
{
	m_ipAddr = "127.0.0.1";
}

void AddNotifyJsonRpcMethod(sct::ProtocolJsonHandler& handler)
{
	handler.method2notification[Notify_Exit::kMethodType] = [](Reader& visitor)
	{
		return Notify_Exit::notify::ReflectReader(visitor);
	};
	handler.method2notification[Notify_InitializedNotification::kMethodType] = [](Reader& visitor)
	{
		return Notify_InitializedNotification::notify::ReflectReader(visitor);
	};

	
	handler.method2notification[Notify_LogMessage::kMethodType] = [](Reader& visitor)
	{
		return Notify_LogMessage::notify::ReflectReader(visitor);
	};
	handler.method2notification[Notify_ShowMessage::kMethodType] = [](Reader& visitor)
	{
		return Notify_ShowMessage::notify::ReflectReader(visitor);
	};
	
	handler.method2notification[Notify_sendNotification::kMethodType] = [](Reader& visitor)
	{
		return Notify_sendNotification::notify::ReflectReader(visitor);
	};
	
	handler.method2notification[lang_actionableNotification::kMethodType] = [](Reader& visitor)
	{
		return lang_actionableNotification::notify::ReflectReader(visitor);
	};
	handler.method2notification[lang_progressReport::kMethodType] = [](Reader& visitor)
	{
		return lang_progressReport::notify::ReflectReader(visitor);
	};
	

	handler.method2notification[sct_NotifyJcvmOutput::kMethodType] = [](Reader& visitor)
	{
		return sct_NotifyJcvmOutput::notify::ReflectReader(visitor);
	};
	handler.method2notification[sct_NotifyDisconnect::kMethodType] = [](Reader& visitor)
	{
		return sct_NotifyDisconnect::notify::ReflectReader(visitor);
	};
 	
}

sct::ProtocolJsonHandler::ProtocolJsonHandler()
{
	AddNotifyJsonRpcMethod(*this);

	method2response[sct_DownLoadCapFile::kMethodType ] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_DownLoadCapFile::response::ReflectReader(visitor);
	};
	method2response[sct_Connect::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_Connect::response::ReflectReader(visitor);
	};
	method2response[sct_SetProtocol::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_SetProtocol::response::ReflectReader(visitor);
	};
	method2response[sct_gp_auth::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_gp_auth::response::ReflectReader(visitor);
	};
	method2response[sct_InstalllApplet::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_InstalllApplet::response::ReflectReader(visitor);
	};
	method2response[sct_Transmit::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_Transmit::response::ReflectReader(visitor);
	};
 	
	method2response[sct_GetCardInfo::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_GetCardInfo::response::ReflectReader(visitor);
	};
 	
	method2response[sct_Launch::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_Launch::response::ReflectReader(visitor);
	};
	method2response[sct_CheckBeforeLaunch::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_CheckBeforeLaunch::response::ReflectReader(visitor);
	};
 	

	method2response[sct_initialize::kMethodType] = [](Reader& visitor)
	{
		if (visitor.HasMember("error"))
			return 	Rsp_Error::ReflectReader(visitor);

		return sct_initialize::response::ReflectReader(visitor);
	};
	
}
bool SmartCardTool::check_sct_alive()
{
	if (sct)
	{
		return true;
	}
 	if(log)
 	{
		wstring strPrompt = L"sct is not alvie.";
		log->log(Log::Level::SEVERE, strPrompt);
 	}
	return false;
}

bool SmartCardTool::initialize(int processId)
{

 	if(!check_sct_alive())
 	{
		return false;
 	}
	sct_initialize::request request;
	request.params.processId = processId;
	auto  eventFuture = std::make_shared< Condition< LspMessage > >();
	sct->sendRequest(request, [&](std::unique_ptr<LspMessage> msg)
		{
			eventFuture->notify(std::move(msg));
			return true;
		});
	auto msg = eventFuture->wait(100000);
	if (!msg)
	{
		return false;
	}
	auto  result = dynamic_cast<sct_initialize::response*>(msg.get());
	if (result)
	{
		sctServerCapabilities  _lsServerCapabilities;
		_lsServerCapabilities.swap(result->result.capabilities);
		//Notify_InitializedNotification::notify _notify;
		//sct->sendNotification(_notify);
		return true;
	}
	else
	{
		auto error = reinterpret_cast<Rsp_Error*>(msg.get());
		log->log(Log::Level::SEVERE, error->error.ToString());
		return false;
	}

}



SmartCardTool::~SmartCardTool()
{


}

bool SmartCardTool::GetCardInfo(CardInfoType type_, std::vector<unsigned char>& out)
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_GetCardInfo::request request;
	request.params.type_ = type_;
 	
	auto  data = sct->waitResponse(request, 40000);

	if (!data)
	{
		if (log)
		{
			wstring strPrompt = L"GetCardInfo request timeout.";
			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}
	auto rsp = dynamic_cast<sct_GetCardInfo::response*>(data.get());
	if (!rsp)
	{
		if (log)
		{
			string strPrompt = "GetCardInfo request error." + data.get()->ToJson();
			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}
	if (rsp->result.state)
	{
		out.swap(rsp->result.data.value());
		return  true;
	}
	else
	{
		if (log)
		{
			string strPrompt = "GetCardInfo failed. Reason:";
			strPrompt += rsp->result.info.value();
			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}

	return true;
}



bool SmartCardTool::Launch()
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_Launch::request request;
	auto  data = sct->waitResponse(request, 100000);

	if (!data)
	{
		if (log)
		{
			wstring strPrompt = L"Launch request timeout.";
			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}
	auto rsp = dynamic_cast<sct_Launch::response*>(data.get());
	if (!rsp)
	{
		if (log)
		{
			string strPrompt = "Launch request error." + data.get()->ToJson();

			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}
	if (rsp->result.state)
	{
		if (log)
		{
			log->log(Log::Level::INFO, L"Launch successfully");
		}
		if(rsp->result.info)
		{
			m_ipAddr.swap(rsp->result.info.value().host);
			m_cmdPort = rsp->result.info.value().cmd_port;
			m_eventPort = rsp->result.info.value().event_port;
		}
	}
	else
	{
		if (log)
		{
			string strPrompt = "Launch failed. Reason:";
			strPrompt += rsp->result.error.value();
			log->log(Log::Level::SEVERE, strPrompt);
		}
		return false;
	}
	return true;
}

void SmartCardTool::TerminateLaunch()
{


	if (!check_sct_alive())
	{
		return ;
	}
	sct_TerminateLaunch::notify notify;
	sct->sendNotification(notify);
	connect_state = false;
	return  ;
}

void SmartCardTool::show_message(lsMessageType type_,
	const std::string& msg)
{
	if (!check_sct_alive())
	{
		return ;
	}
	Notify_ShowMessage::notify notify;
	notify.params.type = type_;
	notify.params.message = msg;
	sct->sendNotification(notify);
}

bool SmartCardTool::CheckBeforeLaunch()
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_CheckBeforeLaunch::request request;
	auto  data = sct->waitResponse(request, 50000);

	if (!data)
	{
		wstring strPrompt = L"CheckBeforeLaunch request timeout.";
		if(log)
		{
			log->log(Log::Level::SEVERE, strPrompt);
		}

		return false;
	}
	auto rsp = dynamic_cast<sct_CheckBeforeLaunch::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "CheckBeforeLaunch request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		return true;
	}
	else
	{
		string strPrompt = "Check Before Launch JCVM failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	return true;
}


bool SmartCardTool::Connect(SctProtocol protocol)
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_Connect::request request;

	request.params.protocol = protocol;

	auto  data = sct->waitResponse(request, 40000);

	if (!data)
	{
		wstring strPrompt = L"Connect request timeout.";
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_Connect::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "Connect request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		connect_state = true;
		return true;
	}
	else
	{
		string strPrompt = "Connect failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}

}




void  SmartCardTool::DisConnect()
{
	if (!check_sct_alive())
	{
		return ;
	}
	sct_Disconnect::notify notify;
	sct->sendNotification(notify);
	connect_state = false;
	return ;
}

bool SmartCardTool::DownLoadCapFile(const string& strCapFileName)
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_DownLoadCapFile::request request;
	request.params.uri.raw_uri_ = make_file_scheme_uri(strCapFileName);
	
	auto  data = sct->waitResponse(request, 40000);
	
	if(!data)
	{
		wstring strPrompt = L"DownLoadCapFile request timeout.";
		log->log(Log::Level::SEVERE,strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_DownLoadCapFile::response*>(data.get());
	if(!rsp)
	{
		string strPrompt = "DownLoadCapFile request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if(rsp->result.state)
	{
		string strPrompt = "DownLoadCapFile successfully";
		if(rsp->result.info)
		{
			strPrompt = rsp->result.info.value();
		}
		log->log(Log::Level::INFO, strPrompt);
		return true;
	}
	else
	{
		string strPrompt = "DownLoadCapFile failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}

}

void SmartCardTool::SetWindowsVisible(SetWindowVisibleParams& params)
{
	if (!check_sct_alive())
	{
		return ;
	}
	sct_SetWindowsVisible::notify notify;
	notify.params.swap(params);
	sct->sendNotification(notify);

}

void SmartCardTool::SetWindowPos(SetWindowPosParams& params)
{
	sct_SetWindowsPos::notify notify;
	notify.params.swap(params);
	sct->sendNotification(notify);
}

bool SmartCardTool::SetProtocol(SctProtocol protocol)
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_SetProtocol::request request;
 	
	request.params.protocol = protocol;
 	
	auto  data = sct->waitResponse(request, 40000);

	if (!data)
	{
		wstring strPrompt = L"SetProtocol request timeout.";
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_SetProtocol::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "SetProtocol request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		m_curProtocol = protocol;
		return true;
	}
	else
	{
		string strPrompt = "SetProtocol failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	
}

bool SmartCardTool::GpAuth()
{

	if (!check_sct_alive())
	{
		return false;
	}
	sct_gp_auth::request request;

	auto  data = sct->waitResponse(request, 100000);

	if (!data)
	{
		wstring strPrompt = L"gp_auth request timeout.";
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_gp_auth::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "gp_auth request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		string strPrompt = "gp_auth request successfully.";
		if (rsp->result.info)
			strPrompt = rsp->result.info.value();
		
		log->log(Log::Level::INFO, strPrompt);
		return true;
	}
	else
	{
		string strPrompt = "gp_auth failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}

}

bool SmartCardTool::InstallApplet(InstallAppletParams& params)
{
	if (!check_sct_alive())
	{
		return false;
	}
	sct_InstalllApplet::request request;
	request.params.swap(params);
	auto  data = sct->waitResponse(request);

	if (!data)
	{
		wstring strPrompt = L"Install Applet request timeout.";
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_InstalllApplet::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "Install Applet request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		string strPrompt = "Install Applet successfully.";
		if (rsp->result.info)
			strPrompt = rsp->result.info.value();
		log->log(Log::Level::INFO, strPrompt);
		return true;
	}
	else
	{
		string strPrompt = "Install Applet failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
}


bool SmartCardTool::Transmit(const std::vector<unsigned char>& cmdApdu, std::vector<unsigned char>& rspApdu)
{

	if (!check_sct_alive())
	{
		return false;
	}
	sct_Transmit::request request;
	request.params.command = cmdApdu;
	
	auto  data = sct->waitResponse(request);

	if (!data)
	{
		wstring strPrompt = L"Transmit request timeout.";
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	auto rsp = dynamic_cast<sct_Transmit::response*>(data.get());
	if (!rsp)
	{
		string strPrompt = "Transmit request error." + data.get()->ToJson();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	if (rsp->result.state)
	{
		rspApdu.swap(rsp->result.data.value());
		//log->log(Log::Level::INFO, L"Transmit successfully");
		return true;
	}
	else
	{
		string strPrompt = "Transmit failed. Reason:";
		strPrompt += rsp->result.info.value();
		log->log(Log::Level::SEVERE, strPrompt);
		return false;
	}
	

}
