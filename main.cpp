/**
* \file
* The most simple example.
* 
*/

#include <thread>
#include <vector>
#include <sstream>
#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <nlohmann/json.hpp>

using namespace curlpp::options;

using namespace nlohmann::json_abi_v3_11_3;

void get_card(int card_ind)
{

    
	try
	{
		// Set the URL.
        std::string url = "https://ringsdb.com/api/public/card/0100" + std::to_string(card_ind);
		

        // The first easiest example is to retreive the content of
        // a web page and put it in a stream.
        //curlpp::options::Url(url);

        // You don't need to use just the standard outputs. You
        // can use any stream:
        std::ostringstream os;
        os << curlpp::options::Url(url);

        std::string json_str = os.str();

        std::cout << json_str << '\n';
        json data = json::parse(json_str);
	}

	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
    
}

int main(){

    std::vector<std::jthread> threads;
    for(int i = 1; i <= 9; i++) threads.emplace_back(get_card,i);

}