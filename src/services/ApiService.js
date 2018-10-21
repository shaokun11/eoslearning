import {Api, JsonRpc, RpcError, JsSignatureProvider} from 'eosjs';

async function takeAction(action, value) {
    const pk = process.env.REACT_APP_EOS_PRIVATE_KEY;
    const rpc = new JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
    const provider = new JsSignatureProvider([pk]);
    const api = new Api({rpc, provider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder()});
    try {
        return await api.transact({
            action: [{
                account: process.env.REACT_APP_EOS_CONTRACT_NAME,      // 合约的拥有这
                name: 'action',
                authorization: [{
                    actor: localStorage.getItem('cardgame_account'),  // 用户输入的账号名
                    permission: 'active'   // 所需的权限
                }],
                data: value,    // 携带的参数
            }]
        }, {
            blocksBehind: 3,   // 3个节点确认后
            expireSeconds: 30, // 超时时间 30秒
        });
    } catch (e) {
        console.log(JSON.stringify(e.json, null, 2))
        if (e instanceof RpcError)
            console.log(JSON.stringify(e.json, null, 2))
    }
}

class ApiService {
    static login({username, key}) {
        return new Promise((resolve, reject) => {
            localStorage.setItem('cardgame_account', username);
            localStorage.setItem("cardgame_key", key);
            // 执行登录，这里只需要用户的account 即可
            takeAction("login", {username}).then(() => {
                console.log("login success");
                resolve();
            }).catch(err => {
                localStorage.removeItem("cardgame_account");
                localStorage.removeItem("cardgame_key");
                console.log(err)
                reject(err);
            })
        });
    }
}

export default ApiService;