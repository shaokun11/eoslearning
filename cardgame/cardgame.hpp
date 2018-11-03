#include <eosiolib/eosio.hpp>
using namespace std;
class cardgame : public eosio::contract {
// 继承contract
  public:
    //构造方法,并实例化了multi_index表users
    cardgame( account_name self):contract(self),
                                _users(self,self),
                                _seed(self,self){};

    // table和action 必须加上@abi 的注解，这样才能生存对应的abi，
    // 在eosio.cdt中，使用 [[eosio::action]] 更加简洁 
    [[eosio::action]]
    void login(account_name user);

    // 开始游戏，分发手里牌
    [[eosio::action]]
    void startgame(account_name user);

    //从手上牌选择卡片进行对战
    [[eosio::action]]
    void playcard(account_name user,uint8_t player_card_index);

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
    typedef uint8_t card_id;
    const map<card_id,card> card_dict = {
        {0 , {EMPTY , 0}},
        {1 , {EMPTY , 1}},
        {2 , {EMPTY , 1}},
        {3 , {EMPTY , 2}},
        {4 , {EMPTY , 2}},
        {5 , {EMPTY , 3}},
        {6 , {EMPTY , 1}},
        {7 , {EMPTY , 1}},
        {8 , {EMPTY , 2}},
        {9 , {EMPTY , 2}},
        {10 , {EMPTY , 3}},
        {11 , {EMPTY , 1}},
        {12 , {EMPTY , 1}},
        {13 , {EMPTY , 2}},
        {14 , {EMPTY , 2}},
        {15 , {EMPTY , 3}},
        {16 , {EMPTY , 3}},
        {17 , {EMPTY , 0}},
    };
    struct game
    {
     int8_t status = ONGOING;   // 只要登录 默认游戏正在进行
     int8_t life_player = 5;    // 游戏玩家 5条生命
     int8_t ai_player = 5;      // 游戏玩家 5条生命
     vector<card_id>  deck_player = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}; // 玩家待选的卡牌id
     vector<card_id> deck_ai = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};     // ai待选的卡牌id
     vector<card_id> hand_player = {0,0,0,0}; // 玩家的手里牌
     vector<card_id> hand_ai = {0,0,0,0};     // ai的手里牌
     card_id selected_card_player = 0;          // 从待选牌中选中的牌
     card_id selected_card_ai = 0;              // 从待选牌中选中的牌
    };

  	[[eosio::table]]
  	struct userinfo
  	{
  		account_name name;			//玩家的名字    account_name 是uint64_t的一个别名
  		uint16_t win_count = 0;		
  		uint16_t lost_count = 0;
      game game_data;         // game 

  		auto primary_key() const {return name;}  
  		//多索引表查找名为primary_key（）的getter函数。这必须使用结构中的第一个字段，编译器将使用它来添加主键。让我们定义这个功能。
  	};
    [[eosio::table]]
    struct seed // 这个是用来生成随机数的，全局储存，只要有人玩就会改变，没办法，eos里只能用table来持久化数据在链上
    {
      uint64_t key = 1;
      uint32_t value = 1;

      auto primary_key(){return key;};
    }

  	typedef eosio::multi_index<N(userinfo),userinfo> user_index;
  	//多索引表定义，它有两个参数： 表名 
  	//						 结构定义了我们打算在多索引表中存储的数据。 
  	// 新类型名称作为我们的多索引表定义的别名
  	// 
  	// 多索引表中的数据由四条信息标识： code（account_name） 
  	// 							 scop 
  	// 							 table name
  	// 							 primary key 
    typedef multi_index<N(seed),seed> seed_index;
    user_index _users; //声明表的实例
    seed_index _seed;

    int random(const int range);
    void draw_one_card(vector<uint8_t>& deck,vector<uint8_t>& hand);
};

