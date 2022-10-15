const express = require('express');
const app=express();
const bodyParser = require('body-parser');
const socket = require('socket.io');


app.use(bodyParser.urlencoded({extended:false}));// to parse the url req  body

app.use(express.static('public'));// setting the default root to public folder
app.set('view engine','ejs');//setting the template engine

app.get('/',(req,res)=>{
    res.render('index');// render the index page
});

//get username and roomnmae from form and pass it to room
app.post('/room',(req,res)=>{
    roomname=req.body.roomname;
    username=req.body.username;
    console.log("POST request");
    res.redirect(`/room?username=${username}&roomname=${roomname}`);
});

//Rooms
app.get('/room',(req,res)=>{
    console.log('get req for /room');
    res.render('room');
    
});

//Start Server
const server = app.listen(3000, () => {
    console.log(`Server Running on 3000`);
});

const io = socket(server);
require('./utils/socket')(io);