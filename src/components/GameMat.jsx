import React, {Component} from 'react';


import HandCards from "./HandCards";
import PlayerInfo from "./PlayerInfo";

export default class extends Component {
    render() {
        const {deckCardCount, aiLife, aiHandCards, aiName, playerLife, playerHandCards, playerName, onPlayCard} = this.props;
        return (
            <div>
                <div>
                    <PlayerInfo
                        name={aiName}
                        life={aiLife}
                    />
                    <div>
                        {aiName}'S Deck ({deckCardCount})
                    </div>
                    <HandCards
                        cards={aiHandCards}
                    />

                </div>
                <p>..............</p>
                <div>
                    <PlayerInfo
                        name={playerName}
                        life={playerLife}
                    />
                    <div>
                        {playerName}'S Deck ({deckCardCount})
                    </div>
                    <HandCards
                        cards={playerHandCards}
                        onPlayCard={onPlayCard}
                    />
                </div>
            </div>
        )
    }
}