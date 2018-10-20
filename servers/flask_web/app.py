#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime

from flask import Flask, jsonify, request
import json
app = Flask(__name__)

@app.route("/", methods=['GET'])
def hello():
    client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
    prediction = client.query("select * from prediction")
    # sensordata = client.query("select * from sensordata")
    print("prediction: {0}".format(prediction))
    return "prediction: {0}".format(prediction)

@app.route('/reply', methods=['GET'])
def reply():

# 現在時刻
    utc_now = datetime.now(timezone('UTC'))
    jst_now = utc_now.astimezone(timezone('Asia/Tokyo'))
    def write(host='influxdb',port=8086,per=None,time=None):
        # """Instantiate a connection to the InfluxDB."""
        user = 'root'
        password = 'root'
        dbname = 'superdry'
        json_body = [
            {
                "measurement": "prediction",
                "tags": {
                    "user": "umetsu",
                    "id": "0"},
                "time": str(jst_now),
                "fields": {
                    "dryness": per,
                    "rest_of_time": time,
                }
            }
        ]

        print("client: ",host, port, user, password, dbname)
        client = InfluxDBClient(host, port, user, password, dbname)
        client.create_database(dbname)
        client.write_points(json_body)

    write(per=0.4, time=60)


    return "write"
    # answer = "Yes, it is %s!\n" % data["keyword"]
    # result = {
    #   "Content-Type": "application/json",
    #   "Answer":{"Text": data}
    # }
    # return answer
    # return jsonify(result)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=6500, debug=True)


#
#
# def example_digits(num):
#
#     #scilit-learnライブラリに付属しているデータセット"digits"を利用
#     digits = datasets.load_digits()
#
#     #サポートベクターマシン（というもの）を使って、分類.
#     # 64個のint配列:正解の数字
#     clf = svm.SVC()
#     clf.fit(digits.data,digits.target)
#
#     #仮に下記の"test_data"のようなデータは、どの数字に該当するか？を予測
#     test = [num]*64
#     test_data = [test]
#
#     c1 = "-----テスト----"
#     c2 = str(test_data)
#
#     d1 = "-----予測----"
#     global pre
#     pre = clf.predict(test_data)
#     d2 = str(pre)
#
#     text = c1+"<br>"+c2+"<br>"+d1+"<br>"+d2
#
#     return(text)
#
#
