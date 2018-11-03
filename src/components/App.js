import React, {Component} from 'react';
import {connect} from 'react-redux';
import Login from './Login';
import Game from './Game';

class App extends Component {
    render() {
        const {user: {name}} = this.props;
        return (
            <div>
                {<Game/>}
                {/*{name && <Game/>}*/}
                {/*{!name && <Login/>}*/}
            </div>
        )
    }
}


const mapStateToProps = state => state;

export default connect(mapStateToProps)(App);