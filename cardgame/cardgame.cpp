#include "cardgame.hpp"

void cardgame::login(account_name user)
{
	require_auth(user); 
	//确保这个用户已经被授权
	// 这里我理解了很久，把我的想法记录在这里
	// 这里是调用此合约的account name，即提供私钥的account，这里传入的这个account必须拥有它的active权限，
	// 当然，这里还可以要求有其他的权限，这样就可以给特殊的账号特殊的权限
	auto user_iterator = _users.find(user);  // 查找
	if (user_iterator == _users.end()) {
		user_iterator = _users.emplace(user, [&](auto& new_user){
			new_user.name = user;
		});
	}
	// 这里使用multi_index的find 和 emplace(插入)两种方法，
	// 这些方法的形式都是一样的，记住就好，可以去官网查看详细的方法说明
}

void cardgame::startgame(account_name name) {
	require_auth(name);
	auto& info = _users.get(name, "user not exist");
	_users.modify(info,name,[&](auto& _to_modify_user){
		game game_data;
		for(uint8_t i = 0; i < 4; i++){
			draw_one_card(game_data.deck_player,game_data.hand_player);
			draw_one_card(game_data.deck_ai,game_data.hand_ai);
		}
		_to_modify_user.game_data = game_data;
	});
}

void cardgame::playcard(account_name user,uint8_t player_card_index){
	require_auth(user);
	eosio.assert(player_card_index < 4,"invalid hand index"); // 手上牌最多四张
	// 通过user找到数据表中数据
	auto& player = _users.get(user,"User not exist");
	eosio_assert(player.game_data.status == ONGONING,"game have ended"); // 确保本轮游戏没有结束
	eosio_assert(player.game_data.selected_card_player == 0,"the player have selected car in this turn"); // 确保手上没有选牌
	// 修改数据表
	_users.modify(player,user,[&](auto& _to_modify_user){
		game& game_data = _to_modify_user.game_data;
		// 设定选中的卡片id
		game_data.selected_card_player = game_data.hand_player[player_card_index];
		// 将手中卡片对应位置置位empty
		game_data.hand_player[player_card_index] = 0;
	});
}
EOSIO_ABI(cardgame,(login)(playcard))








