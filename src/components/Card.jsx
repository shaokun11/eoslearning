import React, {Component} from 'react';

// card type =>card power
const cardDict = [
    [0, 0],
    [1, 1], [1, 1],
    [1, 2], [1, 2],
    [1, 3],
    [2, 1], [2, 1],
    [2, 2], [2, 2],
    [2, 3],
    [3, 1], [3, 1],
    [3, 2], [3, 2],
    [3, 3],
    [4, 3],
    [5, 0]
];

class Card extends Component {
    render() {
        const {cardId, onClick} = this.props;
        const Tag = !!cardId && onClick ? 'a' : 'span'
        let cardType = ""
        switch (cardDict[cardId][0]) {
            case 1:
                cardType = "FIRE";
                break;
            case 2:
                cardType = "WOOD";
                break;
            case 3:
                cardType = "WATER";
                break;
            case 4:
            case 5:
                cardType = "SPECIAL";
                break;
            default:
                cardType = "EMPTY";
                break;
        }
        return (
            <Tag onClick={onClick}>
                <span>card type : {cardType} --- </span>
                <span>card power: {cardId !== 0 && cardDict[cardId][1]}</span>
            </Tag>
        )
    }
}

export default Card