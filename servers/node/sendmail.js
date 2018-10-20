var unirest = require("unirest");

var req = unirest("POST", "http://163.221.128.127:8000/cgi-bin/cgitest.py/post");

req.headers({
  "Postman-Token": "958ef013-310f-476a-8ed7-c004af8bd538",
  "cache-control": "no-cache",
  "Content-Type": "application/x-www-form-urlencoded",
  "content-type": "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW"
});

req.multipart([
  {
    "body": "7"
  }
]);

req.end(function (res) {
  if (res.error) throw new Error(res.error);

  console.log(res.body);
});