#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime

from flask import Flask, jsonify, request, render_template, make_response
import json
app = Flask(__name__)

@app.route("/api", methods=['GET'])
def sensor_request_api():
    params = request.args
    wetness = params.get('wetness', default='0', type = str)
    temperature = params.get('temperature', default='0', type = str)
    humidity = params.get('humidity', default='0', type = str)
    co2 = params.get('co2', default='0', type = str)
    tvoc = params.get('tvoc', default='0', type = str)

    write(wetness, temperature, humidity, co2, tvoc)
    return 'hoge'###"wetness" + wetness + " temperature:" + temperature + " humidity" + humidity + " co2" + co2 + " tvoc:" + tvoc

@app.route("/", methods=['GET'])
def index():
    client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
    sensordata = client.query("select * from sensordata where time >= now() - 300m")
    # prediction = client.query("select * from prediction where time >= now() - 300m")

    json2list = list(sensordata.get_points(measurement=None))

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
        wetness = sensordata_dict['wetness']
        temperature = sensordata_dict['temperature']
        co2 = sensordata_dict['co2']
        humidity = sensordata_dict['humidity']
        tvoc = sensordata_dict['tvoc']
        time = sensordata_dict['time']

    return render_template('index.html', wetness=wetness, temperature=temperature, humidity=humidity)

# @app.route('/write', methods=['GET'])
def write(wetness, temperature, humidity, co2, tvoc):

    utc_now = datetime.now(timezone('UTC'))
    jst_now = utc_now.astimezone(timezone('Asia/Tokyo'))
    host='influxdb'
    port=8086
    user = 'root'
    password = 'root'
    dbname = 'superdry'
    json_body = [
        {
            "measurement": "sensordata",
            "tags": {
                "user": "umetsu",
                "id": "0"},
            "time": str(jst_now),
            "fields": {
                "wetness": float(wetness),
                "temperature": float(temperature),
                "humidity": float(humidity),
                "co2": float(co2),
                "tvoc": float(tvoc)
            }
        }
    ]

    print("client: ",host, port, user, password, dbname)
    client = InfluxDBClient(host, port, user, password, dbname)
    client.create_database(dbname)
    client.write_points(json_body)

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
