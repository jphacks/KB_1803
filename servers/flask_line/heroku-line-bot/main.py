#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from pytz import timezone
from datetime import datetime
import os

from flask import Flask, jsonify, request, render_template, make_response,abort

import json

from linebot import (
    LineBotApi, WebhookHandler
)
from linebot.exceptions import (
    InvalidSignatureError
)
from linebot.models import (
    MessageEvent, TextMessage, TextSendMessage,
)

app = Flask(__name__)

# get channel_secret and channel_access_token from your environment variable
channel_secret = os.getenv('LINE_CHANNEL_SECRET', None)
channel_access_token = os.getenv('LINE_CHANNEL_ACCESS_TOKEN', None)
if channel_secret is None:
    print('Specify LINE_CHANNEL_SECRET as environment variable.')
    sys.exit(1)
if channel_access_token is None:
    print('Specify LINE_CHANNEL_ACCESS_TOKEN as environment variable.')
    sys.exit(1)

line_bot_api = LineBotApi(channel_access_token)
handler = WebhookHandler(channel_secret)

# line_bot_api = LineBotApi('tRgwuGuCkcxVaEsSHWUG9IKlZP4GbWg8NHBtVoWZyiYZKXFMUcb927L3dISXnkhFYM3UJpqXrK8Fb+jvgYa1sypdD9vrvTmBVQ/4/Xk8RTM1wyRQIwZIk7zZtEOsEttIeMMJcgty2h/gi1n2c28HtwdB04t89/1O/w1cDnyilFU=')
# handler = WebhookHandler('8cf05bba309414fbfacc902fb0a92028')


# "/api"で起動するアプリ:書き込み
@app.route("/api", methods=['GET'])
def sensor_request_api():
    return 'success'

# ブラウザで訪れたときに起動する機能：botに送信
@app.route("/")
def hello_world():
    line_bot_api.push_message('U05a569eac688ebe9aeb37d5386dc39a6', TextSendMessage(text='プッシュ通知ｷﾀ━━━━(ﾟ∀ﾟ)━━━━!!!!!'))
    return "Complete"

# botに送信したときに起動する機能
@app.route("/callback", methods=['POST'])
def callback():
    # get X-Line-Signature header value
    signature = request.headers['X-Line-Signature']

    # get request body as text
    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)

    # handle webhook body
    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)

    return 'OK'

#オウム返し
@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    line_bot_api.reply_message(
        event.reply_token,
        TextSendMessage(text=event.message.text))



if __name__ == "__main__":
    port = int(os.getenv("PORT", 5000))
    app.run(host="0.0.0.0", port=port)

