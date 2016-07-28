var http = require("http");
var url = require("url");

var postServer = http.createServer((req, res) => {
    var uri = url.parse(req.url, true);
    var method = uri.query["method"];
    var host = "10.0.41.138";

    console.log("Forward: "+method+" "+host+uri.pathname);

    var preq = http.request({
        "host": host,
        "path": uri.pathname,
        method: method
    }, (pres) => {
        pres.pipe(res);
        pres.pipe(process.stdout);
        pres.on("close",() => { res.end(); });
    });
    if (uri.query["body"])
    {
        console.log("Body data: "+uri.query["body"]);
        preq.write(uri.query["body"]);
    }

    preq.end();
});

console.log("Server started...");
postServer.listen(80);
