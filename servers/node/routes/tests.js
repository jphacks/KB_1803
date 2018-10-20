var express = require('express');
var router = express.Router();

var Test = require('../models/test');

/* 送信されたデータを保存する */
router.post('/', function(req, res, next) {
    let series = req.body.series;
    for (let s of series) {
        var test = new Test();
        test.time = new Date(s.values[0][0]);
        test.host = s.tags.host;
        test.location = s.tags.location;
        test.value = s.values[0][1];
        console.log(test);

        test.save(function(err) {
            if (err) res.send(err);
        });
    }
    res.send(req.body);
});

/* 最新の5件を返す */
router.get('/latest', function(req, res, next) {
    Test.find(null, null, { sort:{time: -1}, limit: 10 }, function(err, tests) {
        if (err) res.send(err);
        res.json(tests);
    });
});

module.exports = router;
