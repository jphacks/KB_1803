#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from flask import Flask, jsonify, request
from influxdb import InfluxDBClient
from apscheduler.schedulers.background import BackgroundScheduler
from pytz import timezone
from datetime import datetime
import json
import numpy as np
from sklearn import datasets,svm
from sklearn import metrics

from influxdb import InfluxDBClient

# このアプリ
app = Flask(__name__)

#10/20 10:36 kapacitorから受けるデータの型がわからないから，flaskからgetする処理を毎分行うことにする
def example_digits(nums):

    #scilit-learnライブラリに付属しているデータセット"digits"を利用
    digits = datasets.load_digits()

    #サポートベクターマシン（というもの）を使って、分類.
    # 64個のint配列:正解の数字
    clf = svm.SVC()
    clf.fit(digits.data,digits.target)

    #仮に下記の"test_data"のようなデータは、どの数字に該当するか？を予測
    test = nums
    test_data = [test]

    c1 = "-----テスト----"
    c2 = str(test_data)

    d1 = "-----予測----"
    global pre
    pre = clf.predict(test_data)
    d2 = str(pre)

    text = c1+"<br>"+c2+"<br>"+d1+"<br>"+d2

    return(text)

@app.route("/", methods=['GET'])
def hello():
    # jsonで取ってきて表示
    client = InfluxDBClient(host='influxdb', port=8086, database='prediction')
    result = client.query("select * from results")
    print("Result: {0}".format(result))
    # json形式のデータをリスト化
    data2 = list(result.get_points(measurement=None))
    return str(data2)

@app.route('/reply', methods=['POST'])
def reply():
    data = request.get_json()
    # result = {
    #   "Content-Type": "application/json",
    #   "Answer":{"Text": data}
    # }
    # return jsonify(result)
    
    def write(host='influxdb',port=8086,per=None,time=None):
        # 現在時刻 →　データ取得した時刻
        utc_now = datetime.now(timezone('UTC'))
        jst_now = utc_now.astimezone(timezone('Asia/Tokyo'))
        
        ###Instantiate a connection to the InfluxDB.###
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
                    "dryness": per,
                    "rest_of_time": time,
                    }
            }
        ]

        print("client: ",host, port, user, password, dbname)
        client = InfluxDBClient(host, port, user, password, dbname)
        client.create_database(dbname)
        client.write_points(json_body)

    write(per=0.6, time=6*2)

    return str(data)



if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)