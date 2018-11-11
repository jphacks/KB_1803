#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime

from flask import Flask, jsonify, request, render_template, make_response
import json

app = Flask(__name__)

# "/api"で起動するアプリ:書き込み
@app.route("/api", methods=['GET'])
def sensor_request_api():
    return 'success'

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
