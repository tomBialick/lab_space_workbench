import React, {Component} from 'react';
import './App.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: "",
      chat: ""
    };
    this.nameChange = this.nameChange.bind(this);
    this.chatChange = this.chatChange.bind(this);
    this.sendChat = this.sendChat.bind(this)
  }

  nameChange(event) {
    if (event.target.value) {
      this.setState({name: event.target.value})
    }
  }

  chatChange(event) {
    if (event.target.value) {
      this.setState({chat: event.target.value})
    }
  }

  sendChat(event) {
    event.preventDefault();
    let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
    let data = {
      "name": this.state.name,
      "chat": this.state.chat
    };
    let data_json = JSON.stringify(data)

    fetch( hosturl + '/chat', {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: data_json
    }).then(response => response.json()).then((responseJson) => {
      alert(responseJson.body)
    })
  }



  render() {
    return (
      <div id = "app-area" style={{width:'100%',height:'100%'}}>
        <div id = "chat-log-area" style={{width:'100%'}}>
        </div>
        <div id = "message-area" style={{width:'100%'}}>
          <form>
            <label>Name:
              <input type="text" onChange={(e) => this.nameChange()}/>
            </label>
            <label>Chat:
              <input type="text" onChange={(e) => this.chatChange()}/>
            </label>
            <button onClick={(e) => this.sendChat(e)}>Send</button>
          </form>
        </div>
      </div>
    );
  }
}

export default App;
