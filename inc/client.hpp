#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "option.hpp"

class Client {
public:
    Client(const std::string& lockfilePath);
    ~Client();
    std::string getFriendCounts();
	std::string getBlockPlayer();

private:
    std::string lockfilePath;
    std::string port;
	std::string pid;
    std::string password;
	std::string req_url;

	std::string encodedAuth;

    void parse_lockfile();
	void make_url();
	void base64();
	std::string performGetRequest(const std::string& url);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    std::string base64_encode(const std::string& in);
};

#endif // CLIENT_HPP
