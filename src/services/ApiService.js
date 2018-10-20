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

            resolve("hello shaokun")
            // takeAction("login", {username}).then(res => {
            //     resolve(res);
            // }).catch(err => {
            //     localStorage.removeItem("cardgame_account");
            //     localStorage.removeItem("cardgame_key");
            //     reject(err);
            // })
        });
    }
}

export default ApiService;