#include "cardgame.hpp"

void cardgame::login(eosio::name user)
{
	eosio::require_auth(user);
	auto itr = _users.find(user.value);
	if(itr == _users.end()){
		_users.emplace(get_self(),[&](auto&  row){
				row.name = user;
			});
	} 
	// 否则登录成功
}

void cardgame::startgame(eosio::name user) {
	eosio::require_auth(user);
	auto& itr = _users.get(user.value,"User not exist");
	_users.modify(itr, get_self(), [&](auto& row){
		game game_data;
		for(uint8_t i = 0; i < 4; i++){
			draw_card(game_data.deck_player, game_data.hand_player);
			draw_card(game_data.deck_ai, game_data.hand_ai);
		}
		row.game_data = game_data;
	});
}

void cardgame::endgame(eosio::name user) {
	eosio::require_auth(user);
	auto& itr = _users.get(user.value,"User not exist");
	_users.modify(itr, get_self(), [&](auto& row){
		row.game_data = game();
	});
}

void cardgame::nextround(eosio::name user){
	require_auth(user);
	auto& itr = _users.get(user.value, "User doesn't exist");
	eosio_assert(itr.game_data.status == ONGOING,
			"nextround: This game has ended. Please start a new one.");
	eosio_assert(itr.game_data.selected_card_player != 0 && itr.game_data.selected_card_ai != 0,
			"nextround: Please play a card first.");

	_users.modify(itr, get_self(), [&](auto& modified_user) {
		game& game_data = modified_user.game_data;

		game_data.selected_card_player = 0;
		game_data.selected_card_ai = 0;
		game_data.life_lost_player = 0;
		game_data.life_lost_ai = 0;
		if (game_data.deck_player.size() > 0) draw_card(game_data.deck_player, game_data.hand_player);
		if (game_data.deck_ai.size() > 0) draw_card(game_data.deck_ai, game_data.hand_ai);
	});
};

void cardgame::playcard(eosio::name user, uint8_t index) {
  // Ensure this action is authorized by the player
  require_auth(user);

  // Checks that selected card is valid
  eosio_assert(index < 4, "playcard: Invalid hand index");

  auto& itr = _users.get(user.value, "User doesn't exist");

  // Verify game status is suitable for the player to play a card
  eosio_assert(itr.game_data.status == ONGOING,
               "playcard: This game has ended. Please start a new one");
  eosio_assert(itr.game_data.selected_card_player == 0,
               "playcard: The player has played his card this turn!");

  _users.modify(itr, user, [&](auto& modified_user) {
    game& game_data = modified_user.game_data;

    // Assign the selected card from the player's hand
    game_data.selected_card_player = game_data.hand_player[index];
    game_data.hand_player[index] = 0;

    // AI picks a card
    int ai_card_idx = ai_choose_card(game_data);
    game_data.selected_card_ai = game_data.hand_ai[ai_card_idx];
    game_data.hand_ai[ai_card_idx] = 0;

    resolve_selected_cards(game_data);

    update_game_status(modified_user);
  });
}

void cardgame::draw_card(vector<uint8_t>& deck, vector<uint8_t>& hand) {
	int deck_card_idx = random(deck.size());
	int empty_slot = -1;
	for (int i = 0; i < hand.size(); i++) {
		auto id = hand[i];
		if (card_dict.at(id).type == EMPTY) {
			empty_slot = i;
			break;
		}
	}
	eosio_assert(empty_slot != -1, "No empty slot in the player's hand");
	hand[empty_slot] = deck[deck_card_idx];
	deck.erase(deck.begin() + deck_card_idx);
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


int cardgame::ai_best_card_win_strategy(const int ai_attack_point, const int player_attack_point){
	if (ai_attack_point > player_attack_point) return 3;
	if (ai_attack_point < player_attack_point) return -2;
	return -1;
};
int cardgame::ai_min_loss_strategy(const int ai_attack_point, const int player_attack_point){
	if (ai_attack_point > player_attack_point) return 1;
	if (ai_attack_point < player_attack_point) return -4;
	return -1;
};
int cardgame::ai_points_tally_strategy(const int ai_attack_point, const int player_attack_point){
 	return ai_attack_point - player_attack_point;	
};
int cardgame::ai_loss_prevention_strategy(const int8_t life_ai, const int ai_attack_point, const int player_attack_point){
	if (life_ai + ai_attack_point - player_attack_point > 0) return 1;
	return 0;
};

int cardgame::ai_choose_card(const game& game_data) {
  int available_strategies = 4;
  if (game_data.life_ai > 2) available_strategies--;
  int strategy_idx = random(available_strategies);
  int chosen_card_idx = -1;
  int chosen_card_score = std::numeric_limits<int>::min();

  for (int i = 0; i < game_data.hand_ai.size(); i++) {
    const auto ai_card_id = game_data.hand_ai[i];
    const auto ai_card = card_dict.at(ai_card_id);
    if (ai_card.type == EMPTY) continue;
    auto card_score = calculate_ai_card_score(strategy_idx, game_data.life_ai, ai_card, game_data.hand_player);
    if (card_score > chosen_card_score) {
      chosen_card_score = card_score;
      chosen_card_idx = i;
    }
  }
  return chosen_card_idx;
}

int cardgame::calculate_ai_card_score(const int strategy_idx,
                                      const int8_t life_ai,
                                      const card& ai_card,
                                      const vector<uint8_t> hand_player) {
   int card_score = 0;
   for (int i = 0; i < hand_player.size(); i++) {
      const auto player_card_id = hand_player[i];
      const auto player_card = card_dict.at(player_card_id);

      int ai_attack_point = calculate_attack_point(ai_card, player_card);
      int player_attack_point = calculate_attack_point(player_card, ai_card);

      // Accumulate the card score based on the given strategy
      switch (strategy_idx) {
        case 0: {
          card_score += ai_best_card_win_strategy(ai_attack_point, player_attack_point);
          break;
        }
        case 1: {
          card_score += ai_min_loss_strategy(ai_attack_point, player_attack_point);
          break;
        }
        case 2: {
          card_score += ai_points_tally_strategy(ai_attack_point, player_attack_point);
          break;
        }
        default: {
          card_score += ai_loss_prevention_strategy(life_ai, ai_attack_point, player_attack_point);
          break;
        }
      }
    }
    return card_score;
}

int cardgame::calculate_attack_point(const card& card1, const card& card2) {
	int result = card1.attack_point;
	if ((card1.type == FIRE && card2.type == WOOD) ||
		(card1.type == WOOD && card2.type == WATER) ||
		(card1.type == WATER && card2.type == FIRE)) {
		result++;
	}
  return result;
};


void cardgame::resolve_selected_cards(game& game_data) {
  const auto player_card = card_dict.at(game_data.selected_card_player);
  const auto ai_card = card_dict.at(game_data.selected_card_ai);

  if (player_card.type == VOID || ai_card.type == VOID) return;

  int player_attack_point = calculate_attack_point(player_card, ai_card);
  int ai_attack_point =  calculate_attack_point(ai_card, player_card);
  if (player_attack_point > ai_attack_point) {
    int diff = player_attack_point - ai_attack_point;
    game_data.life_lost_ai = diff;
    game_data.life_ai -= diff;
  } else if (ai_attack_point > player_attack_point) {
    int diff = ai_attack_point - player_attack_point;
    game_data.life_lost_player = diff;
    game_data.life_player -= diff;
  }
}

void cardgame::update_game_status(userinfo& user) {
  game& game_data = user.game_data;
  if (game_data.life_ai <= 0) {
    game_data.status = PLAYER_WON;
  } else if (game_data.life_player <= 0) {

    game_data.status = PLAYER_LOST;
  } else {
    const auto is_empty_slot = [&](const auto& id) { return card_dict.at(id).type == EMPTY; };
    bool player_finished = std::all_of(game_data.hand_player.begin(), game_data.hand_player.end(), is_empty_slot);
    bool ai_finished = std::all_of(game_data.hand_ai.begin(), game_data.hand_ai.end(), is_empty_slot);
    if (player_finished || ai_finished) {
      if (game_data.life_player > game_data.life_ai) {
        game_data.status = PLAYER_WON;
      } else {
        game_data.status = PLAYER_LOST;
      }
    }
  }

  if (game_data.status == PLAYER_WON) {
    user.win_count++;
  } else if (game_data.status == PLAYER_LOST) {
    user.lost_count++;
  }
}

EOSIO_DISPATCH(cardgame,(login)(startgame)(playcard)(endgame)(nextround))

