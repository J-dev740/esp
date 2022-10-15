const {getUsers, users} = require('./getUsers');
//socket connections
 
function socket(io) {
    io.on('connection',(socket)=>{
        socket.on('joined-user', (data) =>{
            //Storing users connected in a room in memory
            var user = {};
            user[socket.id] = data.username;
            if(users[data.roomname]){
                users[data.roomname].push(user);
            }
            else{
                users[data.roomname] = [user];
            }
            
            //Joining the Socket Room
            socket.join(data.roomname);       
            //Emitting New Username to Clients
            io.to(data.roomname).emit('joined-user', {username: data.username});
            //Send online users array
            io.to(data.roomname).emit('online-users', getUsers(users[data.roomname]));
        });
        socket.on('chat', (data) =>{
            io.to(data.roomname).emit('chat', {username: data.username, message: data.message});
        });
         //Broadcasting the user who is typing
        socket.on('typing', (data) => {
            socket.broadcast.to(data.Roomname).emit('typing', data.Username);
       });
         socket.on('left', (data)=>{
          // var rooms = Object.keys(socket.rooms);
            var socketId = data.sock;
           // console.log(user[socketId]);
             //var roomname = rooms[1];
             var user=users[data.roomname];
             users[roomname].foreach((user) => {
                 if(user[socketId]){
             users[data.roomname].splice(users[data.roomname].indexOf(user[socketId]), 1);
             
                 }
             });
    
            //Send online users array
            io.to(data.roomname).emit('left',{sock:socketId});
            io.to(data.roomname).emit('online-users', getUsers(users[data.roomname]));
        });
    });
}
module.exports = socket;