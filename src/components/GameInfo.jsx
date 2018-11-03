import React, {Component} from 'react';


export default class extends Component {
    render() {
        const {deckCardCount, handCardCount, onEndGame} = this.props;
        return (
            <div>
                {
                    <p>round : <span>{18 - deckCardCount - handCardCount}/17</span></p>
                }
                <div>
                    <button onClick={onEndGame}>quit</button>
                </div>
            </div>
        )
    }
}