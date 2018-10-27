// React core
import React, {Component} from 'react';
import {connect} from 'react-redux'
import {UserAction} from "../actions/index";
import PlayerProfile from './PlayerProfile'
import ApiService from "../services/ApiService"

class Game extends Component {

    constructor(props) {
        super(props);
        this.loadUser = this.loadUser.bind(this)
        this.loadUser() //当登录成功，里即加载数据
    }

    loadUser() {
        const user = this.props.user
        const {setUser} = this.props;
        ApiService.getUserByName(user.name).then(user => {
            setUser({
                win_count: user.win_count,
                lost_count: user.lost_count
            })
        })
    }

    render() {
        const {user: {name, win_count, lost_count}} = this.props;
        return (
            <section>
                Welcome to shaokun eos learning
                <PlayerProfile
                    name={name}
                    winCount={win_count}
                    lostCount={lost_count}
                />
            </section>
        )
    }

}


const mapStateToProps = state => state

const mapDispatchToProps = {
    setUser: UserAction.setUser
}

export default connect(mapStateToProps, mapDispatchToProps)(Game);