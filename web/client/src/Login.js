import React, {Component} from 'react';
import './Login.css';
import SignUp from './SignUp.js'

class Login extends Component {
  constructor(props) {
    super(props);

    this.state = {
      hasAccount: true,
      username: "",
      password: ""
    }
    this.handleSignIn = this.handleSignIn.bind(this);
    this.usernameChange = this.usernameChange.bind(this);
    this.passwordChange = this.passwordChange.bind(this);
    this.signInOrCreateAccount = this.signInOrCreateAccount.bind(this);
    this.handleCreateAccount = this.handleCreateAccount.bind(this);
    this.handleAccountCreated = this.handleAccountCreated.bind(this);
  }

  handleSignIn(event) {
    event.preventDefault();
    let user = this.state.username
    let pass = this.state.password
    let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
    let data = {
      "username": user,
      "password": pass
    };
    let data_json = JSON.stringify(data)

    fetch( hosturl + '/users/auth', {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: data_json
    }).then(response => response.json()).then((responseJson) => {
      if (responseJson.body.results.status === "failure") {
        alert(responseJson.body.results.reason)
      }
      else {
        this.props.login(true, user);
      }
    })
  }

  usernameChange(event) {
    if (event.target.value) {
      this.setState({username: event.target.value})
    }
  }

  passwordChange(event) {
    if (event.target.value) {
      this.setState({password: event.target.value})
    }
  }

  handleCreateAccount() {
    this.setState({hasAccount: false})
  }

  handleAccountCreated() {
    this.setState({hasAccount: true})
  }

  signInOrCreateAccount() {
    if (this.state.hasAccount) {
      return (
        <div id="login-area" style={{width:'100%'}}>
          <form id='login-form'>
            <label>Username:
              <input type="text" onChange={this.usernameChange}/>
            </label>
            <br />
            <label>Password:
              <input type="password" onChange={this.passwordChange}/>
            </label>
            <button onClick={(e) => this.handleSignIn(e)}>Login</button>
          </form>
          <button onClick={(e) => this.handleCreateAccount(e)}>Create Account</button>
        </div>
      )
    }
    else {
      return <SignUp accountMade={this.handleAccountCreated}/>
    }
  }

  render() {
    return (
      <div>
        {this.signInOrCreateAccount()}
      </div>
    )
  }
}

export default Login;
