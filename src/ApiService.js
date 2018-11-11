import {Api, JsonRpc, JsSignatureProvider} from 'eosjs';

async function takeAction(action, account, pk, dataValue) {
    console.log("takeAction", action,pk, dataValue)
    const rpc = new JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
    const signatureProvider = new JsSignatureProvider([pk]);
    const api = new Api({rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder()});
    try {
        return await api.transact({
            actions: [{
                account: process.env.REACT_APP_EOS_CONTRACT_NAME_5,  // 合约的拥有者
                name: action,             // 执行的合约方法
                authorization: [{
                    actor: account,       // 执行合约的账户
                    permission: 'active', // 执行合约的账户所需要的权限
                }],
                data: dataValue,          // 所需要的参数,以json字符串进行传递，注意key为合约中定义的参数的名字，也可以通过abi进行查看
            }]
        }, {
            blocksBehind: 3,              // 多少个块后确认
            expireSeconds: 30,            // 超时时间
        });
    } catch (err) {
        throw(err)
    }
}

class ApiService {

    // 结束游戏 不再保存游戏的状态
    static endgame(user) {
        return takeAction("endgame",user,localStorage.getItem("cardgame_key"), {user})
    }

    // 选牌，进行比较
    static playcard(user,index) {
        return takeAction("playcard",user,localStorage.getItem("cardgame_key"), {user,index})
    }

    // 继续比赛，会根据上一轮的结果，选择下一轮的可选牌
    static nextround(user) {
        return takeAction("nextround",user,localStorage.getItem("cardgame_key"), {user})
    }

    // 重新开始游戏，会随机产生手里牌至初始状态
    static startgame(user) {
        return takeAction("startgame",user,localStorage.getItem("cardgame_key"), {user})
    }

    static async getUserByName(user) {
        const rpc = new JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
        const res = await rpc.get_table_rows({
            code: process.env.REACT_APP_EOS_CONTRACT_NAME_5,             //由于在合约中写死了，这里也可以写死
            scope: process.env.REACT_APP_EOS_CONTRACT_NAME_5,            //由于在合约中写死了，这里也可以写死
            table: "userinfo",                                         //由于在合约中写死了，这里也可以写死
            json: true,                                                // 默认值 ，可以不填
            lower_bound: user,                                        // 匹配规则，不填默认返回全部
        })
        return res.rows[0];
    }

    static login({name, key}) {
        return new Promise((resolve, reject) => {
            localStorage.setItem("cardgame_key", key);
            localStorage.setItem("cardgame_account", name);
            takeAction("login", name, key, {user: name}).then(res => {
                resolve();
            }).catch(err => {
                localStorage.removeItem("cardgame_key");
                localStorage.removeItem("cardgame_account");
                console.log(err)
                reject(err);
            })
        });
    }
}

export default ApiService;