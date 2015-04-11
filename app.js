require('shelljs/global')

var http = require('http');
var fs = require('fs');
var path = require('path');
var exec = require('child_process').exec;
var markdown = require( "markdown" ).markdown;
var async = require('async');
var os = require('os')

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
  		fs.readFile(path.join("problems", problem, "starter.cpp"), 'utf8', function(err, data) {
  			callback(err, data);
		  });
  	},
    function(callback) {
      fs.readFile(path.join("problems", problem, "solution.cpp"), 'utf8', function(err, data) {
        callback(err, data);
      })
    }
  ],
  function(err, results) {
  	socket.emit("problem", {
		  desc: markdown.toHTML(results[0]),
		  starter: results[1],
      solution: results[2]
	   });
  });

  socket.on('run', function(code) {
  	// Hash code to an id
  	
  	fs.writeFile("tmp/code.cpp", code, function(err) {
		// TODO: make cwd platform-independent
  		exec('g++ code.cpp -o code', { cwd: "./tmp" }, function (error, stdout, stderr) {
  			if(fs.existsSync("tmp/code.exe") || fs.existsSync("tmp/code")) {
          async.map(ls('problems/' + problem + '/cases/*.in'), function(file, callback) {
            var casename = path.parse(file).name;
            var result = { name: casename };


            async.parallel([
              function(input) { // Read in input file
                fs.readFile(file, 'utf8', function(err, data) {
                  input(null, data);
                });
              },
              function(expectedoutput) { // Read in output file
                fs.readFile(path.join(path.parse(file).dir, casename + ".out"), 'utf8', function(err, data) {
                  expectedoutput(null, data);
                });
              }
            ], function(err, files) {
              result.input = files[0];
              result.expected = files[1];

              var windows = os.platform().indexOf("win32") != -1 || os.platform().indexOf("win64") != -1;
              var command = windows ? 'code' : './code';

              var child = exec(command, { cwd: "./tmp" }, function (error, stdout, stderr) {
                result.output = stdout;
                result.passed = (result.output.trim() == result.expected.trim());
                callback(null, result);
              });

              child.stdin.write(result.input);
              child.stdin.end();
              
              // TODO: Pipe in input file
            });
          }, function(err, results) {
            socket.emit('results', results);

            // TODO: Remove code executable
          });
  			} else {
  				// TODO: Compile time error
  				console.log("Compile Error: " + stderr);
  			}
  		});
  	});
  });
});
