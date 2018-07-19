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

var serial_port = null;

if (config.enable_serial_connection) {
    serial_port = new SerialPort(config.serial_port_name, {
        baudRate: config.serial_baud_rate
    });
}

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


if (serial_port != null) {
    serial_port.on('error', function (err) {
        console.log('Error: ', err.message);
    });
}

if (serial_port != null) {
    serial_port.on('open', function () {
        // open logic
    });
}

if (serial_port != null) {
    serial_port.on('readable', function () {
        console.log('Data:', port.read());
    });
}

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
        state: 1,
        pos_x: 520,
        pos_y: 25,
        visible_name: "HL-Rechts",
        id: 2,
        last_update: null,
        valid_states: ["0", "1"]
    },
    ep_bremsventil: {
        state: 1,
        pos_x: 220,
        pos_y: 150,
        visible_name: "EP-Bremsventil",
        id: 3,
        last_update: null,
        valid_states: ["0", "1"]
    },
    uic_stellung: {
        state: -1,
        pos_x: 345,
        pos_y: 177,
        visible_name: "UIC Stellung",
        id: 4,
        last_update: null,
        valid_states: ["0", "1", "2"]
    },
    bremse_aus_ventile_a: {
        state: 1,
        pos_x: 577,
        pos_y: 132,
        visible_name: "B-Off-A",
        id: 5,
        last_update: null,
        valid_states: ["0", "1"]
    },
    bremse_aus_ventile_b: {
        state: 1,
        pos_x: 577,
        pos_y: 190,
        visible_name: "B-Off-B",
        id: 6,
        last_update: null,
        valid_states: ["0", "1"]
    },
    bremse_aus_ventile_c: {
        state: 0,
        pos_x: 577,
        pos_y: 240,
        visible_name: "B-Off-C",
        id: 7,
        last_update: null,
        valid_states: ["0", "1"]
    },
    steuerungsventil_kurzhub_uic: {
        state: 1,
        pos_x: 290,
        pos_y: 200,
        visible_name: "UIC-Zylinderventil",
        id: 8,
        last_update: null,
        valid_states: ["0", "1"]
    },
    bistabile_magnetventil: {
        state: 1,
        pos_x: 210,
        pos_y: 310,
        visible_name: "Brems-Bistabil",
        id: 9,
        last_update: null,
        valid_states: ["0", "1"]
    },
    relaisventil: {
        state: 1,
        pos_x: 400,
        pos_y: 370,
        visible_name: "Relaisventil",
        id: 10,
        last_update: null,
        valid_states: ["0", "1"]
    },
    r_mode_ventil: {
        state: 0,
        pos_x: 490,
        pos_y: 320,
        visible_name: "R-Mode",
        id: 11,
        last_update: null,
        valid_states: ["0", "1"]
    },
};
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
                if(serial_port != null){
                    var packet_id =uuidv1();
                    serial_port.write("set_auto_" + String(ventil_data[key].id) + "_" + String(ventil_data[key].state) + "_");
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
