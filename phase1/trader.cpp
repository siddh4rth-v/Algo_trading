#include "receiver.h"
#include <vector>
#include "order.cpp"
using namespace std;

int main(int argc, char *argv[]) {
    // Check if there are enough command-line arguments
    

    // Convert command-line arguments to integers
    int arg1 = std::atoi(argv[1]);
  

    // Use conditional statements to execute different code based on arguments
    if (arg1 == 1) {
        Receiver rcv;
        // sleep(5);
        // std::string message = rcv.readIML();
        bool fd = false;
        std::string next = "";
        while(!fd){

            orderslist<std::string, int, char> List;
            // std::cout << message << std::endl;
            // std::cout << message << std::endl;

            std::string message = rcv.readIML();

            message = next + message;

            std::istringstream iss(message);
            std::string token;

            if(message.find('$') != std::string::npos){
                fd = true;
                // std::cout << message;
            }

            // std::cout << message;
            // std::cout << 1 << '\n';


                while (std::getline(iss, token, char(13))) {

                    // if(token.size() >= 11) std::cout << token << std::endl;

                    std::istringstream sub_iss(token);
                    std::string s_t;
                    std::string name;
                    int price;
                    char type;
                    if (std::getline(sub_iss, s_t,' ')) {
                        name = s_t;
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        price = std::stoi(s_t);
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        type = s_t[0];
                    }
                    if(token[token.size()-1] == '#'){
                        List.add_order(name, price, type);
                        // std::cout << s_t[1] << '\n';
                    } else {
                        // if(token.size() > 10) std::cout << token << std::endl;
                        next = token;
                    }
                }
                if(message[message.size()-1] == '$'){
                    int i = message.size()-1;
                    while(message[i] != char(13)){
                        i--;
                    }
                    std::string token;
                    std::string name;
                    std::string s_t;
                    int price;
                    char type;
                    for(int j = i+1; j < message.size()-1; j++){
                        token += message[j];
                    }
                    std::istringstream sub_iss(token);
                    if (std::getline(sub_iss, s_t,' ')) {
                        name = s_t;
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        price = std::stoi(s_t);
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        type = s_t[0];
                    }
                    if(token[token.size()-1] == '#'){
                        List.add_order(name, price, type);
                        // std::cout << s_t[1] << '\n';
                    } else {
                        // if(token.size() > 10) std::cout << token << std::endl;
                        next = token;
                    }

                }
                if(std::getline(iss, token, char(36))){
                    // cout << 1 << endl;
                    std::istringstream sub_iss(token);
                    std::string s_t;
                    std::string name;
                    int price;
                    char type;
                    if (std::getline(sub_iss, s_t,' ')) {
                        name = s_t;
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        price = std::stoi(s_t);
                    }
                    if (std::getline(sub_iss, s_t,' ')) {
                        type = s_t[0];
                    }
                    if(token[token.size()-1] == '#'){
                        List.add_order(name, price, type);
                        // std::cout << s_t[1] << '\n';
                    } else {
                        // if(token.size() > 10) std::cout << token << std::endl;
                        next = token;
                    }
                }
            // List.printOrders();
            // cout << 1<< endl;

            vector<string> initial;
            vector<int> last_trade_price;
            vector<int> best_buy;
            vector<int> best_sell;

            for(int i = 0; i < List.getSize(); i++){
                // cout << 1 <<  endl;
                bool a = false;
                int index = 0;
                if(List.type[i] == 's'){
                    // cout << 1 <<  endl;
                    for(int j = 0; j < initial.size();j++){
                        if(initial[j] == List.stock_name[i]){
                            a = true;
                            index = j;
                        }
                    }

                        if(a == false){
                            cout << List.stock_name[i] << " " << List.price[i] << " " << "b" << endl;
                            initial.push_back(List.stock_name[i]);
                            last_trade_price.push_back(List.price[i]);
                            best_buy.push_back(INT32_MIN);
                            best_sell.push_back(INT32_MAX);
                        } else {
                            if(best_sell[index] > List.price[i]){
                                best_sell[index] = List.price[i];
                                if(best_buy[index] == List.price[i]){
                                    best_buy[index] = INT32_MIN;
                                    best_sell[index] = INT32_MAX;
                                    cout << "No Trade" << endl;
                                } else if (best_buy[index] != List.price[i]){
                                    if(last_trade_price[index] <= List.price[i]){
                                        best_sell[index] = List.price[i];
                                        cout << "No Trade" << endl;
                                    } else {
                                        cout << List.stock_name[i] << " " << List.price[i] << " " << "b" << endl;
                                        last_trade_price[index] = List.price[i];
                                    }
                                }
                            } else if(best_sell[index] <= List.price[i]){
                                cout << "No Trade" << endl;
                            }
                        }



                } else {
                    for(int j = 0; j < initial.size();j++){
                        if(initial[j] == List.stock_name[i]){
                            a = true;
                            index = j;
                        }
                    }

                        if(a == false){
                            cout << List.stock_name[i] << " " << List.price[i] << " " << "s" << endl;
                            initial.push_back(List.stock_name[i]);
                            last_trade_price.push_back(List.price[i]);
                            best_buy.push_back(INT32_MIN);
                            best_sell.push_back(INT32_MAX);
                        } else {
                            if(best_buy[index] < List.price[i]){
                                best_buy[index] = INT32_MIN;
                                if(best_sell[index] == List.price[i]){
                                    best_sell[index] = INT32_MAX;
                                    cout << "No Trade" << endl;
                                } else if (best_sell[index] != List.price[i]){
                                    if(last_trade_price[index] >= List.price[i]){
                                        best_buy[index] = List.price[i];
                                        cout << "No Trade" << endl;
                                    } else {
                                        cout << List.stock_name[i] << " " << List.price[i] << " " << "s" << endl;
                                        last_trade_price[index] = List.price[i];
                                    }
                                }
                            } else if(best_buy[index] >= List.price[i]){
                                cout << "No Trade" << endl;
                            }
                        }
                }
            }


        }

    } else if (arg1 == 2) {
        Receiver rcv;
        bool foundDollar = false;
        string next = "";
        while (!foundDollar) {
        std::string message = rcv.readIML();
        message = next + message;

        std::istringstream iss(message);
        std::string token;

        while (std::getline(iss, token, char(13))) {
            cout << "No Trade" << endl;
        }

        if(message[message.size()-1] == '$'){
            int i = message.size()-1;
            while(message[i] != char(13)){
                i--;
            }
        }


        // Check if the received data contains '$'
        if (message.find('$') != std::string::npos) {
            // rcv.terminate();
            foundDollar = true;
        }
    }

    cout << 0;

    } else if (arg1 == 3) {
        Receiver rcv;
        bool foundDollar = false;
        string next = "";
        while (!foundDollar) {
        std::string message = rcv.readIML();
        message = next + message;

        std::istringstream iss(message);
        std::string token;

        while (std::getline(iss, token, char(13))) {
            cout << "No Trade" << endl;
        }

        if(message[message.size()-1] == '$'){
            int i = message.size()-1;
            while(message[i] != char(13)){
                i--;
            }
        }


        // Check if the received data contains '$'
        if (message.find('$') != std::string::npos) {
            // rcv.terminate();
            foundDollar = true;
        }
    }

    cout << 0;

    } 

    return 0; // Return success code
}