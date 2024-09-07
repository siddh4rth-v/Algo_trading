#include <vector>

template <typename T, typename R, typename S>
class orderslist{
public:
    std::vector<T> stock_name;
    std::vector<R> price;
    std::vector<S> type;
    int size;

    orderslist(){size = 0;}

    void add_order(T& Name, R& Price, S& Type){
        stock_name.push_back(Name);
        price.push_back(Price);
        type.push_back(Type);
        size++;
    }

    int getSize(){
        return size;
    }

    void printOrders(){
        for(int i = 0; i < size; i++){
            // std::cout << stock_name[i] << " : " << price[i] << " : " << type[i] << '\n';
            std::cout << stock_name[i] << " " << price[i] << " " << type[i] << "#" <<'\n';
        }
    }

};
