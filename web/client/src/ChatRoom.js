import React, {Component} from 'react';
import './ChatRoom.css';

import socketIOClient from 'socket.io-client';

class ChatRoom extends Component {
  constructor(props) {
    super(props);
    this.state = {
      chat: "",
      lastMessageID: 0,
      firstMessageLoadedID: -1,
      messages: [],
      attachment: null,
      message: "",
      endpoint: 'https://ec2-18-188-142-129.us-east-2.compute.amazonaws.com:3001/',
      host: 'https://ec2-18-188-142-129.us-east-2.compute.amazonaws.com:3001'
    }
    this.addChat = this.addChat.bind(this);
    this.chatChange = this.chatChange.bind(this);
    this.fileChange = this.fileChange.bind(this);
    this.sendChat = this.sendChat.bind(this);
    this.sendFile = this.sendFile.bind(this);
    this.handleOldChat = this.handleOldChat.bind(this);
    this.handleNewChat = this.handleNewChat.bind(this);
    this.fetchOldChat = this.fetchOldChat.bind(this);
    this.fetchInitialChat = this.fetchInitialChat.bind(this);
  }

  componentDidMount() {
    const { endpoint } = this.state;
    const socket = socketIOClient(endpoint);
    this.fetchInitialChat();
    socket.on('messages', data => {
      if (this.state.firstMessageLoadedID === -1) {
        this.setState({firstMessageLoadedID: data.payload.message_id})
      }
      this.setState(state => ({messages: [[state.message], ...state.messages]}))
      this.setState(state => ({message: data.payload}))
      this.setState({lastMessageID: data.payload.message_id})
    })
    socket.on('attachment', data => {
      if (this.state.firstMessageLoadedID === -1) {
        this.setState({firstMessageLoadedID: data.payload.message_id})
      }
      this.setState(state => ({messages: [[state.message], ...state.messages]}))
      this.setState(state => ({message: data.payload}))
      this.setState({lastMessageID: data.payload.message_id})
    })
  }

  fetchOldChat() {
    let stoppingPoint;
    if (this.state.firstMessageLoadedID === -1) {
      this.setState({firstMessageLoadedID: 0})
      stoppingPoint = this.state.lastMessageID;
    }
    else {
      stoppingPoint = this.state.firstMessageLoadedID;
    }

    let hosturl = this.state.host;
    fetch( hosturl + '/chat/old?messageID=' + stoppingPoint, {
      method: 'GET',
      headers: {
          'Content-Type': 'application/json',
      }
    }).then(response => response.json()).then((responseJson) => {
      if (responseJson.body[0]) {
        this.setState({lastMessageID: responseJson.body[responseJson.body.length - 1].message_id})
        let jsonData = responseJson.body;
        jsonData.reverse();
        this.setState(state => ({messages: [jsonData, ...state.messages]}))
        if (this.state.firstMessageLoadedID !== 0) {
          this.setState({firstMessageLoadedID: 0})
        }
      }
    })
  }

  fetchInitialChat() {
    if (this.state.firstMessageLoadedID === -1) {
      this.setState({firstMessageLoadedID: 0})
    }

    let hosturl = this.state.host;
    fetch( hosturl + '/chat?messageID=' + this.state.lastMessageID, {
      method: 'GET',
      headers: {
          'Content-Type': 'application/json',
      }
    }).then(response => response.json()).then((responseJson) => {
      if (responseJson.body[0]) {
        this.setState({lastMessageID: responseJson.body[responseJson.body.length - 1].message_id})
        let jsonData = responseJson.body;
        jsonData.reverse();
        this.setState(state => ({messages: [jsonData, ...state.messages]}))
        if (this.state.firstMessageLoadedID !== 0) {
          this.setState({firstMessageLoadedID: 0})
        }
      }
    })
  }

  addChat(event) {
    event.preventDefault();
    this.fetchOldChat();
  }

  chatChange(event) {
    if (event.target.value) {
      this.setState({chat: event.target.value})
    }
  }

  fileChange(event) {
    if (event.target.files) {
      this.setState({attachment: event.target.files[0]})
    }
  }

  sendChat(event) {
    event.preventDefault();
    if (this.state.chat) {
      let hosturl = this.state.host;
      let data = {
        "username": this.props.user,
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
  }

  sendFile(event) {
    event.preventDefault();
    let form = new FormData();
    form.append('username', this.props.user);
    form.append('file', this.state.attachment)
    fetch( this.state.host + '/file', {
      method: 'POST',
      mode: 'no-cors',
      body: form
    }).then(response => {
      this.setState({attachment: null})
      this.refs.filebox.value = null;
    })
  }

  handleOldChat() {
    return (
        this.state.messages.map(messageData => {
          return messageData.map(dataItem => {
            if (dataItem) {
              if (!dataItem.attachment_location) {
                if (dataItem.message) {
                  return (
                    <React.Fragment key={dataItem.message_id}>
                      <div style={{border: '2px solid blue'}}>
                        <h4>{dataItem.username}</h4>
                        <p>{dataItem.message}</p>
                      </div>
                    </React.Fragment>
                  )
                }
                else {
                  return
                }
              }
              else {
                if (dataItem.type.includes('image')) {
                  return (
                    <React.Fragment key={dataItem.message_id}>
                      <div style={{border: '2px solid blue'}}>
                        <h4>{dataItem.username}</h4>
                        <img src={dataItem.attachment_location} alt={dataItem.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
                      </div>
                    </React.Fragment>
                  )
                }
                else if (dataItem.type.includes('video')) {
                  return (
                    <React.Fragment key={dataItem.message_id}>
                      <div style={{border: '2px solid blue'}}>
                        <h4>{dataItem.username}</h4>
                        <video controls src={dataItem.attachment_location} alt={dataItem.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
                      </div>
                    </React.Fragment>
                  )
                }
                else if (dataItem.type === 'false') {
                  return
                }
                else {
                  return (
                    <React.Fragment key={dataItem.message_id}>
                      <div style={{border: '2px solid blue'}}>
                        <h4>{dataItem.username}</h4>
                        <embed src={dataItem.attachment_location} alt={dataItem.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
                      </div>
                    </React.Fragment>
                  )
                }
              }
            }
            else {
              return
            }
          }
        )})
      )
  }

  handleNewChat() {
    if (this.state.message) {
      if (!this.state.message.attachment_location ) {
        return (
          <React.Fragment key={this.state.message.message_id}>
            <div style={{border: '2px solid red'}}>
              <h4>{this.state.message.username}</h4>
              <p>{this.state.message.message}</p>
            </div>
          </React.Fragment>
        )
      }
      else {
        if (this.state.message.type.includes('image')) {
          return (
            <React.Fragment key={this.state.message.message_id}>
              <div style={{border: '2px solid red'}}>
                <h4>{this.state.message.username}</h4>
                <img src={this.state.message.attachment_location} alt={this.state.message.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
              </div>
            </React.Fragment>
          )
        }
        else if (this.state.message.type.includes('video')) {
          return (
            <React.Fragment key={this.state.message.message_id}>
              <div style={{border: '2px solid red'}}>
                <h4>{this.state.message.username}</h4>
                <video controls src={this.state.message.attachment_location} alt={this.state.message.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
              </div>
            </React.Fragment>
          )
        }
        else if (this.state.message.type === 'false') {
          return
        }
        else {
          return (
            <React.Fragment key={this.state.message.message_id}>
              <div style={{border: '2px solid red'}}>
                <h4>{this.state.message.username}</h4>
                <embed src={this.state.message.attachment_location} alt={this.state.message.attachment_name} style={{maxWidth: "400px", maxHeight: "400px", width: "auto", height: "auto"}} />
              </div>
            </React.Fragment>
          )
        }
      }
    }
    else {
      return
    }
  }

  render() {
    return (
      <div id = "app-area" style={{width:'100%',height:'100%'}}>
        <div id = "message-area" style={{width:'450px'}}>
          <form id="chat-form">
            <label>Chat:
              <input type="text" onChange={this.chatChange} ref="chatbox"/>
            </label>
            <button onClick={(e) => this.sendChat(e)}>Send</button>
          </form>
          <form id="file-form">
            <label>File:
              <input type="file" onChange={this.fileChange} ref="filebox"/>
            </label>
            <button onClick={(e) => this.sendFile(e)}>Submit</button>
          </form>
        </div>
        <button onClick={(e) => this.addChat(e)}>Update Chats</button>
        <div id = "chat-log-area" style={{width:'450px'}}>
          <div id = "new-chat-log-area">
            {this.handleNewChat()}
          </div>
          <div id = "old-chat-log-area">
            {this.handleOldChat()}
          </div>
        </div>
      </div>
    );
  }
}

export default ChatRoom;
