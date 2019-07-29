var socket_io = require('socket.io');
var io = socket_io();
var socketApi = {};
socketApi.io = io;

io.on('connection', (socket) => {
    console.log('A user connected');

    socket.on('disconnect', () => {
      console.log('Client disconnected');
  });
});

socketApi.sendNotification = function(event, message_id, username, message) {
    io.sockets.emit(event, {
        payload: {
          message_id: message_id,
          username: username,
          message: message
        }
      }
    );
}

socketApi.sendAttachementNotification = function(event, message_id, type, username, fileName, fileURL) {
    io.sockets.emit(event, {
        payload: {
          message_id: message_id,
          type: type,
          username: username,
          attachment_name: fileName,
          attachment_location: fileURL
        }
      }
    );
}

module.exports = socketApi;
