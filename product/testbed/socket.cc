#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <gtest/gtest.h>
#include "common/utility/logging.h"

#define CHECK_WSA_ERROR(msg, isWrong)                                              \
    if (isWrong)                                                                   \
    {                                                                              \
        LOG(ERROR) << "wsa " << msg << " failed with errno " << WSAGetLastError(); \
    }                                                                              \
    ASSERT_FALSE(isWrong);

#define CHECK_SSL_ERROR(msg, isWrong)             \
    if (isWrong)                                  \
    {                                             \
        LOG(ERROR) << "SSL " << msg << " failed"; \
        ERR_print_errors_fp(stdout);              \
    }                                             \
    ASSERT_FALSE(isWrong);

std::wstring Utf8ToUnicode(const std::string &utf8)
{
    LPCCH ptr = utf8.c_str();
    int size = MultiByteToWideChar(CP_UTF8, 0, ptr, -1, NULL, NULL);
    std::wstring wstrRet(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, ptr, -1, (LPWSTR)wstrRet.c_str(), size);
    return wstrRet;
}

void showCerts(SSL *ssl)
{
    char line[1024];
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL)
    {
        LOG(INFO) << "have X.509 certificate!";
        std::memset(line, 0, sizeof(line));
        X509_NAME_oneline(X509_get_subject_name(cert), line, sizeof(line) - 1);
        LOG(INFO) << "[SUBJECT] " << line;
        std::memset(line, 0, sizeof(line));
        X509_NAME_oneline(X509_get_issuer_name(cert), line, sizeof(line) - 1);
        LOG(INFO) << "[ISSUER ] " << line;
        X509_free(cert);
    }
    else
        LOG(INFO) << "have no X.509 certificate!" << std::endl;
}

void ssl_put(SSL *ssl)
{
    char buf[1024];
    std::memset(buf, 0, sizeof(buf));
    int len = SSL_read(ssl, buf, sizeof(buf) - 1);
    CHECK_SSL_ERROR("connect", len < 0)
    if (len > 0)
        LOG(INFO) << "[server] " << buf;
}

void ssl_send(SSL *ssl, const std::string &cmd)
{
    SSL_write(ssl, cmd.c_str(), (int)cmd.size());
    ssl_put(ssl);
}

TEST(SMTP, SendEmail)
{
    std::wcout.imbue(std::locale(""));

    const char *host = "smtp.aliyun.com";
    const char *port = "465";

    WSADATA wsa;
    CHECK_WSA_ERROR("startup", WSAStartup(MAKEWORD(2, 2), &wsa) != 0)

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    CHECK_SSL_ERROR("context", ctx == NULL)

    addrinfo hints;
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo *result;
    CHECK_WSA_ERROR("getaddrinfo", getaddrinfo(host, port, &hints, &result) != 0);
    SOCKET sock;
    for (addrinfo *rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == INVALID_SOCKET)
            continue;
        if (connect(sock, rp->ai_addr, (int)rp->ai_addrlen) != SOCKET_ERROR)
            break;
    }
    CHECK_WSA_ERROR("connect", sock == NULL)

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)sock);
    CHECK_SSL_ERROR("connect", SSL_connect(ssl) == -1)
    LOG(INFO) << "connected with " << SSL_get_cipher(ssl) << " encryption";

    showCerts(ssl);

    ssl_put(ssl);
    ssl_send(ssl, "HELO smtp.aliyun.com\r\n");
    ssl_send(ssl, "AUTH LOGIN\r\n");
    ssl_send(ssl, "amF5c2luY29AYWxpeXVuLmNvbQ==\r\n");
    ssl_send(ssl, "ZDQxYjgyQGFsaXl1bg ==\r\n");
    ssl_send(ssl, "MAIL FROM: <jaysinco@aliyun.com>\r\n");
    ssl_send(ssl, "RCPT TO: <jaysinco@163.com>\r\n");
    ssl_send(ssl, "DATA\r\n");
    ssl_send(ssl, "From: jaysinco@aliyun.com\r\nTo: jaysinco@163.com\r\nSUBJECT: meeting report\r\n.\r\n");
    ssl_send(ssl, "QUIT\r\n");

    SSL_shutdown(ssl);
    SSL_free(ssl);
    closesocket(sock);
    SSL_CTX_free(ctx);
    CHECK_WSA_ERROR("cleanup", WSACleanup() == SOCKET_ERROR)
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    testing::InitGoogleTest(&argc, argv);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    RUN_ALL_TESTS();
    google::ShutdownGoogleLogging();
    return 0;
}