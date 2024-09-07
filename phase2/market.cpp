#include "market.h"
#include "orders.cpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <atomic>
#include <mutex>
#include <sstream>

using namespace std;



//////

struct tradername{
    std::string Broker_name;
    int bought; 
    int sold;
    int net;
};
std::vector<tradername> tlist;

///////




market::market(int argc, char** argv)
{
    inputfile.open("output.txt");
}

void market::start()
{   
    orderslist<string,int> active_list;

    int money = 0;
    int completedtrades =0;
    int sharedtrades = 0 ;
    
	while(std::getline(inputfile,line)){
        if(line[0] != '!'){
            // cout << line << endl;
            if(line[0] == 'T' && line[1] == 'L'){
                continue;
            }

            std::istringstream iss(line);
            string time_t;
            int time;

            if(std::getline(iss,time_t,' ')){
                time = stoi(time_t);
            }
            string name;
            if(std::getline(iss,name,' ')){}
            string bs;
            if(std::getline(iss,bs,' ')){}
            string stock_temp;
            vector<pair<string,int>> lc;
            
            if(std::getline(iss,stock_temp,'$')){
                std::istringstream iss_2(stock_temp);
                while(true){
                    string s_name;
                    if(std::getline(iss_2,s_name,' ')){}
                    else {
                        break;
                    }
                    int u=s_name.size()+2;
                    // cout << u << endl;
                    // cout << stock_temp.size() << endl;
                    if(u > stock_temp.size()){
                        std::pair<string, int> myPair(s_name, 1);
                        lc.push_back(myPair);
                        break;
                    }
                    // cout << "yes" << endl;
                    string quan_t;
                    // cout << "yes" << endl;
                    int quan;
                    if(std::getline(iss_2,quan_t,' ')){
                        // cout << "yes" << endl;
                        quan = stoi(quan_t);
                    } else {
                        break;
                    }
                    std::pair<string, int> myPair(s_name, quan);
                    lc.push_back(myPair);
                }
            }
            string price_t;
            int p_t;
            if(std::getline(iss,price_t,' ')){
                p_t = stoi(price_t);
            }
            string dur_t;
            if(std::getline(iss,dur_t,'#')){}
            string quantity_t;
            int quant;
            if(std::getline(iss,quantity_t,' ')){
                quant = stoi(quantity_t);
            }
            int dur;
            int exp;
            if(std::getline(iss,dur_t,'\n')){
                dur = stoi(dur_t);
                if(dur == -1){
                    exp = INT32_MAX;
                }
                else{exp = time + dur;}
            }

            single_order<string,int> sub_S;
            sub_S.Broker_name = name;
            sub_S.timestamp = time;
            sub_S.type = bs;
            sub_S.expiration = exp;
            sub_S.price = p_t;
            sub_S.stocks = lc;
            sub_S.sort_stocks();
            sub_S.quantity = quant;


            
            ///////////////
            int index =0 ;
            for(int i=0; i<tlist.size() ;i++){
                if(tlist[i].Broker_name == sub_S.Broker_name ){
                    index = 1;
                }
            }
            if(index == 0){
                tradername x;
                x.bought=0;
                x.sold=0;
                x.net=0;
                x.Broker_name = sub_S.Broker_name;
                tlist.push_back(x);
            }
            ////////////





            int i = active_list.L.size();
            orderslist<string,int> max_profit;
            if(i == 0) {
                // cout << "yes" << endl;

                active_list.L.push_back(sub_S);
                continue;

            }
            // std::cout << time << " " << name << " " << bs << " " << stock_temp << " " << p_t << " " << quant << " " << exp << endl;

            // cout << "yes" << endl;
            for(int j=0;j<active_list.L.size();j++){
                            // cout << "yes" << endl;

                // if(time > active_list.L[j].expiration  || active_list.L[j].quantity == 0){
                    if(time > active_list.L[j].expiration){
                    active_list.L.erase(active_list.L.begin() + j);
                    j--;
                } 
            }

            for(int j=0;j<active_list.L.size();j++){

                // if(time > active_list.L[j].expiration  || active_list.L[j].quantity == 0){
                //     active_list.L.erase(active_list.L.begin() + j);
                //     j--;
                // } 
                if (active_list.L[j].type != bs){
                    bool flag = true;
                    if(lc.size() != active_list.L[j].stocks.size()){
                        flag = false;
                    }
                    else {
                        for(int k = 0; k < lc.size(); k++){
                            if(active_list.L[j].stocks[k].first != lc[k].first || active_list.L[j].stocks[k].second != lc[k].second) flag = false;
                        }
                    }
                    if(flag == true){
                        if(bs == "SELL"){
                            if(active_list.L[j].price >= p_t){
                                // if(stock_temp == "AMZN ") cout << 1 << endl;
                                max_profit.L.push_back(active_list.L[j]);
                                max_profit.index.push_back(j);
                            }
                        }
                        else{
                            if(active_list.L[j].price <= p_t){
                                max_profit.L.push_back(active_list.L[j]);
                                max_profit.index.push_back(j);
                            }
                        }
                    }
                }
            }
            

            // if(stock_temp == "AMD "){
                // for(int i = 0; i < max_profit.L.size();i++){
                //      std::cout << max_profit.L[i].timestamp << " " << max_profit.L[i].Broker_name << " " << max_profit.L[i].type << endl;
                // }
            // }
            int k1 = max_profit.L.size();

            for (int l1 = 0; l1 < k1 - 1; ++l1) {
        for (int l2 = 0; l2 < k1 - 1 - l1; ++l2) {
            // Change the condition if you want to sort in ascending order
            int k2=max_profit.L[l2].price;
            int k3 =max_profit.L[l2 + 1].price;
            if (k2 > k3) {
                // std::cout << "Swapping elements at indices " << l2 << " and " << l2 + 1 << std::endl;

                // Swap elements in max_profit.L
                std::swap(max_profit.L[l2], max_profit.L[l2 + 1]);

                // Swap elements in max_profit.index
                std::swap(max_profit.index[l2], max_profit.index[l2 + 1]);
            }
        }
    }

            

            if(max_profit.L.empty()){
                active_list.L.push_back(sub_S);
            }

            else if(bs == "BUY"){
                // max_profit.heapSort();
                int i = 0;
                while(!max_profit.L.empty() && i < max_profit.L.size() && sub_S.quantity > 0)
                {
                    if(max_profit.L[i].quantity == sub_S.quantity) {
                        std::cout << sub_S.Broker_name << " purchased " << to_string(sub_S.quantity) << " share of " << stock_temp << "from " << active_list.L[max_profit.index[i]].Broker_name << " for $" << to_string(active_list.L[max_profit.index[i]].price) <<"/share"<<endl;
                        
                    

                        ////////////////////
                        money +=  (active_list.L[max_profit.index[i]].price)*(sub_S.quantity) ;
                        completedtrades++;
                        sharedtrades+=sub_S.quantity;
                        for(int e=0; e<tlist.size();e++){
                            if(tlist[e].Broker_name == sub_S.Broker_name){
                                tlist[e].bought += sub_S.quantity;
                                tlist[e].net -= (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                            if(tlist[e].Broker_name == active_list.L[max_profit.index[i]].Broker_name){
                                tlist[e].sold += sub_S.quantity;
                                tlist[e].net += (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                        }
                        /////////////////////


                        active_list.L[max_profit.index[i]].quantity = 0;
                        active_list.L.erase(active_list.L.begin() + max_profit.index[i]);
                        sub_S.quantity = 0;
                        break;
                    }
                    else if(max_profit.L[i].quantity > sub_S.quantity){
                        std::cout << sub_S.Broker_name << " purchased " << to_string(sub_S.quantity) << " share of " << stock_temp << "from " << active_list.L[max_profit.index[i]].Broker_name << " for $" << to_string(active_list.L[max_profit.index[i]].price) <<"/share"<<endl;
                        
                        
                        
                        ////////////////////
                        money +=  (active_list.L[max_profit.index[i]].price)*(sub_S.quantity) ;
                        completedtrades++;
                        sharedtrades+=sub_S.quantity;
                        for(int e=0; e<tlist.size();e++){
                            if(tlist[e].Broker_name == sub_S.Broker_name){
                                tlist[e].bought += sub_S.quantity;
                                tlist[e].net -= (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                            if(tlist[e].Broker_name == active_list.L[max_profit.index[i]].Broker_name){
                                tlist[e].sold += sub_S.quantity;
                                tlist[e].net += (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                        }
                        /////////////////////



                        active_list.L[max_profit.index[i]].quantity = active_list.L[max_profit.index[i]].quantity - sub_S.quantity;
                        sub_S.quantity = 0;
                        // purchased 12 share of AMZN from KarGoCorp for $100/share
                        break;
                    } else if(max_profit.L[i].quantity > 0){
                        std::cout << sub_S.Broker_name << " purchased " << to_string(active_list.L[max_profit.index[i]].quantity) << " share of " << stock_temp << "from " << active_list.L[max_profit.index[i]].Broker_name << " for $" << to_string(active_list.L[max_profit.index[i]].price) <<"/share"<<endl;
                        
                        

                        ////////////////////
                        money +=  (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity) ;
                        completedtrades++;
                        sharedtrades+=active_list.L[max_profit.index[i]].quantity;
                        for(int e=0; e<tlist.size();e++){
                            if(tlist[e].Broker_name == sub_S.Broker_name){
                                tlist[e].bought += active_list.L[max_profit.index[i]].quantity;
                                tlist[e].net -= (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity);
                            }
                            if(tlist[e].Broker_name == active_list.L[max_profit.index[i]].Broker_name){
                                tlist[e].sold += active_list.L[max_profit.index[i]].quantity;
                                tlist[e].net += (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity);
                            }
                        }
                        /////////////////////
                        


                        // active_list.L[max_profit.index[i]].quantity -= sub_S.quantity;
                        sub_S.quantity -= active_list.L[max_profit.index[i]].quantity;
                        active_list.L.erase(active_list.L.begin() + max_profit.index[i]);
                    }
                    i++;
                }
                if(sub_S.quantity > 0){
                    active_list.L.push_back(sub_S);
                }
            }
            else{
                int i = max_profit.L.size()-1;
                while(!max_profit.L.empty()  && i >= 0 && sub_S.quantity > 0){
                    if(max_profit.L[i].quantity >= sub_S.quantity){
                        // purchased 12 share of AMZN from KarGoCorp for $100/share
                        std::cout << active_list.L[max_profit.index[i]].Broker_name << " purchased " << to_string(sub_S.quantity) << " share of " << stock_temp << "from " << sub_S.Broker_name << " for $" << to_string(active_list.L[max_profit.index[i]].price) <<"/share" << endl;
                        
                        
                        ////////////////////
                        money +=  (active_list.L[max_profit.index[i]].price)*(sub_S.quantity) ;
                        completedtrades++;
                        sharedtrades+=sub_S.quantity;
                        for(int e=0; e<tlist.size();e++){
                            if(tlist[e].Broker_name == sub_S.Broker_name){
                                tlist[e].sold += sub_S.quantity;
                                tlist[e].net += (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                            if(tlist[e].Broker_name == active_list.L[max_profit.index[i]].Broker_name){
                                tlist[e].bought += sub_S.quantity;
                                tlist[e].net -= (active_list.L[max_profit.index[i]].price)*(sub_S.quantity);
                            }
                        }
                        /////////////////////

                        
                        
                        active_list.L[max_profit.index[i]].quantity -= sub_S.quantity;
                        sub_S.quantity = 0;
                        break;
                    } else if(max_profit.L[i].quantity > 0){
                        std::cout << active_list.L[max_profit.index[i]].Broker_name << " purchased " << to_string(active_list.L[max_profit.index[i]].quantity) << " share of " << stock_temp << "from " << sub_S.Broker_name << " for $" << to_string(active_list.L[max_profit.index[i]].price) <<"/share" << endl;
                        
                        
                        ////////////////////
                        money +=  (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity) ;
                        completedtrades++;
                        sharedtrades+=active_list.L[max_profit.index[i]].quantity;
                        for(int e=0; e<tlist.size();e++){
                            if(tlist[e].Broker_name == sub_S.Broker_name){
                                tlist[e].sold += active_list.L[max_profit.index[i]].quantity;
                                tlist[e].net += (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity);
                            }
                            if(tlist[e].Broker_name == active_list.L[max_profit.index[i]].Broker_name){
                                tlist[e].bought += active_list.L[max_profit.index[i]].quantity;
                                tlist[e].net -= (active_list.L[max_profit.index[i]].price)*(active_list.L[max_profit.index[i]].quantity);
                            }
                        }
                        /////////////////////


                        // active_list.L[max_profit.index[i]].quantity -= sub_S.quantity;
                        sub_S.quantity -= active_list.L[max_profit.index[i]].quantity;
                        active_list.L.erase(active_list.L.begin() + max_profit.index[i]);
                    }
                    i--;
                }
                if(sub_S.quantity > 0){
                    active_list.L.push_back(sub_S);
                }
            }
        } else {
            break;
        }
    }



    ///////////////
    std::cout << std::endl; 
    std::cout << "---End of Day---" << std::endl;
    std::cout << "Total Amount of Money Transferred: $" << money << std::endl;
    std::cout << "Number of Completed Trades: " << completedtrades << std::endl;
    std::cout << "Number of Shares Traded: " << sharedtrades << std::endl;

    for(int i=0;i<tlist.size();i++){
        std::cout << tlist[i].Broker_name << " bought "<<tlist[i].bought << " and sold "<<tlist[i].sold<< " for a net transfer of $" << tlist[i].net << std::endl ;
    }
    ///////////////












}
