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
#define PID_INV_SIGN_CMD 16

int16_t Enc_Pulse=16384; //엔코더분해능(PPR). 메뉴얼에는 512~2048 범위에서 입력하라고 되어있음. 확인필요(PID 156)
 
uint8_t set_CanBitrate[8]={PID_ECAN_BITRATE,0xaa,BIT_RATE,0,0,0,0,0}; 
uint8_t set_Encoder[8]={PID_ENC_PPR,Int2LHByte(Enc_Pulse).low,Int2LHByte(Enc_Pulse).high,0,0,0,0,0};
uint8_t inverse_sign[8]={PID_INV_SIGN_CMD,1,0,0,0,0,0,0}; //모터1번(오른쪽모터)의 속도, 위치명령 반전



void setup() {
 
  // put your setup code here, to run once:
  Serial.begin(115200);

  CAN_initialize();
  int k=0;
  uint8_t* check_arr=NULL;

  
  for(k=0;k<10;k++) //엔코더 PPR 설정
  {
    CAN_write(set_Encoder);

    check_arr=CAN_read(PID_ENC_PPR);
    int16_t Enc_PPR=LHByte2Int16(check_arr[1],check_arr[2]);
    
    if(Enc_PPR==Enc_Pulse)
      break;
    if(k==9)
    {
      //에러메시지(엔코더 펄스수 설정 실패)
      Serial.println("Enoder PPR Setting FALSE");
    }
  }

  delay(500);
  
  for(k=0;k<10;k++) //모터1(오른쪽모터)신호 반전 설정
  {
    CAN_write(inverse_sign);

    check_arr=CAN_read(PID_INV_SIGN_CMD);
    int16_t sign_inv=check_arr[1];
    
    if(sign_inv==1)   // 1:신호반전 o, 0:신호반전 x
      break;
    if(k==9)
    {
      //에러메시지(모터1 신호반전 설정 실패)
      Serial.println("Motor1 Sign Inverse Setting FALSE");
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
