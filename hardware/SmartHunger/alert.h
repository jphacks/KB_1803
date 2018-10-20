
/************************************
     ALERTを送信する条件
*************************************/

bool AlertFlag(int t, int h, int l, int c, int count){
//  if( count > 0 && HUMAN_c == "1"){
//    IR(data3);
//    return true;
//  } else if( t > 1 && HUMAN_c == "1"){
//    IR(data2);
//    return true;
//  } else if( h > 1 && HUMAN_c == "1"){
//    IR(data1);
//    return true;
//  } else {
//    return false;
//  }
}

/************************************
     IFTTTへ送信するデータの作製
*************************************/

byte LineSend() {

  if (client.connect(server, 80) != 1) return 0;

  
  client.print("GET /trigger/");
  client.print(makerEvent);
  client.print("/with/key/");
  client.print(makerKey);
  client.print("?value1=");
  client.print("");
  client.print("&value2=");
  client.print("");
  client.print("&value3=");
  client.print("");
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  int count = 0;
  while (client.available() == 0) {
    delay(50);

    count++;
    if (count > 20 * 20) { 
      // about 20s
      return -1;
    }
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (client.connected() == 0) {
    client.stop();
  }
  return 1;
}


