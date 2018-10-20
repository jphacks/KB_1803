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

# "/"の時のアプリ
@app.route("/", methods=['GET'])
def hello():
    # jsonで取ってくる
    client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
    # 最新のminutes分のデータを取り出す
    minutes = 60
    result = client.query("select * from sensordata where time >= now() - %sm" % str(minutes))    
    # json形式のデータをリスト形式に変更
    json2list = list(result.get_points(measurement=None))

    # 要素の有無
    if len(json2list) == 0:
        return str("過去%s分間のデータがありません" % str(minutes))

    else:
        # リストから最新の要素を取り出す
        sensordata_dict = json2list[-1]
        # キー
        keys = list(sensordata_dict.keys())
        # 値
        values = list(sensordata_dict.values())

        # 取ってきた値を変数に代入()
        Wet = sensordata_dict['wetness']
        Temp = sensordata_dict['temperature']
        Co2 = sensordata_dict['co2']
        Hum = sensordata_dict['humidity']
        Tvoc = sensordata_dict['tvoc']
        Time = sensordata_dict['time']
        # User = sensordata_dict['user']
        # Id = sensordata_dict['id']

        return str(keys)+str(values)

#"/reply"の時のアプリ（superdry.autogenにデータが書き込まれるごとに呼び出される）
@app.route('/reply', methods=['POST'])
def reply():
    """HttpOutで与えられたデータを実質使えていないので，できれば使いたいけど後回しで
    data = request.get_json()
    # result = {
    #   "Content-Type": "application/json",
    #   "Answer":{"Text": data}
    # }
    # return jsonify(result)
    """
    
    # Influxdbからデータを取得する関数
    def readDB():
        # jsonで取ってくる
        client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
        # 最新のminutes分のデータを取り出す
        minutes = 10
        result = client.query("select * from sensordata where time >= now() - %sm" % str(minutes))    
        # json形式のデータをリスト形式に変更
        json2list = list(result.get_points(measurement=None))

        # 要素の有無
        if len(json2list) == 0:
            #return str("過去%s分間のデータがありません" % str(minutes))
            datalist = [0,0,0,0,0,0]

        else:
            # リストから最新の要素を取り出す
            sensordata_dict = json2list[-1]
            # キー
            keys = list(sensordata_dict.keys())
            # 値
            values = list(sensordata_dict.values())

            # 取ってきた値を変数に代入()
            Wet = sensordata_dict['wetness']
            Temp = sensordata_dict['temperature']
            Co2 = sensordata_dict['co2']
            Hum = sensordata_dict['humidity']
            Tvoc = sensordata_dict['tvoc']
            Time = sensordata_dict['time']
            # User = sensordata_dict['user']
            # Id = sensordata_dict['id']

            datalist = [Wet,Temp,Co2,Hum,Tvoc,Time]
        
        return datalist

    # 機械学習の結果を返す関数
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


    # Influxdbにデータを書き込む関数
    def write(host='influxdb',port=8086,dryness=None,rest_of_time=None):
        # 現在時刻 →　データ取得した時刻
        utc_now = datetime.now(timezone('UTC'))
        jst_now = utc_now.astimezone(timezone('Asia/Tokyo'))
        
        ###Instantiate a connection to the InfluxDB.###
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
                    "dryness": dryness,
                    "rest_of_time": rest_of_time,
                    }
            }
        ]

        print("client: ",host, port, user, password, dbname)
        client = InfluxDBClient(host, port, user, password, dbname)
        client.create_database(dbname)

        client.write_points(json_body)

    datalist = readDB()    

    write(dryness=datalist[0], rest_of_time=datalist[1])
    #write(dryness=100.0, rest_of_time=200.0)
    #return str(data)



if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)