#include <Windows.h>
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <regex>
#include <sstream>
#include <ctime>
#include <nlohmann/json.hpp>
#include "client.h"
#define QUOT(x) (std::string("\"")+x+"\"")
#define MAKE_DETAIL_URL(x) (std::string("https://")+x+"/cgi-bin/mmwebwx-bin")

namespace cxx {

namespace wx {

const std::string BASE_DOMAIN = "https://login.weixin.qq.com";
const std::string USER_AGENT = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36";

Client::Client() {
    curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "can't init curl easy interface!";
        exit(-1);
    }
}

Client::~Client() {
    if (curl)
        curl_easy_cleanup(curl);
}

void Client::login() {
    waitForScanQRCode();
    LOG(INFO) << "loading the contact, this may take a little while";

    std::ostringstream wxInitUrl;
    time_t localTime = std::time(nullptr);
    wxInitUrl << logInfo.url << "/webwxinit" << "?r=" << (-1 * localTime / 1579) 
        << "&pass_ticket=" << logInfo.passTicket;
    const std::vector<std::string> jsonHeader = {
        "ContentType: application/json; charset=UTF-8",
        "User-Agent: " + USER_AGENT
    };
    auto wxInitResp = httpPost(wxInitUrl.str(), jsonHeader, logInfo.baseRequest);
    LOG(INFO) << wxInitResp.code;
    LOG(INFO) << wxInitResp.headers;
    LOG(INFO) << wxInitResp.body;
}

void Client::waitForScanQRCode() {
    const std::string qrLoginUrl = BASE_DOMAIN + "/jslogin?appid=wx782c26e4c19acffb&fun=new";
    const std::vector<std::string> agentHeader = { "User-Agent: " + USER_AGENT };
    const std::string showQRUrl = "http://api.qrserver.com/v1/create-qr-code/?data=https://login.weixin.qq.com/l/";
    const std::string chromePath = "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
    const std::regex uuid_patt("window.QRLogin.code = (\\d+); window.QRLogin.uuid = \"(\\S+?)\";");
    const std::regex code_patt("window.code=(\\d+)");
    while (true) {
        auto uuidResp = httpGet(qrLoginUrl, agentHeader);
        if (uuidResp.code != 200) {
            LOG(INFO) << "failed to connect jslogin, responseCode=" << uuidResp.code;
            exit(-1);
        }
        std::smatch uuid_match;
        if (!std::regex_search(uuidResp.body, uuid_match, uuid_patt)
            || uuid_match.size() != 3 || uuid_match[1] != "200") {
            LOG(INFO) << "failed to match window.QRLogin.uuid, response=" << QUOT(uuidResp.body);
            exit(-1);
        }
        std::string uuid = uuid_match[2];
        LOG(INFO) << "uuid=" << QUOT(uuid);
        LOG(INFO) << "scan QRCode to login";
        std::string cmd = QUOT(chromePath) + " --new-window " + QUOT(showQRUrl + uuid);
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        CreateProcess(NULL, (char*)cmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        bool hintConfirm = true;
        while (true) {
            std::ostringstream checkUrl;
            time_t localTime = std::time(nullptr);
            checkUrl << "/cgi-bin/mmwebwx-bin/login?loginicon=true&uuid=" << uuid << "&tip=1&r="
                << (-1 * localTime / 1579) << "&_=" << localTime;
            auto checkResp = httpGet(BASE_DOMAIN + checkUrl.str(), agentHeader);
            if (checkResp.code != 200) {
                LOG(INFO) << "failed to connect mmwebwx, responseCode=" << checkResp.code;
                exit(-1);
            }
            std::smatch code_match;
            if (!std::regex_search(checkResp.body, code_match, code_patt) || code_match.size() != 2) {
                LOG(INFO) << "failed to match window.code, response=" << QUOT(checkResp.body);
                exit(-1);
            }
            std::string code = code_match[1];
            if (code == "200") { 
                processLogInfo(checkResp.body);
                return; 
            }
            else if (code == "201") {
                if (hintConfirm) {
                    LOG(INFO) << "confirm on your phone";
                    hintConfirm = false;
                }
            }
            else if (code != "408") { 
                LOG(INFO) << "login time out, reloading QR code";
                break; 
            }
            else ;
            Sleep(500);
        }
    }
}

void Client::processLogInfo(const std::string &context) {
    const std::vector<std::string> agentHeader = { "User-Agent: " + USER_AGENT };
    const std::regex redirect_patt("window.redirect_uri=\"(\\S+)\";");
    std::smatch redirect_match;
    if (!std::regex_search(context, redirect_match, redirect_patt) || redirect_match.size() != 2) {
        LOG(INFO) << "failed to match window.redirect_uri, response=" << QUOT(context);
        exit(-1);
    }
    logInfo.url = redirect_match[1];
    LOG(INFO) << "window.redirect_uri=" << QUOT(logInfo.url);
    auto redirectResp = httpGet(logInfo.url, agentHeader);
    if (redirectResp.code != 200 && redirectResp.code != 301) {
        LOG(INFO) << "failed to connect redirect, responseCode=" << redirectResp.code;
        exit(-1);
    }
    tinyxml2::XMLDocument doc;
    doc.Parse(redirectResp.body.c_str());
    if (doc.ErrorID() != 0) {
        LOG(INFO) << "failed to pase redirect xml, errno=" << doc.ErrorID();
        exit(-1);
    }
    iterXmlNode(doc.RootElement());
    if (logInfo.skey == "" || logInfo.wxsid == "" || logInfo.wxuin == "" || logInfo.passTicket == "") {
        LOG(ERROR) << "wechat account may be LIMITED to log in WEB wechat, dom=" << QUOT(redirectResp.body);
        exit(-1);
    }
    logInfo.url = logInfo.url.substr(0, logInfo.url.rfind('/'));
    if (logInfo.url.find("wx2.qq.com") != std::string::npos) {
        logInfo.fileUrl = MAKE_DETAIL_URL("file.wx2.qq.com");
        logInfo.syncUrl = MAKE_DETAIL_URL("webpush.wx2.qq.com");
    } else if (logInfo.url.find("wx8.qq.com") != std::string::npos) {
        logInfo.fileUrl = MAKE_DETAIL_URL("file.wx8.qq.com");
        logInfo.syncUrl = MAKE_DETAIL_URL("webpush.wx8.qq.com");
    } else if (logInfo.url.find("qq.com") != std::string::npos) {
        logInfo.fileUrl = MAKE_DETAIL_URL("file.wx.qq.com");
        logInfo.syncUrl = MAKE_DETAIL_URL("webpush.wx.qq.com");
    } else if (logInfo.url.find("web2.wechat.com") != std::string::npos) {
        logInfo.fileUrl = MAKE_DETAIL_URL("file.web2.wechat.com");
        logInfo.syncUrl = MAKE_DETAIL_URL("webpush.web2.wechat.com");
    } else if (logInfo.url.find("wechat.com") != std::string::npos) {
        logInfo.fileUrl = MAKE_DETAIL_URL("file.web.wechat.com");
        logInfo.syncUrl = MAKE_DETAIL_URL("webpush.web.wechat.com");
    } else {
        logInfo.fileUrl = logInfo.url;
        logInfo.syncUrl = logInfo.url;
    }
    nlohmann::json baseq = {
        {"BaseRequest", {
            {"Skey", logInfo.skey},
            {"Sid", logInfo.wxsid},
            {"Uin", logInfo.wxuin},
            {"DeviceID", logInfo.passTicket}
        }}
    };
    logInfo.baseRequest = baseq.dump();
    LOG(INFO) << "url=" << QUOT(logInfo.url);
    LOG(INFO) << "fileUrl=" << QUOT(logInfo.fileUrl);
    LOG(INFO) << "syncUrl=" << QUOT(logInfo.syncUrl);
    LOG(INFO) << "skey=" << QUOT(logInfo.skey);
    LOG(INFO) << "wxsid=" << QUOT(logInfo.wxsid);
    LOG(INFO) << "wxuin=" << QUOT(logInfo.wxuin);
    LOG(INFO) << "passTicket=" << QUOT(logInfo.passTicket);
    LOG(INFO) << "baseRequest=" << QUOT(logInfo.baseRequest);
}

void Client::iterXmlNode(tinyxml2::XMLElement *node) {
    while (node) {
        const std::string name = node->Name();
        std::string text = "";
        if (node->GetText())
            text = node->GetText();
        if (name == "skey") logInfo.skey = text;
        else if (name == "wxsid") logInfo.wxsid = text;
        else if (name == "wxuin") logInfo.wxuin = text;
        else if (name == "pass_ticket") logInfo.passTicket = text;
        else ;
        iterXmlNode(node->FirstChildElement());
        node = node->NextSiblingElement();
    }
}

size_t write_into_string(void *ptr, size_t size, size_t nmemb, void *stream) {
	std::string *s = (std::string*)stream;
	(*s).append((char*)ptr, size * nmemb);
	return size * nmemb;
}

HttpResponse httpMethod(
    CURL *curl,
    const std::string &method,
    const std::string &url,
    const std::vector<std::string> &headers,
    const std::string &data) {
    HttpResponse resp = { 0 };
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    struct curl_slist *chunk = NULL;
    for (const auto &header : headers)
        chunk = curl_slist_append(chunk, header.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_into_string);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_into_string);
    curl_easy_setopt(curl, CURLOPT_WRITEHEADER, (void*)&resp.headers);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&resp.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 60000);
    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }
    auto status = curl_easy_perform(curl);
    if (status != CURLE_OK) {
        LOG(INFO) << "failed to perform easy curl: " << curl_easy_strerror(status);
        exit(-1);
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.code);
    curl_slist_free_all(chunk);
    return resp;
}

HttpResponse Client::httpGet( const std::string &url, const std::vector<std::string> &headers) {
    return httpMethod(curl, "GET", url, headers, "");
}

HttpResponse Client::httpPost(
        const std::string &url,
        const std::vector<std::string> &headers,
        const std::string &data) {
    return httpMethod(curl, "POST", url, headers, data);
}

} // namespace wx

} // namespace cxx