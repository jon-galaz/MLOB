#include "Book.h"

#include <iostream>
#include <iomanip>

void printBook(const Book& book) 
{
    std::cout << "---------------------------\n";
    std::cout << std::setw(8) << "BID Id"
        << std::setw(8) << "Price"
        << std::setw(8) << "Vol"
        << "    |    "
        << std::setw(8) << "ASK Id"
        << std::setw(8) << "Price"
        << std::setw(8) << "Vol"
        << "\n";

    size_t rows = std::max(book.d_bids.size(), book.d_asks.size());

    for (size_t i = 0; i < rows; ++i) 
    {
        if (i < book.d_bids.size()) 
        {
            const auto& o = book.d_bids[i];
            std::cout << std::setw(8) << o.d_orderId
                << std::setw(8) << o.d_price
                << std::setw(8) << o.d_volume;
        }
        else
            std::cout << std::setw(24) << " ";

        std::cout << "    |    ";
        if (i < book.d_asks.size()) 
        {
            const auto& o = book.d_asks[i];
            std::cout << std::setw(8) << o.d_orderId
                << std::setw(8) << o.d_orderId
                << std::setw(8) << o.d_volume;
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------\n\n";
}

int main()
{
    Book mlob;
    
    printBook(mlob);
    
    mlob.addOrder(1, Side::Bid, 100, 5);
    mlob.addOrder(2, Side::Bid, 101, 3);
    mlob.addOrder(3, Side::Bid, 99, 7);
    std::cout << "After adding 3 bids:\n";
    printBook(mlob);

    mlob.addOrder(4, Side::Ask, 105, 2);
    mlob.addOrder(5, Side::Ask, 103, 4);
    mlob.addOrder(6, Side::Ask, 108, 1);
    std::cout << "After adding 3 asks:\n";
    printBook(mlob);

    std::cout << "-- Limit BUY#1000 6 @103 --\n";
    mlob.matchOrder(1000, Side::Bid, 103, 6);
    printBook(mlob);

    std::cout << "-- Limit SELL#2000 4 @99 --\n";
    mlob.matchOrder(2000, Side::Ask, 99, 4);
    printBook(mlob);

	return 0;
}