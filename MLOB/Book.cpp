#include "Book.h"

#include <algorithm>

void Book::addOrder(const OrderId orderId, const Side side, const Price price, const Volume volume)
{
	// get the data from the corresponding side
	auto& data = side == Side::Ask ? d_asks : d_bids;

	// find the inserting position by price
	auto it = std::find_if(data.begin(), data.end(), [&](const auto& order)
		{
			return side == Side::Ask ? price < order.d_price : price > order.d_price;
		});

	size_t index = std::distance(data.begin(), it);

	// insert it at that position
	data.insert(it, {orderId, price, volume});

	// update indices
	for (size_t i = index; i < data.size(); ++i)
		d_index[data[i].d_orderId] = {side, i};
}

void Book::modifyOrder(OrderId orderId, Volume newVolume)
{
	auto it = d_index.find(orderId);

	if (it == d_index.end()) 
		return;

	auto [side, index] = it->second;

	auto& data = side == Side::Bid ? d_bids : d_asks;

	if (newVolume == 0)
		deleteOrder(orderId);
	else 
		data[index].d_volume = newVolume;

}

void Book::deleteOrder(OrderId orderId)
{
	auto it = d_index.find(orderId);

	if (it == d_index.end()) 
		return;

	auto [side, index] = it->second;

	auto& data = (side == Side::Bid ? d_bids : d_asks);

	// erase data
	data.erase(data.begin() + index);

	// erase index
	d_index.erase(it);

	// fix index
	for (size_t i = index; i < data.size(); ++i)
		d_index[data[i].d_orderId] = { side, i };
}

void Book::matchOrder(OrderId orderId, Side side, Price price, Volume volume)
{
	auto& bookSideData = side == Side::Ask ? d_asks : d_bids;
	auto clientSide = side == Side::Ask ? Side::Bid : Side::Ask;
	auto& clientSideData = clientSide == Side::Ask ? d_asks : d_bids;

	auto canMatchOrder = [&](Price bookPrice) -> bool
	{
		return side == Side::Ask ? price <= bookPrice : price >= bookPrice;
	};

	Volume remainingVolume = volume;

	// while there is volume to trade and orders to handle
	while (remainingVolume > 0 && !clientSideData.empty() && canMatchOrder(clientSideData.front().d_price))
	{
		Order topOrder = clientSideData.front();
		Volume tradedVolume = std::min(remainingVolume, topOrder.d_volume);

		// compute the remaining volume
		remainingVolume -= tradedVolume;

		// handle corresponding editing of orders
		if (tradedVolume == topOrder.d_volume)
			deleteOrder(topOrder.d_orderId);
		else
			modifyOrder(topOrder.d_orderId, topOrder.d_volume - tradedVolume);

		// create leftover orders
		if (remainingVolume > 0)
			addOrder(orderId, side, price, remainingVolume);
	}
}
