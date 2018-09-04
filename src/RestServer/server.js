var express = require('express');
var app = express();
var path = require('path');
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var express = require('express');
var session = require('express-session');
var bodyParser = require('body-parser');
var sessionstore = require('sessionstore');
var os = require("os");
var chalk = require('chalk');
var mqtt = require('mqtt');
var config = require('./config.json');
var uuidv1 = require('uuid/v1');
var got = require('got');

var fs = require('fs');
var SerialPort = require('serialport');
const Readline = SerialPort.parsers.Readline;
var serial_port = null;


serial_port = new SerialPort('/dev/ttyAMA0', {
        baudRate: 9600,

        autoOpen: false,
        flowControl: false
    });

var VERSION =0.2;

var port = process.env.PORT || config.webserver_default_port || 3000;

//----------------------------- EXPRESS APP SETUP ------------------------------------------//
app.set('trust proxy', 1);
app.use(function (req, res, next) {
    if (!req.session) {
        return next(); //handle error
    }
    next(); //otherwise continue
});
app.set('views', __dirname + '/views');
app.engine('html', require('ejs').renderFile);
// Routing
app.use(express.static(path.join(__dirname, 'public')));
app.use(session({
    secret: 'ssshhfddghjhgewretjrdhfsgdfadvsgvshthhh',
    store: sessionstore.createSessionStore(),
    resave: true,
    saveUninitialized: true
}));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: true
}));

app.use(function (req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

const parser = serial_port.pipe(new Readline({ delimiter: '\r\n' }));
parser.on('data', console.log);


    serial_port.on('error', function (err) {
        console.log('Error: ', err.message);
        io.emit('err', { msg: String(err.message), silcence: true });
    });

serial_port.open(function (err) {
    if (err) {
        return console.log('Error opening port: ', err.message);
    }

    // Because there's no callback to write, write errors will be emitted on the port:
    serial_port.write('RESET___');
    io.emit('err', { msg: "SERIAL PORT OPEN", silcence: true });
});


// The open event is always emitted
serial_port.on('open', function () {
    // open logicg
});



 //   serial_port.on('readable', function () {
   //     console.log('Data:', serial_port.read());
//});

  // this.port.on('data', function (data) {
    //console.log('Data1:', data, data.toString('utf8'));
    //});
serial_port.on('error', function (err) {
    console.log('Error: ', err.message);
});



var ventil_data = {
    hl_ventil_links: {
        state: 0,
        pos_x: 305,
        pos_y: 25,
        visible_name: "HL-Links",
        id: 1,
        last_update: null,
        valid_states: ["0", "1"]

    },
    hl_ventil_rechts: {
        state: 0,
        pos_x: 520,
        pos_y: 25,
        visible_name: "HL-Rechts",
        id: 2,
        last_update: null,
        valid_states: ["0", "1"]
    },
    ep_bremsventil: {
        state: 0,
        pos_x: 220,
        pos_y: 150,
        visible_name: "EP-Bremsventil",
        id: 3,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    uic_stellung: {
        state: 0,
        pos_x: 345,
        pos_y: 177,
        visible_name: "UIC Stellung",
        id: 4,
        last_update: null,
        valid_states: ["0", "1", "2"],
        is_uic: true
    },
    bremse_aus_ventile_a: {
        state: 0,
        pos_x: 577,
        pos_y: 132,
        visible_name: "B-Off-A",
        id: 5,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    bremse_aus_ventile_b: {
        state: 0,
        pos_x: 577,
        pos_y: 190,
        visible_name: "B-Off-B",
        id: 6,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    bremse_aus_ventile_c: {
        state: 0,
        pos_x: 577,
        pos_y: 240,
        visible_name: "B-Off-C",
        id: 7,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    steuerungsventil_kurzhub_uic: {
        state: 0,
        pos_x: 290,
        pos_y: 200,
        visible_name: "UIC-Zylinderventil",
        id: 8,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    bistabile_magnetventil: {
        state: 0,
        pos_x: 210,
        pos_y: 310,
        visible_name: "Brems-Bistabil",
        id: 9,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    relaisventil: {
        state: 0,
        pos_x: 400,
        pos_y: 370,
        visible_name: "Relaisventil",
        id: 10,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
    r_mode_ventil: {
        state: 0,
        pos_x: 490,
        pos_y: 320,
        visible_name: "R-Mode",
        id: 11,
        last_update: null,
        valid_states: ["0", "1"],
        is_uic: false
    },
};

var ventil_data_init = ventil_data;
var last_update = Math.round(new Date().getTime() / 1000);


app.get('/', function (req, res) {
    sess = req.session;
    res.render('index.ejs', {
        vent_init_data: ventil_data
    });
});





io.on('connection', (socket) => {

});
//TODO BUTTONS FÜR MANUELL SC_HALTEN EINBAUEN

app.get('/rest/:vent_id/set_state/:state', function (req, res) {

    var vent_id = req.params.vent_id;
    var state = req.params.state;

    console.log(vent_id + " " + state);

    var vent_id = req.params.vent_id;
    for (var key in ventil_data) {

        if (!ventil_data.hasOwnProperty(key)) {
            continue;
        }
        if (ventil_data[key].id == vent_id) {
            var is_in = false;
            for (let index = 0; index < ventil_data[key].valid_states.length; index++) {
                const element = ventil_data[key].valid_states[index];
                if (element == state) {
                    is_in = true;
                    break;
                }
            }

            if (is_in) {
                ventil_data[key].state = state;
                ventil_data[key].last_update = Math.round(new Date().getTime() / 1000);
                last_update = Math.round(new Date().getTime() / 1000);

                res.json({
                    state: ventil_data[key].state,
                    info: ventil_data[key],
                    err: "0",
                    err_text: "OK"
                });

                io.emit('broadcast', ventil_data); // short form


                //SEND NEW STATE TO SERIAL
                
                    var packet_id =uuidv1();
                    if (ventil_data[key].is_uic){
                        console.log("SETUIC_" + String(ventil_data[key].id) + "_" + String(ventil_data[key].state) + "_\r\n");
                        serial_port.write("SETUIC_" + String(ventil_data[key].id) + "_" + String(ventil_data[key].state) + "_\r\n");
                    }else{
                        console.log("SET_" + String(ventil_data[key].id) + "_" + String(ventil_data[key].state) + "_\r\n");
                        serial_port.write("SET_" + String(ventil_data[key].id) + "_" + String(ventil_data[key].state) + "_\r\n");
                    }
                    
                

                return;
            } else {
                res.json({
                    state: ventil_data[key].state,
                    info: ventil_data[key],
                    err: "2",
                    err_text: "NEW STATE NOT ACCEPTED see valid_states"
                });
                return;
            }
            break;
        }
    }





    res.json({
        state: ventil_data[key].state,
        err: "1",
        err_text: "ID NOT FOUND"
    });
});


function RESET_ALL(){
    ventil_data = ventil_data_init;
    last_update = Math.round(new Date().getTime() / 1000);
    //TODO UPDATE TIMESTAMP FROM ALL VENTILES

    console.log("RESET___");
    serial_port.write("RESET___");

    for (let index = 0; index < ventil_data_init.length; index++) {
        const element = ventil_data_init[index];
        
            var packet_id = uuidv1();
            if (element.is_uic) {
              //  console.log("SETUIC_" + String(element.id) + "_" + String(element.state) + "_\r\n");
        //        serial_port.write("SETUIC_" + String(element.id) + "_" + String(element.state) + "_\r\n");
            } else {
            //    console.log("SET_" + String(element.id) + "_" + String(element.state) + "_\r\n");
          //      serial_port.write("SET_" + String(element.id) + "_" + String(element.state) + "_\r\n");
        }
    }
}
RESET_ALL(); //AUSGANGSZUSTAND




app.get('/reset', function (req, res) {
 
    RESET_ALL();
    io.emit('err', { msg: "SYSTEM RESET VIA REST API", silcence:true});
    res.json({
        ventile: ventil_data,
        last_update: last_update,
        reset:true
    });
});



app.get('/rest/:vent_id/get_state', function (req, res) {

    var vent_id = req.params.vent_id;
    for (var key in ventil_data) {

        if (!ventil_data.hasOwnProperty(key)) {
            continue;
        }
        if (ventil_data[key].id == vent_id) {
            res.json({
                state: ventil_data[key].state,
                err: "0",
                err_text: "ok"
            });
            return;
        }
    }
    res.json({
        state: null,
        err: "1",
        err_text: "id not found"
    });
});

app.get('/rest/:vent_id/get_info', function (req, res) {

    var vent_id = req.params.vent_id;
    for (var key in ventil_data) {

        if (!ventil_data.hasOwnProperty(key)) {
            continue;
        }
        if (ventil_data[key].id == vent_id) {
            res.json({
                info: ventil_data[key],
                err: "0",
                err_text: "ok"
            });
            return;
        }
    }
    res.json({
        info: null,
        err: "1",
        err_text: "id not found"
    });
});

app.get('/rest/ventile', function (req, res) {
    res.json({
        ventile: ventil_data,
        last_update: last_update
    });
});
// CREATE PART ICON LIST
