import numpy as np
# from sklearn import linear_model
from sklearn.externals import joblib
from sklearn.ensemble import RandomForestRegressor # ランダムフォレスト回帰用

def prediction(X_test):
    # # load the model
    clf = joblib.load('./model.pkl')

    # # 作成したモデルから予測（学習済みモデル使用）
    y_test_pred = clf.predict(X_test)[0]
    
    return float(y_test_pred[0]/60 - 155.95807119469603) #モデルの100%のときの予測時間（秒）を引いて０にしてやる

def calc(dryness):

    # # テストデータ用意
    X_test = dryness

    # # # 推定
    rest_of_time = prediction(X_test)

    # # 結果をリターン
    #rest_of_time = 1.0
    return rest_of_time

