

def calc(dryness, temperature, humidity):
    #from sklearn import linear_mode
    #import pandas as pd

    # テストデータの作成
    #X_test = [temperature,humidity]
    #Y_test = dryness
    # 学習データの作成
    #X_train = 
    #Y_train = 
    # 線形回帰モデル構築
    #clf = linear_model.LinearRegression()
    #clf.fit(X_train, Y_train)

    # 偏回帰係数と切片から，関数を導出
    #dfCoef = pd.DataFrame({"Name":X_train.columns,
    #                    "Coefficients":clf.coef_}).sort_values(by='Coefficients') 
    #coeflist = dfCoef.values.tolist()
    #coefdict = {}
    #for name in dfCoef.Name.values.tolist():
    #    for element in coeflist:
    #        if element[1] == name:
    #            coefdict[name] = element[0]
    #        else:
    #            pass

    # 切片 (誤差)
    #seppen = clf.intercept_
    # 関数の定義
    #def dryness_function(temperature,humidity,coefdict,seppen):
    #    return temperature*coefdict["temperature"] + humidity*coefdict["humidity"] + seppen
    # その関数で，dryness = 100 の時の(temperature, humidity)座標
    #zahyou100 = dryness_function(temperature,humidity,coefdict,seppen)
    # 今の(temperature, humidity)座標
    #nowzahyou = [temperature, humidity]
    # 2点の間の距離が，残り時間（min）
    #a = numpy.array(zahyou100)
    #b = numpy.array(nowzahyou)
    #u = b - a
    rest_of_time = 1.0 #numpy.linalg.norm(u)

    # 結果をリターン
    return rest_of_time
