var Express = require("express");
var Util = require("util");
var BodyParser = require("body-parser");
var Session = require("client-sessions");
var WebSocketServer = require('websocket').server;
var Http = require('http');
var server = Http.createServer(function(request, response){}).listen(1337, function() {});
var g_store = require("memory-store");

var g_confreakdb = require("mongoose");
var g_app = Express();
var g_login = {};

// create the server
g_wsServer = new WebSocketServer({
    httpServer: server
});

// WebSocket server
g_wsServer.on('request', function(req) {
    var email = req.resourceURL.query.email;
    console.log(email);
    var connection = req.accept(null, req.origin);

    var interval = setInterval(function(){
        var args = {
            Email : email
        };
        findAppModel(args, function(resApp){
            /*
            for ( var i in resApp.obj )
            {
                var app = resApp.obj[i];
                if ( app.AppType === APP_TYPE_MAP["Monitor Application"] )
                {
                    app.NumericData = Math.random();
                }
            }
            */
            connection.sendUTF(JSON.stringify(resApp.obj));
        });
    }, 2000); //2 sec
    connection.interval = interval;
    // This is the most important callback for us, we'll handle
    // all messages from users here.
    connection.on('message', function(message) {
        console.log("Oh yes: " + message);
        if (message.type === 'utf8') {
            // process WebSocket message
        }
    });

    connection.on('close', function(conn) {
        // close user connection
        console.log("WebSocket closed", conn);
        clearInterval(this.interval);
    });
});


//connect mongodb
g_confreakdb.connect("localhost:10000/confreakdb");
var UserModel           = g_confreakdb.model("Freak", {name: String, email: String, password: String});
var ApplicationModel    = g_confreakdb.model("ApplicationModel", {AppName: String, AppDesc: String, 
    AppType: Number, BoolData: Boolean, NumericData: Number, Email: String  });

//g_app set up
g_app.set("view engine", "ejs");
g_app.use(BodyParser.json());
g_app.use(BodyParser.urlencoded({extended: true}));
g_app.use(Session({
  cookieName: 'session',
  secret: 'dhgaugahnguroqarnaf',
  duration: 30 * 60 * 1000,
  activeDuration: 5 * 60 * 1000,
  key : "sid"
}));
g_app.listen(8000, function (){
    console.log("confreak start listening port 8000");        
});

//end of g_app set up

//Constants
const LOGIN_ERROR_EMAIL_NOT_FOUND = -1;
const LOGIN_ERROR_WRONG_PASSWORD = -2;

const CHECK_AVAILABILITY_GOOD = 0;
const CHECK_AVAILABILITY_BAD = -1;

const SESSION_VERIFY_GOOD = 0;
const SESSION_VERIFY_BAD  = -1;

const HTTP_GET_INVALID_ARGS = -3;
const HTTP_GET_WRONG_PASSWORD = -4;

const APP_TYPE_MAP = {
    "Control Application" : 0,
    "Monitor Application" : 1,
    0 : "Control Application",
    1 : "Monitor Application"
};

//end of Constants


//requests handlers
g_app.get("/", function(req, res){
    res.render("index.ejs");
});

g_app.get("/register.ejs", function(req, res){
    res.render("register.ejs");
});

g_app.get("/application.ejs", function(req, res){
    if ( sessionVerify(req, res) === SESSION_VERIFY_GOOD )
    {
        res.render("application.ejs");
    }
    else
    {
        res.render("index.ejs");
    }
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
                    g_login[req.body.email] = req.body.password; 
                    res.redirect("/application.ejs?name=" + re.obj.name + "&email=" + req.body.email);
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
                g_login[req.body.email] = req.body.password; 
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
            var checkResult = {errno: CHECK_AVAILABILITY_GOOD};
            if ( re.obj )
            {
                checkResult.errno = CHECK_AVAILABILITY_BAD;   
            }
            res.send(checkResult);
        }
    });   
});

g_app.post("/createApp", function(req, res){
    console.log("createApp Request: ", req.body);
    if ( sessionVerify(req, res) === SESSION_VERIFY_GOOD )
    {
        var email = req.session.email;
        var args = {
            Email : email,
            AppName : req.body.AppName,
            AppType : req.body.AppType
        };
        findAppModel(args, function(resApp){
            if ( resApp.obj && resApp.obj.length > 0 )
            {
                var checkResult = {errno: CHECK_AVAILABILITY_BAD};
                res.send(checkResult);
            }
            else
            {
                var app = createApp(req.body.AppName, req.body.AppDesc, req.body.AppType, false, 0, email);
                insertAppModel(app);
                var checkResult = {errno: CHECK_AVAILABILITY_GOOD};
                res.send(checkResult);
            }
        });
    }
});

g_app.post("/loadAllApplications", function(req, res){
    console.log("loadAllApplications Request: ", req.body);
    if ( sessionVerify(req, res) === SESSION_VERIFY_GOOD )
    {
        var args = {
            Email : req.session.email
        };
        findAppModel(args, function(resApp){
            res.send(resApp.obj);
        });
    }
});

g_app.post("/deleteApp", function(req, res){
    console.log("deleteApp Request: ", req.body);
    if ( sessionVerify(req, res) === SESSION_VERIFY_GOOD )
    {
        var args = {AppName : req.body.AppName, AppType : req.body.AppType};
        removeAppModel(args, function(resApp){
            res.send(resApp);
        });
    }
});

g_app.get("/updateAppData", function(req, res){
    console.log("updateAppData request: ", req.query);
    var inf = req.query;
    var data = Number(inf.data);
    var appType = Number(inf.apptype);
    if ( inf.appname && appType === APP_TYPE_MAP["Monitor Application"] && inf.email && inf.password && inf.data )
    {
        var user = {email : inf.email };
        findUserModel(user, function(userModel){
            if ( userModel.obj.password === inf.password )
            {
                var appRef = {AppName : inf.appname, AppType : appType, Email : inf.email};
                //var update = {AppName : inf.appname, AppType : appType, Email : inf.email, NumericData : data};
                var update = {$set: {NumericData: data}};
                updateAppModel(appRef, update);
                res.send({errno: 0});
            }
            else
            {
                res.send({errno: HTTP_GET_WRONG_PASSWORD});
            }
        });
    }
    else
    {
        res.send({errno: HTTP_GET_INVALID_ARGS});
    }
});

g_app.get("/loadAllApplications", function(req, res){
    console.log("loadAllApplications get Request: ", req.query);
    var inf = req.query;
    if ( inf.email && inf.password )
    {
        var user = { email : inf.email};
        findUserModel(user, function(userModel){
            if ( userModel.obj.password === inf.password )
            {
                var args = { Email : inf.email };
                findAppModel(args, function(resApp){
                    var allApps = {}; 
                    for ( var i in resApp.obj )
                    {
                        var row = resApp.obj[i];
                        allApps[row.AppName + "-" + row.AppType] = row;
                    }
                    res.send(allApps);
                });
            }
            else
            {
                res.send({errno: HTTP_GET_WRONG_PASSWORD});
            }
        })
    }
    else
    {
        res.send({errno : HTTP_GET_INVALID_ARGS});
    }
});


g_app.get("/off", function(req, res){
    req.session.destroy(function(err){
        console.log("Session destroy error");
    });
    console.log("log off finished");
    res.render("index.ejs");
});

g_app.post("/dummy", function(req, res){
    console.log("dummy Request:", req.body)
    if ( !sessionVerify(req, res) )
    {

    }
});

//end of requests handlers



//subs

function createUser(email, password, name)
{
    return {name: name, email: email, password: password};
}

function createApp(appName, appDesc, appType, boolData, numericData, email)
{
    return {AppName: appName, AppDesc: appDesc, AppType: appType, BoolData: boolData, NumericData: numericData, Email: email};
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

function insertAppModel(app)
{
    var appModel = new ApplicationModel(app);
    appModel.save(function (err, userObj){
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

function findAppModel(args, func)
{
    ApplicationModel.find(args, function(err, obj){
        if ( err )
        {
            console.log(err);
        }
        else
        {
            console.log("App Obj found: %j", obj);
        }
        func({errno: err, obj: obj});
    });
}

function updateAppModel(ref, update)
{
    console.log("Ref: %j", ref);
    console.log("New: %j", update);
    ApplicationModel.update(ref, update, function(err, item){
        console.log(err);
        console.log("%j", item);
    });
}

function removeAppModel(args, func)
{
    ApplicationModel.remove(args, function(err, obj){
        if ( err )
        {
            console.log(err);
        }
        else
        {
            console.log(args + " removed successfully");
        }
        func({errno: err});
    });
}

function sessionVerify(req, res) 
{
    if ( req.session.email && req.session.password && g_login[req.session.email] === req.session.password )
    {

        console.log("sessionVerify passed");
        return SESSION_VERIFY_GOOD;
    } 
    else
    {
        console.log("Invalid session obj");
        return SESSION_VERIFY_BAD;
    }
}

//end of subs

