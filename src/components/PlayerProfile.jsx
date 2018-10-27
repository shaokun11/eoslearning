import React, {Component} from 'react';

export default class extends Component {
    render() {
        const {name, winCount, lostCount, onStartGame} = this.props
        return (
            <div>
                <p>user name :{name}</p>
                <p>win count : {winCount}</p>
                <p>lost count : {lostCount}</p>
                <button onClick={onStartGame}>start game</button>
            </div>
        )
    }
}