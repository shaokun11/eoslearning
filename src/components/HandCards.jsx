import React, {Component} from 'react';
import Card from './Card'

export default class  extends Component {

    render() {
        const {cards, onPlayCard} = this.props;
        const generateCards = cards => {
            let elems = [];
            for (let i = 0; i < 4; i++) {
                let cardProps = {
                    key: i,
                    cardId: cards[i]
                }
                if (onPlayCard) {
                    cardProps.onClick = () => {
                        onPlayCard(i)
                    }
                }
                elems.push(<div>
                    <Card {...cardProps}/>
                </div>)
            }
            return elems;
        }

        return (
            <div>
                {generateCards(cards)}
            </div>
        )
    }
}

