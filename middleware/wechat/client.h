#include <string>
#include <vector>
#define CURL_STATICLIB 
#include <curl/curl.h>
#include <tinyxml2.h>

namespace cxx {

namespace wx {

struct LoginInfo {
    std::string url;
    std::string fileUrl;
    std::string syncUrl;
    int logintime;
    std::string skey;
    std::string wxsid;
    std::string wxuin;
    std::string passTicket;
    std::string baseRequest;
};

struct HttpResponse {
    int code;
    std::string headers;
    std::string body;
};

class Client {
public:
    Client();
    ~Client();
    void login();
private:
    void waitForScanQRCode();
    void processLogInfo(const std::string &context);
    void iterXmlNode(tinyxml2::XMLElement *node);
    HttpResponse httpGet(const std::string &url, const std::vector<std::string> &headers);
    HttpResponse httpPost(
        const std::string &url,
        const std::vector<std::string> &headers,
        const std::string &data);
    LoginInfo logInfo;
    CURL *curl;
};

} // namespace wx

} // namespace cxx