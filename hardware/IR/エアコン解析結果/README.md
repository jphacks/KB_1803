# エアコン解析結果

## 解析したエアコン  
Panasonic CS-285CF

## ファイル名  
モード，温度の範囲，風量，風向

## データの構成
Leader＋識別番号＋Tracer＋Leader＋データ＋Tracer

1パルス：約430us　→　T = 430us  
ONタイム：38kHzでデューティ比50%の出力  
OFFタイム：0の出力


Leader  
　データの開始を知らせるコード  
　ONタイム：8T　　OFFタイム：4T

Tracer  
　データの終わりを知らせるコード  
　ONタイム：1T　　OFFタイム：数ms  
　測定したリモコンは10ms以上OFFタイムであったため，その後の処理も考慮し5.5ms以上のOFFタイムで判定
