var Express = require("express");
var Util = require("util");
var BodyParser = require("body-parser");
var Session = require("client-sessions");


var g_confreakdb = require("mongoose");
var g_app = Express();

//connect mongodb
g_confreakdb.connect("localhost:10000/confreakdb");
var UserModel = g_confreakdb.model("users", {name: String, email: String, password: String});

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
    var user = createUser(req.body.email, '', '');
    findUserModel(user, function(re){

        if ( re.errno )
        {
            console.log("login error");
        }
        else
        {
            if ( req.body.password === re.obj.password )
            {
                res.render("application", {user: re.obj});
            }
            else
            {
                res.render("index", {errno: -1});
            }
        }
    });
});

g_app.post("/create", function(req, res){
    var user = createUser(req.body.email, req.body.password, req.body.userName);
    var re = findUserModel(user, returnCallBack);
    if ( !re )
    {
        insertUserModel(user);
        res.render("application", {user: user});
    }
    else
    {
       console.log("Creatation error"); 
    }

});

//end of requests handlers



//subs

function createUser(email, password, name)
{
    return {name: name, email: email, password: password};
}

function insertUserModel(user)
{
    var userModel = new UserModel(user);
    userModel.save(function (err, userObj){
        if (err)
        {
            console.log(err);
        }
        else
        {
            console.log('saved successfully: %j', userObj); 
        }
    });
}

function findUserModel(user, fn)
{
    UserModel.findOne({email: user.email}, function(err, userObj){
        if ( err )
        {
            console.log(err);
        }
        else
        {
            console.log("User obj found: %j", userObj);
        }
        fn({errno: err, obj: userObj});
    });
}

function returnCallBack(obj)
{
    return obj;
}

//end of subs

