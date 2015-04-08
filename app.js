var http = require('http');
var fs = require('fs');
var path = require('path');
var exec = require('child_process').exec;
var markdown = require( "markdown" ).markdown;
var async = require('async');

if(!fs.existsSync("tmp")) { fs.mkdirSync("tmp") };

var server = http.createServer(function(req, res) {
	fs.readFile('index.html', function (err, html) {
		res.writeHeader(200, {"Content-Type": "text/html"});
		res.write(html);
		res.end();
	});
});

var io = require('socket.io')(server);

server.listen(80, 'localhost', function(){
	console.log("Listening on localhost:80...");
});

var problem = "isbalanced";

io.on('connection', function(socket){
  console.log('A user connected');

  async.parallel([
  	function(callback) {
  		fs.readFile(path.join("problems", problem, "README.md"), 'utf8', function read(err, data) {
  			callback(err, data);
		});
  	},
  	function(callback) {
  		fs.readFile(path.join("problems", problem, "starter.cpp"), 'utf8', function read(err, data) {
  			callback(err, data);
		});
  	}
  ],
  function(err, results) {
  	socket.emit("problem", {
		desc: markdown.toHTML(results[0]),
		starter: results[1]
	});
  });

	

  socket.on('run', function(code) {
  	// Hash code to an id
  	
  	fs.writeFile("tmp/code.cpp", code, function(err) {
  		exec('g++ code.cpp -o code', { cwd: "./tmp" }, function (error, stdout, stderr) {
  			if(fs.existsSync("tmp/code.exe") || fs.existsSync("tmp/code")) {
  				exec('code', { cwd: "./tmp" }, function (error, stdout, stderr) {
  					console.log("Program output: " + stdout);
  					socket.emit('output', stdout);
  				});

  				// TODO: Remove code executable
  			} else {
  				// TODO: Compile time error
  				console.log("Compile Error: " + stderr);
  			}
  		});
  	});
  });
});