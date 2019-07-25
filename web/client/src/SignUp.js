import React, {Component} from 'react';
import './SignUp.css';

class SignUp extends Component {
  constructor(props) {
    super(props);
    this.state = {
      username: false,
      password: "",
      passwordMatch: false
    }
    this.usernameChange = this.usernameChange.bind(this);
    this.passwordChange = this.passwordChange.bind(this);
    this.checkPassword = this.checkPassword.bind(this);
    this.handleSignUp = this.handleSignUp.bind(this);
    this.handleGoBack = this.handleGoBack.bind(this);
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

  checkPassword(event) {
    if (event.target.value) {
      if (this.state.password && this.state.password === event.target.value) {
        this.setState({passwordMatch: true})
      }
      else {
        this.setState({passwordMatch: false})
      }
    }
  }

  handleSignUp(event) {
    event.preventDefault();
    this.props.accountMade();
  }

  handleSignUp(event) {
    event.preventDefault();
    if (this.state.passwordMatch) {
      if (this.state.username) {
        let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
        let data = {
          "username": this.state.username,
          "password": this.state.password
        };
        let data_json = JSON.stringify(data)

        fetch( hosturl + '/users/createUser', {
          method: 'POST',
          headers: {
              'Content-Type': 'application/json',
          },
          body: data_json
        }).then(response => response.json()).then((responseJson) => {
          if (responseJson.body.issue) {
            alert(responseJson.body.issue)
          }
          else {
            this.props.accountMade();
          }
        })
      }
      else {
        alert("Sorry, your username cannot be blank")
      }
    }
    else {
      alert("Sorry, some of the credentials aren't right. Please fix them first then try again")
    }
  }


  render() {
    let passwordStatus;

    if (this.state.passwordMatch && this.state.username) {
      passwordStatus = <p>Everything looks good! You're ready to sign up</p>
    }
    else {
      passwordStatus = <p>Passwords do not match or some fields are blank</p>
    }

    return (
      <div>
      <form id='sign-up-form'>
        <label>Username:
          <input type="text" onChange={this.usernameChange}/>
        </label>
        <br />
        <label>Password:
          <input type="password" onChange={this.passwordChange}/>
        </label>
        <br />
        <label>Confirm Password:
          <input type="password" onChange={this.checkPassword}/>
        </label>
        {passwordStatus}
        <button onClick={(e) => this.handleSignUp(e)}>Sign Up</button>
          <button onClick={(e) => this.handleGoBack(e)}>Go Back</button>
      </form>
      </div>
    )
  }
}

export default SignUp;
