#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

using OrderId = uint64_t;
using Volume = uint64_t;
using Price = uint64_t;

enum class Side{ Bid, Ask};

struct Order
{
	OrderId d_orderId;
	Volume d_volume;
	Price d_price;
};

struct Book
{
	std::vector<Order> d_bids;
	std::vector<Order> d_asks;
	std::unordered_map<OrderId, std::pair<Side, size_t>> d_index;

	void addOrder(const OrderId orderId, const Side side, const Price price, const Volume volume );
	void modifyOrder(OrderId orderId, Volume volume);
	void deleteOrder(OrderId orderId);
	void matchOrder(OrderId orderId, Side side, Price price, Volume volume);
};