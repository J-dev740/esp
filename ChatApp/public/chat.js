//client-side api to communicate with socket.js file which serves on the server side
//triggering events
// >click on send
// >typing in message space 
// >when a user joins
// >when a user leaves
// >online-users
//to get HTML elements
const output=document.getElementById('output');
const message = document.getElementById('message');
const send = document.getElementById('send');
const feedback = document.getElementById('feedback');
const roomMessage = document.querySelector('.room-message');
const users = document.querySelector('.users');
//Socket server url
const socket=io.connect('http://localhost:3000');

//Fetch URL Params from URL
const queryString=window.location.search;
const urlParams = new URLSearchParams(queryString);
const username = urlParams.get("username");

const roomname = urlParams.get("roomname");
console.log(username);

//display room name to users
roomMessage.innerHTML=`Your room:${roomname}`;
console.log(roomname)
// console.log(username);
// console.log(roomname);
//emit event when a user joins the chat
socket.emit('joined-user',{
    username :username,
    roomname :roomname
});

//sending data when the user clicks send
send.addEventListener("click", ()=>{
    socket.emit('chat',{
        username:username,
        message: message.value,
        roomname:roomname
    });
    message.value ='';
    console.log('sent the message');
});

//sending username if the user is typing
message.addEventListener("keypress",()=>{
    socket.emit('typing',{Username:username, Roomname:roomname});
});
//displaying that the user has joined the room
socket.on('joined-user', (data)=>{
    output.innerHTML +='<p>--> <strong><em>' + data.username + ' </strong>has Joined the Room</em></p>';
    users.innerHTML=`<p> <p> `;
    // console.log(data.message);
    // output.innerText+=data.username;
});

//displaying the message sent from the user
socket.on('chat', (data) => {
    output.innerHTML += '<p><strong>' + data.username + '</strong>: ' + data.message + '</p>';
    feedback.innerHTML = '';
    document.querySelector('.chat-message').scrollTop = document.querySelector('.chat-message').scrollHeight;

});

//Displaying if a user is typing
socket.on('typing', (user) => {
    feedback.innerHTML = '<p><em>' + user + ' is typing...</em></p>';
});

//Displaying online users
socket.on('online-users', (data) =>{
    users.innerHTML = ` <p> <p>`;
    data.forEach(user => {
        users.innerHTML += `<p> ${user} </p>`;
    });

});

socket.on('disconnecting', ()=>{
    socket.emit('left',{sock:username,roomname:roomname})

});
socket.on('left',(data)=>{
    output.innerHTML +='<p>--> <strong><em>' + data.sock + ' </strong>has Left the Room</em></p>';
});





