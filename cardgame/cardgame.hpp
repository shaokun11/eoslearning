#include <eosiolib/eosio.hpp>
using namespace std;

class [[eosio::contract]]cardgame : public eosio::contract {
  public:
    cardgame(eosio::name reciever,eosio::name code,eosio::datastream<const char*> ds )
                                :contract(reciever,code,ds),
                                _users(reciever,code.value),
                                _seed(reciever,code.value){};
                                
    [[eosio::action]] void login(eosio::name user);
    [[eosio::action]] void startgame(eosio::name user);
    [[eosio::action]] void endgame(eosio::name user);
    [[eosio::action]] void playcard(eosio::name user, uint8_t index);
    [[eosio::action]] void nextround(eosio::name user);

  private:
    enum game_status:int8_t
    {
      ONGOING = 0,      // 游戏正在进行中
      PLAYER_WON = 1,   // 游戏已经结束，玩家获得胜利
      PLAYER_LOST = -1 // 游戏结束， 完结失败
    };
    enum card_type : uint8_t  // 卡片的类型 总共五种属性类型,总共17张卡牌
    {
      EMPTY = 0,    // 不存在的卡片
      FIRE = 1,     // 火属性， 克木  攻击力为 1 和 2 的各两张  攻击力为3的一张    总共5张
      WOOD = 2,     // 木属性， 克水  攻击力为 1 和 2 的各两张  攻击力为3的一张    总共5张
      WATER = 3,    // 水属性   克火  攻击力为 1 和 2 的各两张  攻击力为3的一张    总共5张
      NEUTRAL = 4,  // 中立属性       攻击力为3 总共1张 
      VOID = 5      // 平局属性       共计力为0 总共1张
    };

    struct card
    {
      uint8_t type;   // 卡片类型
      uint8_t attack_point; // 卡片的攻击力
    };

    const  map<uint8_t,card> card_dict = {
        {0 , {EMPTY , 0}},
        {1 , {FIRE , 1}},
        {2 , {FIRE , 1}},
        {3 , {FIRE , 2}},
        {4 , {FIRE , 2}},
        {5 , {FIRE , 3}},
        {6 , {WOOD , 1}},
        {7 , {WOOD , 1}},
        {8 , {WOOD , 2}},
        {9 , {WOOD , 2}},
        {10 , {WOOD , 3}},
        {11 , {WATER , 1}},
        {12 , {WATER , 1}},
        {13 , {WATER , 2}},
        {14 , {WATER , 2}},
        {15 , {WATER , 3}},
        {16 , {NEUTRAL , 3}},
        {17 , {VOID , 0}},
    };
    struct game
    {
     int8_t status = ONGOING;   // 只要登录 默认游戏正在进行
     int8_t life_player = 5;    // 游戏玩家 5条生命
     int8_t life_ai = 5;      // 游戏玩家 5条生命
     vector<uint8_t>  deck_player = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}; // 玩家待选的卡牌id
     vector<uint8_t> deck_ai = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};     // ai待选的卡牌id
     vector<uint8_t> hand_player = {0,0,0,0}; // 玩家的手里牌
     vector<uint8_t> hand_ai = {0,0,0,0};     // ai的手里牌
     uint8_t selected_card_player = 0;          // 从待选牌中选中的牌
     uint8_t selected_card_ai = 0;              // 从待选牌中选中的牌
     uint8_t         life_lost_player = 0;
     uint8_t         life_lost_ai = 0;
    };
    

  	struct [[eosio::table]] userinfo
  	{
  		eosio::name  name;			
  		uint16_t win_count = 0;		
  		uint16_t lost_count = 0;
      game game_data;         

  		auto primary_key() const {return name.value;}  
  	};
    
    struct [[eosio::table]] seed 
    {
      uint64_t key = 1;
      uint32_t seed_value = 1;

      auto primary_key() const {return key;};
    };

  	typedef eosio::multi_index<"userinfo"_n, userinfo> user_index;
    typedef eosio::multi_index<"seed"_n, seed> seed_index;
    user_index _users; 
    seed_index _seed;

  int random(const int range);
  void draw_card(vector<uint8_t>& deck, vector<uint8_t>& hand);

  //AI最可能赢策略。
  int ai_best_card_win_strategy(const int ai_attack_point, const int player_attack_point);
	// AI最不可能输策略。
  int ai_min_loss_strategy(const int ai_attack_point, const int player_attack_point);
	//AI积分出牌策略。
  int ai_points_tally_strategy(const int ai_attack_point, const int player_attack_point);
  //阻止AI输策略。
  int ai_loss_prevention_strategy(const int8_t life_ai, const int ai_attack_point, const int player_attack_point);

  int ai_choose_card(const game& game_data);
  int calculate_ai_card_score(const int strategy_idx, const int8_t life_ai, 
                              const card& ai_card, const vector<uint8_t> hand_player);
  int calculate_attack_point(const card& card1, const card& card2);
 
  void resolve_selected_cards(game& game_data);

  void update_game_status(userinfo& user);

};

