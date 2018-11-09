#include "cardgame.hpp"

void cardgame::login(eosio::name user)
{
	_users.emplace(get_self(),[&](auto&  u){
			u.name = user;
	});
}

void cardgame::startgame(eosio::name user) {
	auto& itr = _users.get(user.value,"User not exist");
	_users.modify(itr,get_self(),[&](auto& _to_modify_user){
		game game_data;
		for(uint8_t i = 0; i < 4; i++){
			draw_card(game_data.deck_player,game_data.hand_player);
			draw_card(game_data.deck_ai,game_data.hand_ai);
		}
		_to_modify_user.game_data = game_data;
	});
}

void cardgame::playcard(eosio::name user, uint8_t player_card_index){
	eosio::require_auth(user);
	eosio_assert(player_card_index < 4,"invalid hand index");// 手上牌最多四张
	// 通过user找到数据表中数据
	auto& player = _users.get(user.value,"User not exist");
	eosio_assert(player.game_data.status == ONGOING,"game have ended");
	eosio_assert(player.game_data.selected_card_player == 0,"the player have selected car in this turn");
	// 修改数据表
	_users.modify(player,get_self(),[&](auto& _to_modify_user){
		game& game_data = _to_modify_user.game_data;
		// 设定选中的卡片id
		game_data.selected_card_player = game_data.hand_player[player_card_index];
		// 将手中卡片对应位置置位empty
		game_data.hand_player[player_card_index] = 0;
	});
};
int cardgame::random(const int range) {
	auto seed_itr = _seed.begin();
	if (seed_itr  == _seed.end()){	// 如果没有随机数据，就用默认值初始化
		seed_itr = _seed.emplace(get_self(),[&](auto& s){
			s = seed{};
		});
	};
	int prime = 65535;
	int new_seed_value = (seed_itr->seed_value + now()) % prime;
	_seed.modify(seed_itr,get_self(),[&](auto& s){
			s.seed_value = new_seed_value;
	});
	// 随机范围  0 ~ range
	int random_res = new_seed_value % range;
	return random_res;
};
void cardgame::draw_card(vector<uint8_t>& deck, vector<uint8_t>& hand) {
	  int deck_card_idx = random(deck.size());
	  int first_empty_slot = -1;
	  for (int i = 0; i <= hand.size(); i++) {
	      auto id = hand[i];
	      if (card_dict.at(id).type == EMPTY) {
	          first_empty_slot = i;
	          break;
	      }
	    }
	  eosio_assert(first_empty_slot != -1, "No empty slot in the player's hand");
	  hand[first_empty_slot] = deck[deck_card_idx];
	  deck.erase(deck.begin() + deck_card_idx);
 };
EOSIO_DISPATCH(cardgame,(login)(startgame)(playcard))