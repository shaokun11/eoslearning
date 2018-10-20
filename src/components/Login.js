import React, {Component} from 'react';
import {connect} from 'react-redux';

import {UserAction} from '../actions/index';
import {ApiService} from "../services/index";


class Login extends Component {
    state = {
        form: {
            username: '',
            key: '',
            error: ''
        }
    };

    constructor(props) {
        super(props);
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleChange(e) {
        const {name, value} = e.target;
        this.setState({
            form: {
                ...this.state.form,
                [name]: value,
                error: ''
            }
        })
    }

    handleSubmit(e) {
        e.preventDefault();
        const {setUser} = this.props;
        const {form} = this.state;
        return ApiService.login(form)
            .then(() => {
                setUser({name: form.username})
            }).catch(err => {
                this.setState({
                    error: err.toString()
                })
            })
    }

    render() {
        return (
            <div>
                <form onSubmit={this.handleSubmit} >
                    <div>
                        <label> account name</label>
                        <input type="text"
                               name='username'
                               placeholder='All small letters, a-z, 1-5 or dot, max 12 characters'
                               onChange={this.handleChange}
                               required
                        />
                    </div>
                    <div>
                        <label> private key</label>
                        <input type="password"
                               name='key'
                               onChange={this.handleChange}
                               required
                        />
                    </div>
                    <div>
                        <button type='submit'>
                            {'confirm'}
                        </button>
                    </div>
                </form>
            </div>
        )
    }
}


const mapStateToProps = state => state;

const mapDispatchToProps = {
    setUser: UserAction.setUser
};

export default connect(mapStateToProps, mapDispatchToProps)(Login);