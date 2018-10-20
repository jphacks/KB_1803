#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from sklearn import datasets,svm
from sklearn import metrics

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime

from flask import Flask, jsonify, request
import json
app = Flask(__name__)

@app.route("/", methods=['GET'])
def hello():
    return "Hey! This is http://localhost:5000 or http://flask:5000"

@app.route('/reply', methods=['POST'])
def reply():
    data = request.get_json()
    result = {
      "Content-Type": "application/json",
      "Answer":{"Text": data}
    }
    return jsonify(result)

    # sent = example_digits(num)
    # write(per=pre[0]/10, time=pre[0]*2)
    write(per=0.1, time=10)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)

# 現在時刻
utc_now = datetime.now(timezone('UTC'))
jst_now = utc_now.astimezone(timezone('Asia/Tokyo'))


def example_digits(num):

    #scilit-learnライブラリに付属しているデータセット"digits"を利用
    digits = datasets.load_digits()

    #サポートベクターマシン（というもの）を使って、分類.
    # 64個のint配列:正解の数字
    clf = svm.SVC()
    clf.fit(digits.data,digits.target)

    #仮に下記の"test_data"のようなデータは、どの数字に該当するか？を予測
    test = [num]*64
    test_data = [test]

    c1 = "-----テスト----"
    c2 = str(test_data)

    d1 = "-----予測----"
    global pre
    pre = clf.predict(test_data)
    d2 = str(pre)

    text = c1+"<br>"+c2+"<br>"+d1+"<br>"+d2

    return(text)


def write(host='influxdb',port=8086,per=None,time=None):
    """Instantiate a connection to the InfluxDB."""
    user = 'root'
    password = 'root'
    dbname = 'prediction'
    json_body = [
        {
            "measurement": "results",
            "tags": {
                "user": "umetsu",
                "id": "0"},
            "time": str(jst_now),
            "fields": {
                #"Float_value": 0.64,
                "dryness(%)": per,
                "rest_of_time(min)": time,
                #"Bool_value": True
                }
        }
    ]

    print("client: ",host, port, user, password, dbname)
    client = InfluxDBClient(host, port, user, password, dbname)

    #print("Create database: " + dbname)
    client.create_database(dbname)

    #print("Write points: {0}".format(json_body))
    client.write_points(json_body)

    #print("Querying data: " + query)
    #result = client.query(query)

    #print("Result: {0}".format(result))
