import React, {Component} from 'react';
import './App.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: "",
      chat: "",
      updateLogClicked: false,
      chatLog: [{
        'Site':'You Can Chat Now!'
      }],
      lastMessageID: 0

    };
    this.nameChange = this.nameChange.bind(this);
    this.chatChange = this.chatChange.bind(this);
    this.sendChat = this.sendChat.bind(this);
    this.getChat = this.getChat.bind(this);
  }
//  {
//               Object.keys(this.props.fruits).map(function(key) {
//                 return <li className="list-group-item list-group-item-info">{this.props.fruits[key]}</li>
//                 }.bind(this))
//               }
  getChat(event) {
    event.preventDefault();
    let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
    fetch( hosturl + '/chat?messageID=' + this.state.lastMessageID, {
      method: 'GET',
      headers: {
          'Content-Type': 'application/json',
      }
    }).then(response => response.json()).then((responseJson) => {
      console.log(responseJson.body)
      this.setState({lastMessageID: responseJson.body[responseJson.body.length - 1].message_id})
    })

  }

  addChat(messages) {


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
      "username": this.state.name,
      "message": this.state.chat
    };
    let data_json = JSON.stringify(data)

    fetch( hosturl + '/chat', {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: data_json
    }).then(response => {
      console.log(response)
    })
  }



  render() {
    return (
      <div id = "app-area" style={{width:'100%',height:'100%'}}>
        <button onClick={(e) => this.getChat(e)}>Update Chats</button>
        <div id = "chat-log-area" style={{width:'100%'}}>
        </div>
        <div id = "message-area" style={{width:'100%'}}>
          <form>
            <label>Name:
              <input type="text" onChange={this.nameChange}/>
            </label>
            <br />
            <label>Chat:
              <input type="text" onChange={this.chatChange}/>
            </label>
            <button onClick={(e) => this.sendChat(e)}>Send</button>
          </form>
        </div>
      </div>
    );
  }
}

export default App;
