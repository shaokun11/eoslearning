import React, {Component} from 'react';

import ApiService from './ApiService'
import Game from './Game'
class Login extends Component {
    state = {
        isLogin : false
    }

    login(name, privateKey) {
        return ApiService.login({name: name, key: privateKey})
            .then(() => {
                this.setState({
                    isLogin : true
                })
            }).catch(err => {
                this.setState({
                    error: err.toString()
                })
            })
    }

    render() {
        return (
            <div>
                {this.state.isLogin ? <Game/> :
                    <div>
                        <button
                            onClick={this.login.bind(this, process.env.REACT_APP_EOS_CONTRACT_NAME, process.env.REACT_APP_EOS_PRIVATE_KEY)}>
                            login account3
                        </button>
                        <button
                            onClick={this.login.bind(this, process.env.REACT_APP_EOS_CONTRACT_NAME_S, process.env.REACT_APP_EOS_PRIVATE_KEY_S)}>
                            login account4
                        </button>
                        <button
                            onClick={this.login.bind(this, process.env.REACT_APP_EOS_CONTRACT_NAME_5, process.env.REACT_APP_EOS_PRIVATE_KEY_5)}>
                            login account5
                        </button>
                    </div>
                }
            </div>
        )
    }
}


export default Login