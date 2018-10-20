# エアコン解析結果

## 解析したエアコン  
Panasonic CS-285CF

## ファイル名  
モード，温度の範囲，風量，風向

## データの構成
Leader＋識別番号＋Tracer＋Leader＋データ＋Tracer

Leader  
　データの開始を知らせるコード  
　ON：8T　　OFF4T

Tracer  
　データの終わりを知らせるコード  
　ON：1T　　OFF：数ms  
　測定したリモコンは10ms以上OFFタイムであったため，その後の処理も考慮し5.5ms以上のOFFタイムで判定
