#include "CAN_driver.h"

#define PID_ECAN_BITRATE 137
#define BIT_RATE 4 
/*******************
* 1 : 50k  
* 2 : 100k 
* 3 : 250k  
* 4 : 500k  
* 5 : 1M
***단위: Bits/s *****/

#define PID_ENC_PPR 156

int16_t Enc_Pulse=16384; //엔코더분해능(PPR). 메뉴얼에는 512~2048 범위에서 입력하라고 되어있음. 확인필요(PID 156)
 
uint8_t set_CanBitrate[8]={PID_ECAN_BITRATE,0xaa,BIT_RATE,0,0,0,0,0}; 
uint8_t set_Encoder[8]={PID_ENC_PPR,Int2LHByte(Enc_Pulse).low,Int2LHByte(Enc_Pulse).high,0,0,0,0,0};



void setup() {
 
  // put your setup code here, to run once:

  CAN_initialize();
  int k=0;
  
  for(k=0;k<10;k++)
  {
    CAN_write(set_Encoder);

    uint8_t* check_enc_arr=CAN_read(PID_ENC_PPR);
    int16_t Enc_PPR=LHByte2Int16(check_enc_arr[1],check_enc_arr[2]);
    
    if(Enc_PPR==Enc_Pulse)
      break;
    if(k==9)
    {
      //에러메시지(엔코더 펄스수 설정 실패)
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
