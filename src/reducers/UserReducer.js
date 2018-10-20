import ActionTypes from '../actions/actionTypes';

const initState = {
    name: '',
    win_count: 0,
    lost_count: 0,
    game: null
};

export default function (state = initState, action) {
    switch (action.type) {
        case ActionTypes.SET_USER: {
            return {
                ...state,
                name: typeof action.name === "undefined" ? state.name : action.name,
                win_count: action.win_count || initState.win_count,
                lost_count: action.lost_count || initState.lost_count,
                game: action.game || initState.game
            }
        }
        default:
            return state;

    }
}