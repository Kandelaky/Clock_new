  //////////////////////////////////////////////////////////////////////////////////////////
  //Сервер приема броадкастов
  //////////////////////////////////////////////////////////////////////////////////////////
void serv_broad(void)
{
  if (udp.listen(port)) 
  {
    udp.onPacket([](AsyncUDPPacket packet) {
      if (packet.isBroadcast()) {
        for (int i = 0; i < packet.length(); i++) 
        {
          packetdata += (char)*(packet.data() + i);
        }
        Serial.print("Data: ");
        Serial.println(packetdata);
  char *cstr = new char[packetdata.length() + 1];
        strcpy(cstr, packetdata.c_str());
        tmp_string = strtok(cstr, "|");
        for (int i = 0; i < 4; i++) {
          nam[i] = String(tmp_string);
          //              Serial.println(nam[i]);
          tmp_string = strtok(NULL, "|");
        }
        t_name = nam[0] + "." + nam[1];
        t_data = nam[2];
        t_char = nam[3];
        Serial.println(t_name);
        packetdata = "";
        delete [] cstr;
        wr_broad(t_name,t_data,t_char);
///////    
///////    
      }
    });
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
//запись принятого бродкаста
//////////////////////////////////////////////////////////////////////////////////////////
void wr_broad( String t_name,String t_data,String t_char)
{
        brdf=0;
        for(int i=0;i<10;i++)
        {
          if(brd[0][i]==t_name)       //обновление данных при совпадении имен
          {
           brdn[i]=0;
           brd[1][i]=t_data;
           brdf=1;                    //устанавливаем флаг о том, что запись сделана
          }          
        }
        if(brdf==0)                   //новая запись
        {
          for(int i=0;i<10;i++)
          {
            if(brdn[i]==3)
            {
              brd[0][i]=t_name;
              brd[1][i]=t_data;
              brd[2][i]=t_char;
              brdn[i]=0;
              break;
            }            
          }
        }        
}        
//////////////////////////////////////////////////////////////////////////////////////////
//процедура посылки броадкаста от датчика
//////////////////////////////////////////////////////////////////////////////////////////
void broadcast(String hstname, String datname, float datdat, String datchar) {
//  char broadcast_[10];
  sprintf(broadcast_, "%.2f", datdat);
  String broadcast = "|" + hstname + "|" + datname + "|" + broadcast_ + "|" + datchar + "|";
  udp.broadcastTo(broadcast.c_str(), port);
  String hstdatname=hstname + "." + datname;
  wr_broad(hstdatname,broadcast_,datchar);          //запись своего броадкаста в локальную таблицу
}
//////////////////////////////////////////////////////////////////////////////////////////
//процедура устаревания броадкастов
//////////////////////////////////////////////////////////////////////////////////////////
void obs_broadcast (void)
{
  for(int i=0;i<10;i++)
  {
    if(brdn[i]<3)
    {
      brdn[i]++;
    }    
  }  
#ifdef DEBUG2  
  prn_broadcast_t();
#endif  
}
//////////////////////////////////////////////////////////////////////////////////////////
//процедура вывода таблицы собранных броадкастов
//////////////////////////////////////////////////////////////////////////////////////////
void prn_broadcast_t (void)
{
       for(int i=0;i<10;i++)
      {
        Serial.print(brd[0][i]);
        Serial.print(" ");
        Serial.print(brd[1][i]);
        Serial.print(" ");
        Serial.print(brdn[i]);
        Serial.println();
      }       
}
//////////////////////////////////////////////////////////////////////////////////////////