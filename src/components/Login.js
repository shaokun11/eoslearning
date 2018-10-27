import React, {Component} from 'react';
import {connect} from 'react-redux';

import {UserAction} from '../actions/index';
import {ApiService} from "../services/index";


class Login extends Component {

    login(name, privateKey) {
        console.log(name,privateKey)
        const {setUser} = this.props;

        return ApiService.login({name: name, key: privateKey})
            .then(() => {
                setUser({name: name})
            }).catch(err => {
                this.setState({
                    error: err.toString()
                })
            })
    }

    render() {
        return (
            <div>
                <button
                    onClick={this.login.bind(this, process.env.REACT_APP_EOS_CONTRACT_NAME, process.env.REACT_APP_EOS_PRIVATE_KEY)}>
                    login account3
                </button>
                <button
                    onClick={this.login.bind(this, process.env.REACT_APP_EOS_CONTRACT_NAME_S, process.env.REACT_APP_EOS_PRIVATE_KEY_S)}>
                    login account4
                </button>
            </div>
        )
    }
}


const mapStateToProps = state => state;

const mapDispatchToProps = {
    setUser: UserAction.setUser
};

export default connect(mapStateToProps, mapDispatchToProps)(Login);