#include <thread>
#include <atomic>
#include <mutex>

#include <vector>
#include <sstream>
#include <iostream>

#include <random>
#include <chrono>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <nlohmann/json.hpp>

using namespace curlpp::options;

using namespace nlohmann::json_abi_v3_11_3;

static std::mutex gLock;
static std::mutex writer_lock;


struct Card {
    int threat = 0;
    int willpower = 0;
    int attack = 0;
    int defense = 0;
    int health = 4;
};

std::string int_str_with_zeros(int i, int n_digits){
    std::string s(n_digits,'0');
    int base = 1;
    for(int d = 0; d < n_digits; d++){
        s[n_digits-1-d] = '0' + char((i / base) % 10);
        base *= 10;
    }
    return s;
}

int get_card(int pack_id, int card_ind)
{
    std::ostringstream os;
    const std::string base_url = "https://ringsdb.com/api/public/card/";
    std::string url = base_url
        + int_str_with_zeros(pack_id, 2)
        + int_str_with_zeros(card_ind, 3);

    os << curlpp::options::Url(url);


    std::string json_str = os.str();
    json data = json::parse(json_str);
    if(data.find("error")!=data.end()) return 1;

    writer_lock.lock();
    std::cout << data.at("name")
        << '\t' << "Pack name: " << data.at("pack_name")
        << '\t' << "Position: " << data.at("position")
        << '\t' << "Code: " << data.at("code") << '\n';
    writer_lock.unlock();

    return 0;
}

int get_card_fake(int pack_id, int card_ind){


    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    
    std::uniform_int_distribution<> dist{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});


    //if(pack_id>100) return 1;
    if(card_ind>100) return 1;
    writer_lock.lock();
    std::cout << "Pack " << pack_id << '\t' << "Card " << card_ind << '\n';
    writer_lock.unlock();
    return 0;
}

void process_cards(){

    static int next_pack_id = 1;
    static int next_card_id = 1;

    while(true){

        

        gLock.lock();
        if(next_pack_id>5){
            gLock.unlock();
            break;       // Look at first 5 packs
        }
        int current_pack_id = next_pack_id;
        int current_card_id = next_card_id++;
        gLock.unlock();

        bool first_card = current_card_id == 1;

        bool got_card = get_card(current_pack_id, current_card_id) == 0;
        
        // No more valid cards in this pack -> try next pack
        gLock.lock();
        if(current_pack_id==next_pack_id && current_card_id==999){
            next_pack_id++;
            next_card_id = 1;
        }   
        gLock.unlock();
    }
}

int main(){
    


    std::vector<std::jthread> threads;
    for(int i = 1; i <= 100; i++) threads.emplace_back(process_cards);
    //process_cards();
}