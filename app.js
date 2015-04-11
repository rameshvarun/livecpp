require('shelljs/global')

var http = require('http');
var fs = require('fs');
var path = require('path');
var exec = require('child_process').exec;
var markdown = require("markdown").markdown;
var async = require('async');
var os = require('os')


var crypto = require('crypto');

if (!fs.existsSync("tmp")) {
	fs.mkdirSync("tmp");
}

var server = http.createServer(function(req, res) {
	fs.readFile('index.html', function(err, html) {
		res.writeHeader(200, {
			"Content-Type": "text/html"
		});
		res.write(html);
		res.end();
	});
});

var io = require('socket.io')(server);

server.listen(80, 'localhost', function() {
	console.log("Listening on localhost:80...");
});

var problem = "isbalanced";

io.on('connection', function(socket) {
	console.log('A user connected');

	// Load README.md, starter.cpp, and solution.cpp in parallel
	async.parallel([function(callback) {
		fs.readFile(path.join("problems", problem, "README.md"), 'utf8', function read(err, data) {
			callback(err, data);
		});
	}, function(callback) {
		fs.readFile(path.join("problems", problem, "starter.cpp"), 'utf8', function(err, data) {
			callback(err, data);
		});
	}, function(callback) {
		fs.readFile(path.join("problems", problem, "solution.cpp"), 'utf8', function(err, data) {
			callback(err, data);
		})
	}
	], function(err, results) {
		// Send files back to client
		socket.emit("problem", {
			desc: markdown.toHTML(results[0]),
			starter: results[1],
			solution: results[2]
		});
	});

	socket.on('run', function(code) {
		// Hash code to an id
		var hash = crypto.createHash('md5').update(code).digest('hex');
		var code_path = path.join("tmp", hash + ".cpp");
		var exec_path = path.join("tmp", hash);

		fs.writeFile(code_path, code, function(err) {
			// Compile executable
			exec('g++ ' + hash + '.cpp -o ' + hash, {
				cwd: "./tmp"
			}, function(error, stdout, stderr) {
				if (fs.existsSync(exec_path + ".exe") || fs.existsSync(exec_path)) {
					async.map(ls('problems/' + problem + '/cases/*.in'), function(file, callback) {
						var casename = path.parse(file).name;
						var result = {
							name: casename
						};

						async.parallel([function(input) { // Read in input file
							fs.readFile(file, 'utf8', function(err, data) {
								input(null, data);
							});
						}, function(expectedoutput) { // Read in output file
							fs.readFile(path.join(path.parse(file).dir, casename + ".out"), 'utf8', function(err, data) {
								expectedoutput(null, data);
							});
						}
						], function(err, files) {
							result.input = files[0];
							result.expected = files[1];

							// Platform independent exec
							var windows = os.platform().indexOf("win32") != -1 || os.platform().indexOf("win64") != -1;
							var command = windows ? hash : './' + hash;

							var start_time = new Date().getTime();
							var child = exec(command, {
								cwd: "./tmp"
							}, function(error, stdout, stderr) {
								result.output = stdout;
								result.passed = (result.output.trim() == result.expected.trim());
								result.time = new Date().getTime() - start_time;

								callback(null, result);
							});

							// Pipe input in to stdin
							child.stdin.write(result.input);
							child.stdin.end();
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
