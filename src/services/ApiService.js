import {Api, JsonRpc, JsSignatureProvider} from 'eosjs';

async function takeAction(action, account, pk, dataValue) {
    console.log("takeAction", action, dataValue)
    const rpc = new JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
    const signatureProvider = new JsSignatureProvider([pk]);
    const api = new Api({
        rpc,
        signatureProvider,
        textDecoder: new TextDecoder(),
        textEncoder: new TextEncoder()
    });
    try {
        return await api.transact({
            actions: [{
                account: "shaokun11113",  // 合约的拥有者
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

    static async getUserByName(username) {
        const rpc = new JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
        const res = await rpc.get_table_rows({
            code: process.env.REACT_APP_EOS_CONTRACT_NAME,             //由于在合约中写死了，这里也可以写死
            scope: process.env.REACT_APP_EOS_CONTRACT_NAME,             //由于在合约中写死了，这里也可以写死
            table: "userinfo",                                           //由于在合约中写死了，这里也可以写死
            json: true,                                                // 默认值 ，可以不填
            lower_bound: username,                                      // 匹配规则，不填默认返回全部
        })
        return res.rows[0];
    }

    static login({name, key}) {
        return new Promise((resolve, reject) => {
            takeAction("login", name, key, {user: name}).then(res => {
                // 执行成功会返回默认的交易hash
                console.log("login success", res);
                resolve();
            }).catch(err => {
                console.log(err)
                reject(err);
            })
        });
    }
}

export default ApiService;