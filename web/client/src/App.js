import React, {Component} from 'react';
import './App.css';

import socketIOClient from 'socket.io-client';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: "",
      chat: "",
      lastMessageID: 0,
      messages: [],
      message: "",
      endpoint: 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001'
    }
    this.addChat = this.addChat.bind(this);
    this.nameChange = this.nameChange.bind(this);
    this.chatChange = this.chatChange.bind(this);
    this.sendChat = this.sendChat.bind(this);
    this.handleOldChat = this.handleOldChat.bind(this);
    this.handleNewChat = this.handleNewChat.bind(this);
    this.fetchOldChat = this.fetchOldChat.bind(this);
  }

  componentDidMount() {
    const { endpoint } = this.state;
    const socket = socketIOClient(endpoint);
    socket.on('messages', data => {
      this.setState(state => ({message: data.payload}))
      this.fetchOldChat()
    })
  }

  fetchOldChat() {
    let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
    fetch( hosturl + '/chat?messageID=' + this.state.lastMessageID, {
      method: 'GET',
      headers: {
          'Content-Type': 'application/json',
      }
    }).then(response => response.json()).then((responseJson) => {
      if (responseJson.body[0]) {
        let jsonData = responseJson.body;
        jsonData.reverse();
        this.setState({lastMessageID: responseJson.body[responseJson.body.length - 1].message_id})
        this.setState(state => ({messages: [jsonData, ...state.messages]}))
      }
    })
  }


  addChat(event) {
    event.preventDefault();
    this.fetchOldChat();
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
      this.setState({chat: ""})
      this.refs.chatbox.value = '';
    })
  }

  handleOldChat() {
    return (
        this.state.messages.map(messageData => {
          return messageData.map(dataItem => (
            <React.Fragment key={dataItem.message_id}>
              <h4>{dataItem.username}</h4>
              <p>{dataItem.message}</p>
            </React.Fragment>
          )
        )})
      )
  }

  handleNewChat() {
    return (
      <React.Fragment key={this.state.message.message_id}>
        <h4>{this.state.message.username}</h4>
        <p>{this.state.message.message}</p>
      </React.Fragment>
    )
  }

  render() {
    return (
      <div id = "app-area" style={{width:'100%',height:'100%'}}>
        <div id = "message-area" style={{width:'100%'}}>
          <form>
            <label>Name:
              <input type="text" onChange={this.nameChange}/>
            </label>
            <br />
            <label>Chat:
              <input type="text" onChange={this.chatChange} ref="chatbox"/>
            </label>
            <button onClick={(e) => this.sendChat(e)}>Send</button>
          </form>
        </div>
        <button onClick={(e) => this.addChat(e)}>Update Chats</button>
        <div id = "chat-log-area" style={{width:'100%'}}>
          <div id = "old-chat-log-area" style={{width:'100%'}}>
            {this.handleOldChat()}
          </div>
        </div>
      </div>
    );
  }
}

export default App;
