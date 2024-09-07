#include <vector>
using namespace std;

template <typename T, typename R>
class single_order{
public:
    T Broker_name;
    std::vector<std::pair<T,R>> stocks; 
    R price;
    T type;
    R timestamp;
    R expiration;
    R quantity;

    
    void bubbleSort(std::vector<std::pair<T,R>>& L) {
        size_t n = L.size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (L[j].first > L[j + 1].first) {
                    std::swap(L[j], L[j + 1]);
                }
            }
        }
    }

    // single_order& operator=(const single_order& other) {
    //     if (this != &other) {
    //         Broker_name = other.Broker_name;
    //         // stocks = other.stocks;
    //         price = other.price;
    //         type = other.type;
    //         timestamp = other.timestamp;
    //         expiration = other.expiration;
    //         quantity = other.quantity;
    //     }
    //     return *this;
    // }


    void sort_stocks(){
        
        bubbleSort(stocks);
        
    }

};

template <typename T, typename R>
class orderslist{
public:
    // std::vector<T> Broker_name_list;
    // std::vector<vector<std::pair<T,R>>> stocks_list; 
    // std::vector<R> price_list;
    // std::vector<T> type_list;
    // std::vector<R> timestamp_list;
    // std::vector<R> expiration_list;
     
    vector<single_order<T,R>> L;
    vector<R> index;

};


