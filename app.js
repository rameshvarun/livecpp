require('shelljs/global')

var fs = require('fs');
var path = require('path');
var exec = require('child_process').exec;
var markdown = require("markdown").markdown;
var async = require('async');
var os = require('os');
var crypto = require('crypto');
var express = require('express');
var nunjucks = require('nunjucks');

if (!fs.existsSync("tmp")) {
	fs.mkdirSync("tmp");
}

var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

nunjucks.configure('templates', {
    autoescape: true,
    express: app,
    watch: true,
    tags: {
    	variableStart: '[[',
    	variableEnd: ']]',
    	blockStart: '[%',
    	blockEnd: '%]',
    	commentStart: '<#',
    	commentEnd: '#>'
    }
});

app.use(express.static('public'));
app.get('/:problem', function(req, res){
	res.render('index.html', {
		problem: req.params.problem
	});
});
app.get('/', function(req, res){
	res.render('list.html', {problems: ls('problems/')});
})

server.listen(80, 'localhost', function() {
	console.log("Listening on localhost:80...");
});

io.on('connection', function(socket) {
	console.log('A user connected');

	socket.on('get_problem', function(problem){
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
	});
	
	socket.on('run', function(obj) {

		var code = obj.code;
		var problem = obj.problem;
		// Hash code to an id
		var hash = crypto.createHash('md5').update(code).digest('hex');
		var code_path = path.join("tmp", hash + ".cpp");
		var exec_path = path.join("tmp", hash);

		fs.writeFile(code_path, code, function(err) {
			// Compile executable
			exec('g++ -std=c++11 ' + hash + '.cpp -o ' + hash, {
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
						}, function(extra) { // Read in the extra input file (for comments, etc.)
							fs.readFile(path.join(path.parse(file).dir, casename + ".in.extra"), 'utf8', function(err, data) {
								extra(null, data);
							});
						}, function(expectedoutput) { // Read in output file
							fs.readFile(path.join(path.parse(file).dir, casename + ".out"), 'utf8', function(err, data) {
								expectedoutput(null, data);
							});
						}
						], function(err, files) {

							result.input = files[0];
							if(files.length === 3){
								result.extra = files[1];
								result.expected = files[2];
							}else{
								result.expected = files[1];
							}

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

							child.stdin.write(result.input);
							child.stdin.end();
						});
					}, function(err, results) {
						socket.emit('results', results);
					});
				} else {
          socket.emit("compile_error", stderr);
				}
			});
		});
	});
});
