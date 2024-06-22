#include "option.hpp"

using json = nlohmann::json;

void print_json_pretty(const std::string& json_str) {
    json j = json::parse(json_str);
    std::cout << j.dump(4) << std::endl; // JSON 데이터를 보기 좋게 4칸 들여쓰기로 출력
}

int main() {
    // 콘솔 창을 UTF-8 인코딩으로 설정
    SetConsoleOutputCP(CP_UTF8);

    try {
        #ifdef DEBUG_ON
        std::cout << "Creating Client object" << std::endl;
        #endif

        Client client("C:/Riot Games/League of Legends/lockfile");

        #ifdef DEBUG_ON
        std::cout << "Client object created" << std::endl;
        #endif

        std::string response = client.getFriendCounts();
        std::cout << "First Response: ";
        print_json_pretty(response);

        response = client.getBlockPlayer();
        std::cout << "Second Response: ";
        print_json_pretty(response);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    #ifdef DEBUG_ON
    std::cout << "Program ended" << std::endl;
    #endif
    
    system("pause");

    return 0;
}
