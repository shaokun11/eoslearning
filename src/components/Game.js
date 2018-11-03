// React core
import React, {Component} from 'react';
import {connect} from 'react-redux'
import {UserAction} from "../actions/index";
import PlayerProfile from './PlayerProfile'
import ApiService from "../services/ApiService"
import GameInfo from './GameInfo';
import GameMat from './GameMat';


class Game extends Component {

    constructor(props) {
        super(props);
        this.loadUser = this.loadUser.bind(this)
        this.handleStartGame = this.handleStartGame.bind(this)
        this.loadUser() //当登录成功，里即加载数据
    }

    loadUser() {
        const user = this.props.user
        const {setUser} = this.props;
        ApiService.getUserByName(user.name).then(user => {
            setUser({
                name: user.name,
                win_count: 1,
                lost_count: 1,
                game: {
                    status: 0,
                    life_player: 5,
                    life_ai: 5,
                    deck_player: [5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17],
                    deck_ai: [1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17],
                    hand_player: [1, 2, 3, 4],
                    hand_ai: [5, 6, 7, 8],
                    selected_card_player: 0,
                    selected_card_ai: 0,
                }

            })
        })
    }

    handleStartGame() {
        const user = this.props.user
        return ApiService.startgame(user.name).then(() => {
            return this.loadUser();
        });
    }


    render() {
        const {user: {name, win_count, lost_count, game}} = this.props;
        const isGameStarted = game && game.deck_ai.length !== 17;
        return (
            <section>
                Welcome to shaokun eos learning
                {
                    !isGameStarted ? <PlayerProfile
                            name={name}
                            winCount={win_count}
                            lostCount={lost_count}
                            onStartGame={this.handleStartGame}/> :
                        <div>
                            <GameMat
                                deckCardCount={game.deck_ai.length}
                                aiLife={game.life_ai}
                                aiHandCards={game.hand_ai}
                                aiName="computer"
                                playerLife={game.life_player}
                                playerHandCards={game.hand_player}
                                playerName={name}
                            />
                            <GameInfo
                                deckCardCount={game.deck_ai.length}
                                handCardCount={game.hand_ai.filter(x => x > 0).length}
                            />
                        </div>
                }

            </section>
        )
    }

}

const mapStateToProps = state => state

const mapDispatchToProps = {
    setUser: UserAction.setUser
}

export default connect(mapStateToProps, mapDispatchToProps)(Game);