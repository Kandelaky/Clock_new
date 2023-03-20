//обработчики событий WEB сервера
//
//
//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}
//
//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin") {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
      content="";
      content+="<html><body><form action='/login' method='POST'>";
      content+="<table border=0 width=100% height=100%>";
      content+="<tr><td align=center>";
      content += hstname;
      content+="<table border=0>";
      content+="<tr><td bgcolor='#cccccc'>User:</td><td><input type='text' name='USERNAME' placeholder='user name'></td></tr>";
      content+="<tr><td bgcolor='#cccccc'>Password:</td><td><input type='password' name='PASSWORD' placeholder='password'></td></tr>";
      content+="<tr><td bgcolor='#cccccc'></td><td bgcolor='#cccccc'><input type='submit' name='SUBMIT' value='Submit'></form></td></tr>";
      content+="</table>";
      content+="</td></tr></table>";
      content+="You also can go <a href='/inline'>here</a>";
      content+="</body></html";
        server.send(200, "text/html", content); 
}
//
//
//404
/*
void handle_NotFound(){
//  server.send(404, "text/plain", "Not found");
  server.sendHeader("Location", "/",true);   //Redirect to our html web page
  server.send(302, "text/plane","");
}
*/
//no need authentification
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//
//
//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  auth();
//
//включение/выключение синхронизации по NTP, если WiFi в режиме ST
   if(cntwifi==0)
    {
      if (server.arg("setntp")!= "")
      {
        if(server.arg("ntp")=="")
        {
         web_ntp=0;
         preferences.begin("nvs", false);                    //открываем хранилище nvs на чтене и запись
         preferences.putUInt("web_ntp", web_ntp);
         preferences.end();                                    //закрываем хранилище
               Serial.println("ntp off\n");
        }
      }
       if(server.arg("ntp")=="on")
       {
       web_ntp=1;
       synfl=0;
       Serial.println("ntp on\n");
       preferences.begin("nvs", false);                    //открываем хранилище nvs на чтене и запись
       preferences.putUInt("web_ntp", web_ntp);
       preferences.end();                                    //закрываем хранилище
       }
    }
//    
//ручная установка времени
    if (server.arg("time")!= "")
    {
       String mytime=server.arg("time");
       myhours=mytime.substring(0,2);
       myminutes=mytime.substring(3,5);
       Serial.println(myminutes);
    }
    else
    {
       myhours="";
       myminutes="";
    }
//
//
  if(server.arg("value")!="")
  {
  volume=server.arg("value");
  preferences.begin("nvs", false);                    //открываем хранилище nvs
  preferences.putString("volume", volume);              //сохраняем значение громкости
  preferences.end();                                  //закрываем хранилище
  Serial.print("Volume=");
  Serial.println(volume);
  }
//
      if (server.arg("setntp")!= "")
      {  
        if(server.arg("boy")=="")
        {
            boy=0;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("boy", boy);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("Boy=off");
        }
      }
      if(server.arg("boy")=="on")
        {
            boy=1;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("boy", boy);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("Boy=on");
        }
//    
        if (server.arg("setntp")!= "")
      {  
        if(server.arg("voice")=="")
        {
            voice=0;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("voice", voice);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("voice=off");
        }
      }
      if(server.arg("voice")=="on")
        {
            voice=1;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("voice", voice);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("voice=on");
        }

       if (server.arg("setntp")!= "")
      {  
        if(server.arg("cshelk1")=="")
        {
            cshelk1=0;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("cshelk1", cshelk1);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("cshelk1=off");
        }
      }

      if(server.arg("cshelk1")=="on")
        {
            cshelk1=1;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("cshelk1", cshelk1);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("cshelk1=on");
        }

       if (server.arg("setntp")!= "")
      {  
        if(server.arg("cshelk2")=="")
        {
            cshelk2=0;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("cshelk2", cshelk2);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("cshelk2=off");
        }
      }

      if(server.arg("cshelk2")=="on")
        {
            cshelk2=1;
            preferences.begin("nvs", false);                    //открываем хранилище nvs
            preferences.putInt("cshelk2", cshelk2);              
            preferences.end();                                  //закрываем хранилище
            Serial.print("cshelk2=on");
        }


//
  if(server.arg("TZ")!="")
  {
    t_zone=server.arg("TZ");
    preferences.begin("nvs", false);                    //открываем хранилище nvs
    preferences.putString("t_zone", t_zone);              //сохраняем значение временной зоны
    preferences.end();                                  //закрываем хранилище
    if(web_ntp !=0)
    {
      gmtOffset_sec=t_zone.toInt();
      gmtOffset_sec=gmtOffset_sec*3600;
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Синхронизируем внутренние часы со временем из интернета
    }
  }
  if(server.arg("DST")!="")
  {
    daylightOffset_sec=3600;
    preferences.begin("nvs", false);                    //открываем хранилище nvs
    preferences.putInt("dst", daylightOffset_sec);      //сохраняем переход на летнее время
    preferences.end();                                  //закрываем хранилище
    if(web_ntp !=0)
      {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Синхронизируем внутренние часы со временем из интернета
      }
  }
  else
  {
    if(server.arg("setntp")!= "")
    {
      daylightOffset_sec=0;
      preferences.begin("nvs", false);                    //открываем хранилище nvs
      preferences.putInt("dst", daylightOffset_sec);      //сохраняем переход на летнее время
      preferences.end();                                  //закрываем хранилище
     if(web_ntp !=0)
      {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Синхронизируем внутренние часы со временем из интернета
      }
    }
  }
//
    hdr();  //вывод хедера
//     
     content += "  <h1>Установки часов</h1>";
     content += hstname;
     content += "<table width=100% height=100% >";
     content += "<form method='POST'>";
     content += "<tr><td align=center><table border=1>";
     content += "<tr><td align=center bgcolor='#cccccc'><p>Громкость боя:  <span id='servoPos'></span>%</p></td><td align=center bgcolor='#cccccc'><a href=wifi>Настройка<br>WiFi</a></td></tr>";
     content += "<tr><td><p><div class='container'>";
//VALUE
     content += "  <input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"servoSlider\" value=";
     content += volume+" onchange=\"servo(this.value)\"/>";
     content += "</div></p>";
     content += "</td><td align=center>";
//вывод режима WiFi
/*  if(cntwifi==1)
  {
    content += "AP";
  }
  else
  {
    content += "ST";
  }
*/  
  content += "<a href=datchik>Настройка<br> датчиков</a>";
     content += "</td></tr>";
//
  content += "<tr><td align=center>";
  content += "<table width=\"100%\"><tr><td>";
  content += "<input type='checkbox' id='boy' class='toggle-button' name='boy'";
    if(boy==1)
    {
      content +=  " checked";
    }
  content +=  ">";
  content += "  <label for='boy' class='text'>Бой</label>";
  content += "</td><td>";
  content += "&nbsp;&nbsp;&nbsp;<input type='checkbox' id='voice' class='toggle-button' name='voice'";
  if(voice==1)
    {
      content +=  " checked";
    }
  content +=  ">";
  content += "  <label for='voice' class='text'>Голос</label>";
   content += "</td></tr>";

  content +=  "<tr><td>";
  content += "<input type='checkbox' id='cshelk1' class='toggle-button' name='cshelk1'";
  if(cshelk1==1)
    {
      content +=  " checked";
    }
  content +=  ">";
  content += "  <label for='voice' class='text'>Щелчок 1</label>";
  
  content += "</td><td>";
  content += "&nbsp;&nbsp;&nbsp;<input type='checkbox' id='cshelk2' class='toggle-button' name='cshelk2'";
  if(cshelk2==1)
    {
      content +=  " checked";
    }
  content +=  ">";
  content += "  <label for='voice' class='text'>Щелчок 2</label>";

  
  content += "</td></tr>";
  
  content += "</table>";
  content += "</td><td>&nbsp;";
  content += "</td></tr>";
  
//NTP
//web_ntp
  if(cntwifi==1)
  {
     content += "<tr><td bgcolor='#cccccc'>&nbsp;<input type='checkbox' id='toggle-button' class='toggle-button' name='ntp' disabled>";
  }
  else
  {
    if(web_ntp !=0)
    {
       content += "<tr><td bgcolor='#cccccc'>&nbsp;<input type='checkbox' id='toggle-button' class='toggle-button' name='ntp' checked>";
    }
    else
    {
      content += "<tr><td bgcolor='#cccccc'> &nbsp;<input type='checkbox' id='toggle-button' class='toggle-button' name='ntp'";
    }
  }
     content += "  <label for='toggle-button' class='text'>NTP</label></td>";
//TIME
   if(web_ntp !=0)
    {
        content += "<td bgcolor='#cccccc'>  <input type='time' name='time'  disabled></td></tr>";
    }
    else
    {
      content += "<td bgcolor='#cccccc'>  <input type='time' name='time'></td></tr>";
    }
     content += "<tr><td align=center>Часовой пояс*</td><td align=center><input type=text name=TZ size=3 maxlength=3  value=";
     content += t_zone;
     content += " pattern=\"([+-]?[01]?[0-9]|2[0-3])\"></td></tr>";
     content += "<tr><td  align=center>Летнее/Зимнее время*</td><td  align=center><input type=\"checkbox\" name=\"DST\"";
     if (daylightOffset_sec !=0)
     {
      content += " checked";
     }
     content += "> DST</td></tr>";
     content += "<tr><td bgcolor=\"#cccccc\"><small><i>*только для NTP</i></small></td><td align=center bgcolor='#cccccc'>";
     content += "  <input type='submit' value='Submit' class='submit'></td></tr>";
     content += "</table>";
     content += "</td></tr></table>";
     content += "<input type='hidden' id='setntp' name='setntp' value='34657'>";
     content += "</form>";
     content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
     load_http("/slider.html");
     load_http("/ftr.html");

//  if (server.hasHeader("User-Agent")) {
//    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
//    content += "the user agent used is : " + server.header("REMOTE_ADDR") + "<br><br>";
//  }
//  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);


//if(server.arg("TZ")!="")
//  {
//    delay(5000);
//    ESP.restart();
//  }
}  
/*
// Handle root url (/)
void handle_root() {
  String message = HTML;
  server.send(200, "text/html", message);
}
*/
//
//////////////////////////////////////////////////////////////////////////////////////////
//Страница установки WiFi
void handleWiFi() {
    Serial.println("Enter handleWiFi");
  String header;
  auth();
//страница установки WiFi
//ctlwifi setwifi
    preferences.begin("nvs", false);                    //открываем хранилище nvs на чтене и запись
    if((server.arg("ctlwifi")=="on")&&(cntwifi==0))   //режим ST
    {
      cntwifi=1;
      preferences.putUInt("cntwifi", 1);
      Serial.print(cntwifi);
      cnt_wifi=1;
    }
    else
    {
      if((server.arg("setwifi") !="")&&(cntwifi==1))  //режим AP
      {
       cntwifi=0;
//       preferences.begin("nvs", false);                    //открываем хранилище nvs на чтене и запись
       preferences.putUInt("cntwifi", 0);
       Serial.print(cntwifi);
       cnt_wifi=1;
      
//       cntrlWiFi;                                          //переконфигурируем WiFi
      }
    }
    if(server.arg("hstname") !="")
    {
      hstname=server.arg("hstname");
      preferences.putString("hstname", hstname);
      cnt_wifi=1;
    }    

    if(server.arg("APSSID") !="")
    {
      ssidAP=server.arg("APSSID");
//      Serial.println("APSSID="+ssidAP);
      preferences.putString("ssidAP", ssidAP);
//      Serial.println(preferences.getString("ssidAP", ssidAP_DEF));
      cnt_wifi=1;
    }
    if(server.arg("APPASSWD") !="")
    {
      passwordAP=server.arg("APPASSWD");
      preferences.putString("passwordAP", passwordAP);
//      Serial.println(passwordAP);
      cnt_wifi=1;
    }
    if(server.arg("STSSID") !="")
    {
      ssid=server.arg("STSSID");
      preferences.putString("ssid", ssid);
       cnt_wifi=1;
    }
    if(server.arg("STPASSWD") !="")
    {
      password=server.arg("STPASSWD");
      preferences.putString("password", password);
       cnt_wifi=1;
    }
    if(server.arg("IP") !="")
    {
      String _local_ip=server.arg("IP");
      preferences.putString("local_ip", _local_ip);
       cnt_wifi=1;
    }
    if(server.arg("IP") !="")
    {
      String _subnet=server.arg("MASK");
      preferences.putString("subnet", _subnet);
       cnt_wifi=1;
    }
    if(server.arg("IP") !="")
    {
      String _gateway=server.arg("GW");
      preferences.putString("gateway", _gateway);
       cnt_wifi=1;
    }

//    preferences.end();                                  //закрываем хранилище
//
//
//
String mmm="";
    hdr();    //вывод хедера
//        
        content += "  <h1>Настройка WiFi</h1>";
        content += "<table width=100% height=100% >";
        content += "<form method='POST'>";
        content += "<tr><td align=center>";
        content += "<table border=1>";
        content += "<tr><td>Hostname:</td><td><input type='text' name='hstname' placeholder='Hostname' value=";
                mmm=preferences.getString("hstname", hstname_DEF);
                content += mmm;                
        content += "></td></tr>";
        content += "<tr><td align=center bgcolor=\"#cccccc\">Настройки для режима <b>ST</b></td><td align=center bgcolor=\"#cccccc\">(для подключения к роутеру)</td></tr>";
        content += "<tr><td>SSID:</td><td><input type='text' name='STSSID' placeholder='SSID' value=";
                mmm=preferences.getString("ssid", "");
                content += mmm;
        content += "></td></tr>";
        content += "<tr><td>Password:</td><td><input type='password' name='STPASSWD' placeholder='password' value=";
                mmm=preferences.getString("password", "");
                content += mmm;
        content += "></td></tr>";
        content += "<tr><td align=center bgcolor=\"#cccccc\">Настройки для режима <b>AP</b></td><td align=center bgcolor=\"#cccccc\">(без подключения к роутеру)</td></tr>";
        content += "<tr><td>SSID:</td><td><input type='text' name='APSSID' placeholder='SSID' value=";
                mmm=preferences.getString("ssidAP", ssidAP_DEF);
//                       Serial.println("ssidAP="+mmm);
                       content += mmm;
        preferences.end();                                  //закрываем хранилище
        content += "></td></tr>";
        content += "<tr><td>Password:</td><td><input type='password' name='APPASSWD' placeholder='password' value=";
//        content += passwordAP.c_str();
                mmm=preferences.getString("passwordAP", passwdAP_DEF);
//              Serial.println("PasswdAP="+mmm);
                       content += mmm;
        content += "></td></tr>";
        content += "<tr><td>Static IP</td><td><input type='text' name='IP' placeholder='xxx.xxx.xxx.xxx' value='";
                content +=  Ip2String(local_ip);
        content +="' pattern=\"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$\"></td></tr>";
        content += "<tr><td>Netmask</td><td><input type='text' name='MASK' placeholder='255.255.255.0'value='";
                content +=  Ip2String(subnet);
        content += "' pattern=\"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$\"></td></tr>";
        content += "<tr><td>Gateway</td><td><input type='text' name='GW' placeholder='192.168.111.1' value='";
                content +=  Ip2String(gateway);
        content += "' pattern=\"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$\"></td></tr>";
        content += "<tr><td align=center bgcolor=\"#cccccc\">Выбор режима</b></td><td align=center bgcolor=\"#cccccc\">ST/AP</td></tr>";
        content += "<tr><td align=center >ST/AP</td><td align=center></td></tr>";
        content += "<tr><td align=center bgcolor=\"#cccccc\">";
        content += "<div class=\"container\">";
        if(cntwifi==1)
        {
          content += "  <input type=\"checkbox\" id=\"toggle-button\" class=\"toggle-button\" name=\"ctlwifi\" checked>"; //режим ST
          }
          else
          {
            content += "  <input type=\"checkbox\" id=\"toggle-button\" class=\"toggle-button\" name=\"ctlwifi\">"; //режим AP
          }
        content += "    <label for='toggle-button' class='text' >";
        content += "</label>";
        content += "</div>";
        content += "</td><td bgcolor=\"#cccccc\" align=center><input type=\"submit\" value=\"Submit\" class=\"submit\"></td></tr></td></tr>";
        content += "</table>";
        content += "</td></tr></table>";
        content += "<input type=\"hidden\" id=\"setwifi\" name=\"setwifi\" value=\"34657\">";
        content += "</form>";
        content += "<a href=/>Вернуться на страницу настройки часов</a>";
        load_http("/slider.html");
        load_http("/ftr.html");
            preferences.end();                                  //закрываем хранилище
  server.send(200, "text/html", content);
    if(cnt_wifi)                                        //если были какие-то изменения в еастройках WiFi
    {
      delay(5000);
       ESP.restart();
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
//
//Страница настройки датчиков
void handleDatchik(void)
{
  Serial.println("Enter handleDatchik");
  String header;
  auth();
//  
//
    preferences.begin("nvs", false);                    //открываем хранилище nvs на чтене и запись
      if(server.arg("datname") !="")
      {
        datname=server.arg("datname");
        preferences.putString("datname", datname);
      }    
      if(server.arg("datchar") !="")
      {
        datchar=server.arg("datchar");
        preferences.putString("datchar", datchar);
      }    
      
      if(server.arg("bmp0_datname") !="")
      {
        bmp0_datname=server.arg("bmp0_datname");
        preferences.putString("bmp0_datname", bmp0_datname);
      }    
      if(server.arg("bmp0_datchar") !="")
      {
        bmp0_datchar=server.arg("bmp0_datchar");
        preferences.putString("bmp0_datchar", bmp0_datchar);
      }    
      if(server.arg("bmp1_datname") !="")
      {
        bmp1_datname=server.arg("bmp1_datname");
        preferences.putString("bmp1_datname", bmp1_datname);
      }    
      if(server.arg("bmp1_datchar") !="")
      {
        bmp1_datchar=server.arg("bmp1_datchar");
        preferences.putString("bmp1_datchar", bmp1_datchar);
      }    
      
      if(server.arg("lp1") !="")
      {
        datus[0]=server.arg("lp1");
        preferences.putString("lp1", datus[0]);
      }
      if(server.arg("lp2") !="")
      {
        datus[1]=server.arg("lp2");
        preferences.putString("lp2", datus[1]);
      }
      if(server.arg("mc1") !="")
      {
        datus[2]=server.arg("mc1");
        preferences.putString("mc1", datus[2]);
      }
      if(server.arg("mc2") !="")
      {
        datus[3]=server.arg("mc2");
        preferences.putString("mc2", datus[3]);
      }
      if(server.arg("mc3") !="")
      {
        datus[4]=server.arg("mc3");
        preferences.putString("mc3", datus[4]);
      }
//    if((server.arg("ctlwifi")=="on")&&(cntwifi==0))   //режим ST
//    preferences.putString("gateway", _gateway);
                preferences.end();                                  //закрываем хранилище
//
    hdr();    //вывод хедера
//        
        content += "  <h1>Настройка датчиков</h1>";
        content += hstname;
        content += "<table width=100% height=100% >";
        content += "<form method='POST'>";
        content += "<tr><td align=center>";
        content += "<table border=1>";
        content += "<tr><td align=center bgcolor=\"#cccccc\" colspan=\"6\">Локально установленные</td></tr>";        
        content += "<tr><td align=center bgcolor=\"#eeeeee\" colspan=\"2\">Имя датчика</td><td align=center bgcolor=\"#eeeeee\"  colspan=\"2\">Значение<br>датчика</td><td align=center bgcolor=\"#eeeeee\"  colspan=\"2\">Единицы<br>измерения</td></tr>";
        if(tint!=65535)
          {
          datch_html(datname, "datname", "datchar");
          }
          else
          {
          content += "<tr><td align=center colspan=\"2\">---</td><td  colspan=\"2\">---</td><td  colspan=\"2\">---</td></tr>";
          }
          if(bmp_stat)
          {
          datch_html(bmp0_datname,"bmp0_datname","bmp0_datchar");
          }
          else
          {
          content += "<tr><td align=center colspan=\"2\">---</td><td  colspan=\"2\">---</td><td  colspan=\"2\">---</td></tr>";
          }
          if(bmp_stat)
          {
          datch_html(bmp1_datname,"bmp1_datname","bmp1_datchar");
          }
          else
          {
          content += "<tr><td align=center colspan=\"2\">---</td><td  colspan=\"2\">---</td><td  colspan=\"2\">---</td></tr>";
          }
//          content += "</table><table border=1>";
          content += "<tr><td align=center bgcolor=\"#cccccc\" colspan=\"6\">Условия вывода данных от датчиков</b></td ></tr>";
          content += "<tr><td align=center bgcolor=\"#eeeeee\">Имя датчика</td><td align=center bgcolor=\"#eeeeee\">Цикл1</td><td align=center bgcolor=\"#eeeeee\">Цикл2</td></td><td align=center bgcolor=\"#eeeeee\">1 Хлопок</td><td align=center bgcolor=\"#eeeeee\">2 Хлопка</td><td align=center bgcolor=\"#eeeeee\">3 Хлопка</td></tr>";
//////////////////////////////////////////////////////////////////////////////////////////
//вывод неустаревших броадкастов
//////////////////////////////////////////////////////////////////////////////////////////
//очистка массива флагов 
          for(int j=0;j<3;j++)
          {
            radf[j]=0;
          }
//
  for(int i=0;i<10;i++)
  {
    if(brdn[i]<3)
     {
          content += "<tr><td align=center>";
          content += brd[0][i];
//         content += "bbb";
          for(int j=0;j<5;j++)
          {
          content += "</td><td align=center>";
          content += "<input type=\"radio\" id=\"";
          content += t_radio[j];
          content += "\" name=\"";
          content += t_radio[j];
          content += "\"";
          content += "value=\"";;
          content += brd[0][i];
          content += "\"";
              if(brd[0][i]==datus[j])
              {
                content += "checked";
                radf[j]=1;
              }
              content += ">";
          }
          content += "</td></tr>";
     }          
  }
//          //           //
//проверка на то, выводился ли checked
      for(int i=0;i<5;i++)
      {
          if(radf[i]==0)
          {
            if(datus[i]!="NON")
            {
          content += "<tr><td align=center>";
          content += datus[i];
              for(int j=0;j<3;j++)
              {
                content += "</td><td align=center>";
                content += "<input type=\"radio\" id=\"";
                content += t_radio[j];
                content += "\" name=\"";
                content += t_radio[j];
                content += "\"";
                content += "value=\"";;
                content += datus[i];
                content += "\"";
                if(datus[i]==datus[j])
                {
                    content += "checked";
                  radf[i]=1;
                }
                content += " disabled>";
              }
            content += "</td></tr>";
            }
          }
      }
//   }
//  }
//          //           //
// вывод строки NON
          content += "<tr><td align=center>";
          content += "NON";
  for(int i=0;i<5;i++)
  {
    content += "</td><td align=center>";
    content += "<input type=\"radio\" id=\"";
    content += t_radio[i];
    content += "\" name=\"";
    content += t_radio[i];
    content += "\"";
    content += "value=\"";;
    content += "NON";
    content += "\"";
    if(datus[i]=="NON")
    {
      content += "checked";
    }
    content += ">";
  }
    content += "</td></tr>";  
//          //           //
//
        content += "</tr><td bgcolor=\"#cccccc\" colspan=\"5\">&nbsp;</td><td bgcolor=\"#cccccc\" align=center><input type=\"submit\" value=\"Submit\" class=\"submit\"></td></tr>";
        content += "</table>";
        content += "</td></tr></table>";
        content += "</form>";
        content += "<a href=/>Вернуться на страницу настройки часов</a>";
        ftr();
        server.send(200, "text/html", content);
//        
}
//
//
///////////////////////////////////////////////////////////////////////////////
//Web OTA
///////////////////////////////////////////////////////////////////////////////
 void form_ota (void)
  {
    auth();
    server.sendHeader("Connection", "close");
const char* upgrd =
//////////////////////////////////////////////////////////////////////////////////////////
/*
 * Server Index Page
 */
 "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script src=\"//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js\"></script>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";
    server.send(200, "text/html", upgrd);
  }
//
void update_ota (void)
  {
    server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        ESP.restart();
      } else {
        Update.printError(Serial);
      }
    }
  });
  }
///////////////////////////////////////////////////////////////////////////////
void File_Upload(){
  auth();
  hdrl();
  content += F("<h3>Select File to Upload</h3>"); 
  content += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  content += F("<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>");
  content += F("<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>");
  ftrl();
//  content += F("<a href='/'>[Back]</a><br><br>");
//  append_page_footer();
  server.send(200, "text/html",content);
}
/////////////////////////////
File UploadFile; 
void handleFileUpload(){ // upload a new file to the Filing system
  auth();
   HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("Upload File Name: "); Serial.println(filename);
    if(filename!="/")
    {
    SPIFFS.remove(filename);
//    SD.remove(filename);                         // Remove a previous version, otherwise data is appended the file again
//    UploadFile = SD.open(filename, FILE_WRITE);  // Open the file for writing in SPIFFS (create it, if doesn't exist)
    UploadFile = SPIFFS.open(filename, FILE_WRITE);
    }
    filename = String();
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
  } 
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if(UploadFile)          // If the file was successfully created
    {                                    
      UploadFile.close();   // Close the file again
      Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
      hdrl();
      content += F("<h3>File was successfully uploaded</h3>"); 
      content += F("<h2>Uploaded File Name: "); content += uploadfile.filename+"</h2>";
      content += F("<h2>File Size: "); content += file_size(uploadfile.totalSize) + "</h2><br>";
      content += "<a href=/upload>Next File</a>";
//      append_page_footer();
      server.send(200,"text/html",content);
    } 
    else
    {
      ReportCouldNotCreateFile("upload");
    }
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportCouldNotCreateFile(String target){
  hdrl();
  content += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  ftrl();
//  content += F("<a href='/"); content += target + "'>[Back]</a><br><br>";
    server.send(200,"text/html",content);
}
//////////////////////////////////////
void File_Remove(void)
{
  auth();
  hdrl();
  if(server.arg("rmf") !="")
      {
        SPIFFS.remove(server.arg("rmf"));
      }
//
   root = SPIFFS.open("/");
 Serial.println("OK");
    hdrl();
    content +="<form method='POST'>";
  File file = root.openNextFile();
 
  while(file){
//    content += file.name();
    content += "<input type=\"radio\" id=\"rmf";
//      content += file.name();
    content += "\" name=\"rmf";
//          content += file.name();
    content += "\"";
    content += "value=\"";
          content += file.name();
    content += "\"";
    content += ">";
    content +="<label for=\"rmf\">";
          content += file.name();
    content +="</label>";
      content += "<br>";
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
  root.close();
    content += "<button type=\"submit\">Submit</button>";
    content += "</form>";
    ftrl();
//    content += "</body></html>";
//    content += "<a href='/'>[Back]</a><br><br>";
    server.send(200, "text/html",content);
}
/////////////////////////////////////////////////////////////////////////////////////////////
//страница администратора
/////////////////////////////////////////////////////////////////////////////////////////////
void padmin(void)
{
    auth();
    hdrl();
    content += "<a href='/upgrade'>[Upgrade PGM]</a><br>";
    content += "<a href='/upload'>[Update Files]</a><br>";
    content += "<a href='/fremove'>[Remove Files]</a><br>";
    ftrl();
    server.send(200, "text/html",content);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void handle_Not_Found()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
bool loadFromSPIFFS(String path,String mimeType)
{
//  String dataType = "text/html";
  String dataType = mimeType;

//  Serial.print("Requested page -> ");
//  Serial.println(path);
  if (SPIFFS.exists(path))
  {
    File dataFile = SPIFFS.open(path, "r");
    if (!dataFile)
    {
      handleNotFound();
      return false;
    }

    if (server.streamFile(dataFile, dataType) != dataFile.size())
    {
      Serial.println("Sent less data than expected!");
    }
    else
    {
      Serial.println("Page served!");
    }

    dataFile.close();
  }
  else
  {
    handle_Not_Found();
    return false;
  }
  return true;
}
////////////////////////////////////////////////////////////////////
bool load_http(String path)
{
//  Serial.print("Requested page hhtp -> ");
//  Serial.println(path);
  if (SPIFFS.exists(path))
  {
    File dataFile = SPIFFS.open(path, "r");
    if (!dataFile)
    {
      handleNotFound();
      return false;
    }
    while(dataFile.available()) {
      content += char(dataFile.read());
        }
//       Serial.println(content);
    return true;
  }
}
////////////////////////////////////////////////////////////////////
void handleFavicon(void)
{
  loadFromSPIFFS("/favicon.ico","image/png");
}
////////////////////////////////////////////////////////////////////
void handleJquery(void)
{
  loadFromSPIFFS("/jquery.js","text/javascript");
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//HEADER HTML
////////////////////////////////////////////////////////////////////
void hdr (void)
{
  content ="";
  load_http("/hdr.html");
}   
////////////////////////////////////////////////////////////////////
//HEADER LOCAL
////////////////////////////////////////////////////////////////////
void hdrl (void)
{
  content = "<html><head>";
  content += "</head>";
  content += "<body>";
}
////////////////////////////////////////////////////////////////////
//FUTER HTML
////////////////////////////////////////////////////////////////////
void ftr (void)
{
  load_http("/ftr.html");
}
////////////////////////////////////////////////////////////////////
void ftrl(void)
{
    content += "<a href='/'>[Back to clock setup]</a><br><br>";
    content += "</body></html>";
}
////////////////////////////////////////////////////////////////////
void auth (void)
{
    if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
}
////////////////////////////////////////////////////////////////////
void datch_html (String dname, String dyname, String dychar)
{
          content += "<tr><td colspan=\"2\">";
          content += hstname+".";
          content += "<input type='text' name='";
          content += dyname;
          content += "' placeholder='";          
          content += dname;
          content +="' value=";
          content += dname;
          content +="></td><td align=center  colspan=\"2\">";
          String hstdatname1=hstname + "." + dname;
            for(int i=0;i<10;i++)
            {
              if(brd[0][i]==hstdatname1)
              {
                content += brd[1][i];
                content +="</td><td align=center  colspan=\"2\"><input type='text' name='";
                content += dychar;
                content +="' placeholder='";
                content += brd[2][i];
                content +="' value=";
                content += brd[2][i];                
                content +=" </td></tr>";
              }
            }          
 }
////////////////////////////////////////////////////////////////////
