# イメージは公式のを用いる.onbuildでrequirementsを読み込み，カレントディレクトリの中身をコンテナ内の/usr/src/appにコピー
#詳しくはhttps://github.com/docker-library/python/blob/7eca63adca38729424a9bab957f006f5caad870f/3.6/onbuild/Dockerfileをご覧ください

FROM python:3.5.2-onbuild

#必要なパッケージ
RUN apt-get update
RUN pip install --upgrade pip

# サーバを立ち上げる
CMD ["python", "/usr/src/app/app.py"]
