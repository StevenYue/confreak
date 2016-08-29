var Express = require("express");
var Util = require("util");
var BodyParser = require("body-parser");
var Session = require("client-sessions");


var g_confreakdb = require("mongoose");
var g_app = Express();


//connect mongodb
g_confreakdb.connect("localhost:10000/confreakdb");
var UserModel = g_confreakdb.model("freak", {name: String, email: String, password: String});

//g_app set up
g_app.set("view engine", "ejs");
g_app.use(BodyParser.json());
g_app.use(BodyParser.urlencoded({extended: true}));
g_app.use(Session({
  cookieName: 'session',
  secret: 'dhgaugahnguroqarnaf',
  duration: 30 * 60 * 1000,
  activeDuration: 5 * 60 * 1000,
}));
g_app.listen(8000, function (){
    console.log("confreak start listening port 8000");        
});

//end of g_app set up

//Constants
const LOGIN_ERROR_EMAIL_NOT_FOUND = -1;
const LOGIN_ERROR_WRONG_PASSWORD = -2;

const CHECK_AVAILABILITY_OK = 0;
const CHECK_AVAILABILITY_BAD = -1;

//end of Constants


//requests handlers
g_app.get("/", function(req, res){
    res.render("index.ejs");
});

g_app.get("/register.ejs", function(req, res){
    res.render("register.ejs");
});

g_app.get("/application.ejs", function(req, res){
    res.render("application.ejs");
});

g_app.post("/login", function(req, res){
    console.log("Login Request:", req.body);
    var user = createUser(req.body.email, '', '');
    findUserModel(user, function(re){
        if ( re.errno )
        {
            console.log("login error");
        }
        else
        {
            if ( re.obj )
            {
                if ( req.body.password === re.obj.password )
                {
                    req.session.email = req.body.email;
                    req.session.password = req.body.password;
                    res.redirect("/application.ejs?name=" + re.obj.name);
                }
                else
                {
                    res.redirect("/?errno=" + LOGIN_ERROR_WRONG_PASSWORD);
                }
            }
            else
            {
                res.redirect("/?errno=" + LOGIN_ERROR_EMAIL_NOT_FOUND);
            }
        }
    });
});

g_app.post("/create", function(req, res){
    console.log("Create Freak Request:", req.body);
    var user = createUser(req.body.email, req.body.password, req.body.userName);
    findUserModel(user, function(re){
        if ( re.errno )
        {
            console.log("Creatation error"); 
        }
        else
        {
            if ( !re.obj )
            {
                insertUserModel(user);
                req.session.email = req.body.email;
                req.session.password = req.body.password;
                res.redirect("/application.ejs?name=" + req.body.userName);
            }
            else
            {
                console.log("User %j existed aleady, creatation failed", user);
            }
        }
    });
});

g_app.post("/checkAvailablity", function(req, res){
    console.log("checkAvailablity Request:", req.body);
    var user = createUser(req.body.email, '', '');
    findUserModel(user, function(re){
        if ( re.errno )
        {
            console.log("checkAvailablity error");
        }
        else
        {
            var checkResult = {errno: CHECK_AVAILABILITY_OK};
            if ( re.obj )
            {
                checkResult.errno = CHECK_AVAILABILITY_BAD;   
            }
            res.send(checkResult);
        }
    });   
});

g_app.post("/dummy", function(req, res){
    console.log("dummy Request:", req.body)
    var user = createUser(req.session.email, '', '');
    findUserModel(user, function(re){
        if ( re.errno )
        {
            console.log("sessionVerify error");
        }
        else
        {
            if ( re.obj )
            {
                if ( re.obj.password === req.session.password )
                {
                    console.log("sessionVerify passed");
                }
                else
                {
                    console.log("sessionVerify failed, wrong password");
                    res.redirect("/index");
                }   
            }
            else
            {
                console.log("sessionVerify failed, not a valid user");
                res.send({a:1});
                console.log("fuck");
            }
            
        }
    });   

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
            console.log("Freak obj found: %j", userObj);
        }
        fn({errno: err, obj: userObj});
    });
}

//end of subs

