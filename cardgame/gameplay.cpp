#include "cardgame.hpp"

int cardgame::random(const int range) {
	auto seed_itr = _seed.begin();
	if (seed_itr  == _seed.end())
	{	// 如果没有随机数据，就用默认值初始化
		seed_itr = _seed.emplace(_self,[&](auto& seed){})
	};
	int prime = 65535;
	// 新值得范围 0 ~ 65535
	auto new_seed_value = (seed_itr.value + now()) % prime;
	_seed.modify(seed_itr,_self,[&](auto& s){
			s.value = new_seed_value;
	});
	// 随机范围  0 ~ range
	int random_res = new_seed_value % range;
	return random_res;
}
// 根据待选卡片和手上已有的卡片，随机进行选择新的卡片
void cardgame::draw_one_card(vector<uint8_t> &deck,vector<uint8_t> &hand){
	// 0 ~ 16
 	int deck_card_index = random(deck.size());
 	int first_emprty_slot = -1;
 	for (int i = 0; i < hand.size; ++i)
 	{	
 		auto id = hand[i];
 		if (card_dict.at(id).type == EMPTY){
 			first_emprty_slot = i;
 			break;
 		}
 	}
 	eosio_assert(first_emprty_slot !=- 1, "No empty slot in the players hand");
 	// 如果手上的牌 有空位，进行随机赋值
 	hand[first_emprty_slot] = deck[deck_card_index];
 	// 待选牌中移除掉已经被赋值给手上的牌
 	deck.erase(deck.began() + deck_card_index);
 }






