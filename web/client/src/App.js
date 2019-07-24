import React, {Component} from 'react';
import './App.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: "",
      chat: "",
      lastMessageID: 0,
      messages: []
    }
    this.addChat = this.addChat.bind(this);
    this.nameChange = this.nameChange.bind(this);
    this.chatChange = this.chatChange.bind(this);
    this.sendChat = this.sendChat.bind(this);
    //this.messageDataItem = this.messageDataItem.bind(this);
  }

  addChat(event) {
    event.preventDefault();
    let hosturl = 'https://ec2-13-58-163-102.us-east-2.compute.amazonaws.com:3001';
    this.setState(state => ({updateLogClicked: !state.updateLogClicked}))
    fetch( hosturl + '/chat?messageID=' + this.state.lastMessageID, {
      method: 'GET',
      headers: {
          'Content-Type': 'application/json',
      }
    }).then(response => response.json()).then((responseJson) => {
      //console.log(responseJson.body)
      //<h4>{messageData.username}</h4>
      if (responseJson.body[0]) {
        this.setState({lastMessageID: responseJson.body[responseJson.body.length - 1].message_id})
        this.setState(state => ({messages: [ ...state.messages, responseJson.body]}))
      }
    })
  }

  /*messageDataItem(m_data) {
    console.log("messageDataItem")
    console.log(m_data)
    return (<div>
              <h4>{m_data.username}</h4>
              <p>{m_data.message}</p>
            </div>)
  }*/

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
      console.log("message sent to server")
    })
  }
//return this.messageDataItem(messageData)
  render() {
    return (
      <div id = "app-area" style={{width:'100%',height:'100%'}}>
        <button onClick={(e) => this.addChat(e)}>Update Chats</button>
        <div id = "chat-log-area" style={{width:'100%'}}>
          {this.state.messages.map(messageData => {
            return messageData.map(dataItem => (
              <React.Fragment key={dataItem.message_id}>
                <h4>{dataItem.username}</h4>
                <p>{dataItem.message}</p>
              </React.Fragment>
            ))
          })}
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
