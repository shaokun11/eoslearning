import React, {Component} from 'react';

export default class extends Component {
    render() {
        const {name, life} = this.props

        return (
            <div>
                <div>name : {name}</div>
                <div>life : {life}</div>
                <div>{life < 0 ? 0 : life}/5</div>
            </div>
        )
    }

}
