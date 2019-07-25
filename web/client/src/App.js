import React, {Component} from 'react';
import './App.css';
import ChatRoom from './ChatRoom.js';
import Login from './Login.js';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      loggedIn: false,
      username: ""
    }
    this.handleLogin = this.handleLogin.bind(this);
    this.handleLogout = this.handleLogout.bind(this);
  }

  handleLogin(validated, user) {
    if (validated) {
      this.setState({loggedIn: true, username: user})
    }
  }.bind(this)

  handleLogout() {
    if (this.state.loggedIn) {
      this.setState({loggedIn: false})
    }
  }.bind(this)

  pickSite() {
    if(this.state.loggedIn) {
      return <ChatRoom logout={this.handleLogout()} user={this.state.username}/>
    }
    else {
      return <Login login={this.handleLogin}/>
    }
  }.bind(this)

  render() {
    return (
      <div>
        {this.pickSite()}
      </div>
    );
  }
}

export default App;
