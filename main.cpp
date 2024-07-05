/**
* \file
* The most simple example.
* 
*/

#include <thread>
#include <vector>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>


using namespace curlpp::options;

void get_card(int card_ind)
{

    
	try
	{
		// Our request to be sent.
		curlpp::Easy myRequest;

		// Set the URL.
        std::string url = "https://ringsdb.com/api/public/card/0100" + std::to_string(card_ind);
		myRequest.setOpt<curlpp::options::Url>(url);

		// Send request and get a result.
		// By default the result goes to standard output.
		myRequest.perform();
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

    std::vector<std::thread> threads;
    for(int i = 1; i <= 9; i++) threads.emplace_back(get_card,i);

    for(auto& th : threads) th.join();

}