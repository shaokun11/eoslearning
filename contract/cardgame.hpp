#include <eosiolib/eosio.hpp>
using namespace std;
class cardgame : public eosio::contract {
// 继承contract
  public:
    cardgame( account_name self ):contract(self),_users(self,self){} 
    //构造方法,并实例化了multi_index表users
    
   	/// @abi action 
    void login(account_name user);
    // table和action 必须加上@abi 的注解，这样才能生存对应的abi，
    // 在eosio.cdt中，使用 [[eosio::action]] 更加简洁
  private:
  	/// @abi table users
  	struct user_info
  	{
  		account_name name;			//玩家的名字    account_name 是uint64_t的一个别名
  		uint16_t win_count = 0;		
  		uint16_t lost_count = 0;

  		auto primary_key() const {return name;}  
  		//多索引表查找名为primary_key（）的getter函数。这必须使用结构中的第一个字段，编译器将使用它来添加主键。让我们定义这个功能。
  	};

  	typedef eosio::multi_index<N(user_info),user_info> user_table;
  	//多索引表定义，它有两个参数： 表名 
  	//						 结构定义了我们打算在多索引表中存储的数据。 
  	// 新类型名称作为我们的多索引表定义的别名
  	// 
  	// 多索引表中的数据由四条信息标识： code（account_name） 
  	// 							 scop 
  	// 							 table name
  	// 							 primary key 
  	user_table _users; //声明表的实例
};
