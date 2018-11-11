// React core
import React, {Component} from 'react';

import ApiService from "./ApiService"



class Game extends Component {

    state = {
        name :localStorage.getItem("cardgame_account"),
        loadingSuccess:false

    }

    constructor(props) {
        super(props);
        this.loadUser = this.loadUser.bind(this)
        this.startGame = this.startGame.bind(this)
        this.endGame = this.endGame.bind(this)
        this.playcard = this.playcard.bind(this)
        this.nextround = this.nextround.bind(this)
        this.loadUser()
    }

    loadUser() {
        ApiService.getUserByName(this.state.name).then(user => {
            console.log(user)
            this.setState({
                data:user,
                loadingSuccess:true
            })
        })
    }

    startGame() {
        return ApiService.startgame(this.state.name).then(() => {
            return this.loadUser();
        });
    }

    endGame(){
        return ApiService.endgame(this.state.name).then(() => {
            return this.loadUser();
        });
    }
    playcard(){
        return ApiService.playcard(this.state.name, Math.floor(Math.random()*10000)%4).then(() => {
            return this.loadUser();
        });
    }
    nextround(){
        return ApiService.nextround(this.state.name).then(() => {
            return this.loadUser();
        });
    }

    render() {

        return (
            <div>
                Welcome to shaokun eos learning
                <br/>
                <div>
                    {this.state.loadingSuccess ?
                        <div>
                            <button onClick={this.startGame}>
                                restart game
                            </button>
                            <button onClick={this.endGame}>
                               end game
                            </button>
                            <button onClick={this.playcard}>
                                select card
                            </button>
                            <button onClick={this.nextround}>
                                next round
                            </button>
                            <p>name :{this.state.data.name}</p>
                            <p>win count: {this.state.data.win_count}</p>
                            <p>lost count: {this.state.data.lost_count}</p>
                            <p>statu: {this.state.data.game_data.status}</p>
                            <p>life player: {this.state.data.game_data.life_player}</p>
                            <p>life ai : {this.state.data.game_data.life_ai}</p>

                            <p>deck player: {this.state.data.game_data.deck_player.join(', ')}</p>
                            <p>ai player: {this.state.data.game_data.deck_ai.join(', ')}</p>
                            <p>hand player: {this.state.data.game_data.hand_player.join(', ')}</p>
                            <p>hand ai :{this.state.data.game_data.hand_ai.join(', ')}</p>
                            <p>selected card player: {this.state.data.game_data.selected_card_player}</p>
                            <p>selected card ai: {this.state.data.game_data.selected_card_ai}</p>
                            <p>life_lost_player : {this.state.data.game_data.life_lost_player}</p>
                            <p>life_lost_ai : {this.state.data.game_data.life_lost_ai}</p>
                        </div>
                       : <p>loading game info</p>
                    }
                </div>

            </div>
        )
    }
}


export default Game;