// client.cpp
#include "option.hpp"

Client::Client(const std::string& lockfilePath) : lockfilePath(lockfilePath) {
    #ifdef DEBUG_ON
    std::cout << "Client constructor called" << std::endl;
    #endif

    parse_lockfile();
    base64();
}

Client::~Client() {
    #ifdef DEBUG_ON
    std::cout << "Client destructor called" << std::endl;
    #endif
}

void Client::make_url() {
    req_url = "https://127.0.0.1:" + port;
    #ifdef DEBUG_ON
    std::cout << "URL created: " << req_url << std::endl;
    #endif
}

void Client::parse_lockfile() {
    std::ifstream lockfile(lockfilePath);
    if (!lockfile.is_open()) {
        throw std::runtime_error("Failed to open lockfile at path: " + lockfilePath);
    }

    std::string line;
    std::getline(lockfile, line);
    lockfile.close();

    std::istringstream iss(line);
    std::string name, pid, protocol;
    std::getline(iss, name, ':');
    std::getline(iss, pid, ':');
    std::getline(iss, port, ':');
    std::getline(iss, password, ':');
    std::getline(iss, protocol, ':');
    
    #ifdef DEBUG_ON
    std::cout << "Parsed lockfile - Port: " << port << ", Password: " << password << std::endl;
    #endif

    make_url();
}

void Client::base64() {
    std::string auth = "riot:" + password;
    encodedAuth = base64_encode(auth);

    #ifdef DEBUG_ON
    std::cout << "Base64 encoded auth: " << encodedAuth << std::endl;
    #endif
}

std::string Client::base64_encode(const std::string& in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

size_t Client::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string Client::performGetRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Basic " + encodedAuth).c_str());

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // CURL을 UTF-8 인코딩으로 설정
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "UTF-8");
		
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            #ifdef DEBUG_ON
            std::cout << "Curl request successful" << std::endl;
            #endif
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

/*************************************************************************
 * @brief 친구 수 카운트 func
 * @note GET endpoint "/lol-chat/v1/friend-counts"
 * @return std::string 친구 수를 포함하는 JSON 문자열.
**************************************************************************/
std::string Client::getFriendCounts() {
    std::string get_url = req_url + "/lol-chat/v1/friend-counts";
    #ifdef DEBUG_ON
    std::cout << "Requesting friend counts from URL: " << get_url << std::endl;
    #endif
    return performGetRequest(get_url);
}

/*************************************************************************
 * @brief 친구 수 카운트 func
 * @note GET endpoint "/lol-chat/v1/friend-counts"
 * @return std::string 친구 수를 포함하는 JSON 문자열.
**************************************************************************/
std::string Client::getBlockPlayer() {
    std::string get_url = req_url + "/lol-chat/v1/blocked-players";
    #ifdef DEBUG_ON
    std::cout << "Requesting blocked players from URL: " << get_url << std::endl;
    #endif
    return performGetRequest(get_url);
}
