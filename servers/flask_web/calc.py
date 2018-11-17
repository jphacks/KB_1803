import numpy as np
from sklearn import linear_model
from sklearn.externals import joblib

def prediction(X_test):
    # #X_testはnumpy.ndarrayでshapeは(1, 3)

    # # load the model
    clf = joblib.load('./model.pkl')

    # # 作成したモデルから予測（学習済みモデル使用）
    y_test_pred = clf.predict(X_test)
    
    return float(y_test_pred[0]/60 - 156.07096152) #156.07096152は、モデルの100%のときの予測時間（秒）

def calc(dryness):

    # # テストデータ用意
    X_test = dryness

    # # # 推定
    rest_of_time = prediction(X_test)

    # # 結果をリターン
    #rest_of_time = 1.0
    return rest_of_time



