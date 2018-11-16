#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime
import dateutil.parser

from flask import Flask, jsonify, request, render_template, make_response
import json
import calc

app = Flask(__name__)

# 書き込み
@app.route("/api/write", methods=['GET'])
def sensor_request_api():
    # リクエストの引数から，各センサ値を抽出
    params = request.args
    wetness = params.get('wetness', default='0', type = str)
    dryness = int(100 - (float(wetness) / 4095.0) * 100.0)
    temperature = params.get('temperature', default='0', type = int)
    humidity = params.get('humidity', default='0', type = int)
    co2 = params.get('co2', default='0', type = int)
    tvoc = params.get('tvoc', default='0', type = int)

    # 乾くまでの残り時間を計算
    rest_of_time = calc.calc(dryness, temperature, humidity)

    # センサ値と計算値をinfluxDBに書き込む
    write(dryness, temperature, humidity, co2, tvoc, rest_of_time)

    print('dryness:')
    print(dryness)
    print('temperature')
    print(temperature)
    # 完了したで
    return 'success'

# 読み込み
@app.route('/api/read', methods=['GET'])
def sensor_response_api():

    client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
    sensordata = client.query("select * from sensordata")

    sensordata_json2list = list(sensordata.get_points(measurement=None))

    # 要素の有無
    if len(sensordata_json2list) == 0:
        #return str("過去%s分間のデータがありません" % str(minutes))
        datalist = [0.0,0.0,0.0,0.0,0.0,0.0]
        return "NoData"
    else:
        # リストから最新の要素を取り出す
        # sensordata_dict = sensordata_json2list[-1]
        sensordata_json2list.reverse()
        sensors_dict = []
        count = 0
        for sensordata_json in sensordata_json2list:
            # キー
            # keys = list(sensordata_dict.keys())
            # 値
            # values = list(sensordata_dict.values())
            # return str(sensordata_json['temperature'])
            if is_int(sensordata_json['dryness']) and is_int(sensordata_json['temperature']) and is_int(sensordata_json['humidity']) and is_int(sensordata_json['co2']) and is_int(sensordata_json['tvoc']):
                # 取ってきた値を変数に代入()
                dryness = (int(sensordata_json['dryness']))
                temperature = int(sensordata_json['temperature'])
                humidity = int(sensordata_json['humidity'])
                co2 = int(sensordata_json['co2'])
                tvoc = int(sensordata_json['tvoc'])
                rest_of_time = int(sensordata_json['rest_of_time'])
                recording_time_str = sensordata_json['time']
                recording_time = dateutil.parser.parse(recording_time_str)
                recording_time_jst = recording_time.astimezone(timezone('Asia/Tokyo'))
                recording_time_simple_str = recording_time_jst.strftime('%Y-%m-%d %H:%M:%S')

                values = { 'count': count, 'dryness': dryness, 'temperature': temperature, 'humidity': humidity, 'co2': co2, 'tvoc': tvoc, 'rest_of_time': rest_of_time, 'time': recording_time_simple_str }
                sensors_dict.append(values)
                count += 1
                if (count >= 1000):
                    break

        return jsonify({'values': sensors_dict})

#
@app.route("/", methods=['GET'])
def index():
    client = InfluxDBClient(host='influxdb', port=8086, database='superdry')
    sensordata = client.query("select * from sensordata where time >= now() - 30m")

    sensordata_json2list = list(sensordata.get_points(measurement=None))

    # 要素の有無
    if len(sensordata_json2list) == 0:
        #return str("過去%s分間のデータがありません" % str(minutes))
        datalist = [0.0,0.0,0.0,0.0,0.0,0.0]
        return "error"
    else:
        # リストから最新の要素を取り出す
        sensordata_dict = sensordata_json2list[-1]
        # キー
        keys = list(sensordata_dict.keys())
        # 値
        values = list(sensordata_dict.values())

        # 取ってきた値を変数に代入()
        dryness = (int(sensordata_dict['dryness']))
        temperature = int(float(sensordata_dict['temperature']))
        co2 = int(sensordata_dict['co2'])
        humidity = int(sensordata_dict['humidity'])
        tvoc = int(sensordata_dict['tvoc'])
        rest_of_time = int(sensordata_dict['rest_of_time'])
        time = sensordata_dict['time']

        return render_template('index.html', dryness=dryness, temperature=temperature, humidity=humidity, rest_of_time=rest_of_time)

# influxDBにデータを書き込む関数
def write(dryness, temperature, humidity, co2, tvoc, rest_of_time):

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
                "dryness": dryness,
                "temperature": temperature,
                "humidity": humidity,
                "co2": co2,
                "tvoc": tvoc,
                "rest_of_time": rest_of_time
            }
        }
    ]

    print("client: ",host, port, user, password, dbname)
    client = InfluxDBClient(host, port, user, password, dbname)
    client.create_database(dbname)
    client.write_points(json_body)

    return "write"

def is_int(v):
    return type(v) is int

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=6500, debug=True)
