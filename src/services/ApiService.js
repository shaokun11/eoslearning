import {Api, Rpc, SignatureProvider} from 'eosjs';

async function takeAction(action, value) {
    const pk = process.env.Private_Key;
    const rpc = new Rpc.JsonRpc(process.env.REACT_APP_EOS_HTTP_ENDPOINT);
    const provider = new SignatureProvider([pk]);
    const api = new Api({rpc, provider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder()});
    try {
        return await api.transact({
            action: [{
                account: process.env.REACT_APP_EOS_CONTRACT_NAME,
                name: 'action',
                authorization: [{
                    actor: localStorage.getItem('cardgame_account'),
                    permission: 'active'
                }],
                data: value,
            }]
        });
    } catch (err) {
        throw(err);
    }
}

class ApiService {
    static login({username, key}) {
        console.log("api login",username,key);

        return new Promise((resolve, reject) => {
            localStorage.setItem('cardgame_account', username);
            localStorage.setItem("cardgame_key", key);
            //当去区块链执行成功后，将会走这里
            console.log("login success");
            resolve()
            // takeAction("login", {username}).then(() => {
            //     resolve();
            // }).catch(err => {
            //     localStorage.removeItem("cardgame_account");
            //     localStorage.removeItem("cardgame_key");
            //     reject(err);
            // })
        });
    }
}

export default ApiService;