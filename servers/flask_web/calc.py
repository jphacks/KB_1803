# import numpy as np
# from sklearn import linear_model
# from sklearn.externals import joblib

def calc(dryness, temperature, humidity):

    # # テストデータ用意
    # X_test = np.array([[dryness,humidity,temperature]])

    # # 推定
    # rest_of_time = prediction(X_test)

    # # 結果をリターン
    rest_of_time = 1
    return rest_of_time


def prediction(X_test):
    # #X_testはnumpy.ndarrayでshapeは(1, 3)

    # # load the model
    # clf = joblib.load('./model.pkl')

    # # 作成したモデルから予測（学習済みモデル使用）
    # y_test_pred = clf.predict(X_test)

    return int(y_test_pred[0]/60)
