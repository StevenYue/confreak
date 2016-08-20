var Express = require("express");
var Util = require("util");
var BodyParser = require("body-parser");
var Session = require("client-sessions");


var g_confreakdb = require("mongoose");
var g_app = Express();

//connect mongodb
g_confreakdb.connect("localhost:10000/confreakdb");
var g_tbUsers = g_confreakdb.model("users", {name: String, email: String, password: String});


//g_app set up
g_app.set("view engine", "ejs");
g_app.use(BodyParser.json());
g_app.use(BodyParser.urlencoded({extended: true}));
g_app.listen(8000, function (){
    console.log("confreak start listening port 8000");        
});
//end of g_app set up


//requests handlers
g_app.get("/", function(req, res){
    res.render("index");
});

g_app.get("/register", function(req, res){
    res.render("register");
});

g_app.get("/application.ejs", function(req, res){
    res.render("application");
});

g_app.post("/login", function(req, res){
    console.log(req.body);

    res.render("application");
});

//end of requests handlers

//DB accessor
g_tbUsers.findOne({name: "Steve"}, function(err, userObj)
{
    console.log("call back");
    if ( err )
    {
        console.log(err);
    }
    else
    {
        console.log("obj: %j", userObj);
        console.log(Util.inspect(userObj.name, false, null));
    }
});




//End of DB accessor

//subs


//end of subs

