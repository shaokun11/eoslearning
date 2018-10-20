#include "gameplay.cpp"
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
EOSIO_ABI(cardgame,(login))
