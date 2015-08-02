#include "camera.h"
#include <stdio.h>

//#define DEBUG 
#define CAMERA_READ_ADDRESS 0x61
#define CAMERA_WRITE_ADDRESS 0x60
//#define CAMERA_ADDRESS 0x30
#define CAMERA_ID 0x96
// IRQ vector DMA2CH1 pos54/prio64 address 0x0000 0124 IRQn->DMA2_Stream1_IRQn
// DCMI vector pos78/prio85 address 0x0000 0178 IRQn->DCMI_IRQn
uint8_t camera_read_register(uint32_t regAddr);
void camera_write_register(uint32_t regAddr, uint8_t value);
void camera_reset();
// camerta register data size 8bits
void camera_init(){
  #ifdef DEBUG
  printf("camera_init()\r\n");
  #endif
  camera_init_pins();
  camera_power_down();
//  printf("call_i2c_init()\r\n");
//  printf("call camera_reset_regs()\r\n");
//  printf("call camera_reset_regs()");

//  dcmi_init();

  i2c_init(I2C1);
  
  camera_power_up();
  delay_ms(1000);
  camera_reset();
  #ifdef DEBUG
  printf("Camera init done\r\n");
  #endif
}

void camera_reset(){
  uint8_t i = 0;
  printf("\r\n%d,%X\r\n",(sizeof(OV9655_VGA)/sizeof(OV9655_VGA[0])),OV9655_VGA[0][0]);
  for(i = 0; i<0xC7;i++){
    camera_read_register(i);
  }
  printf("\r\n\r\n");
  camera_write_register(0x12,0x80);
  for(i = 0; i<0xC7;i++){
    camera_read_register(i);
  }
  printf("\r\n\r\n");
  // add VGA configuration
  for(i = 0; i<(sizeof(OV9655_VGA)/sizeof(OV9655_VGA[0]));i++){
    camera_write_register(OV9655_VGA[i][0],OV9655_VGA[i][1]);
  }
  printf("\r\n\r\n");

}
void camera_set_framerate(){
}
void camera_set_color(){
}
void camera_set_resolution(){
}
void camera_set_mode(camera_mode_t mode){
  uint32_t regAddr, value;
  camera_write_register(regAddr, value);
}
void camera_write_register(uint32_t regAddr, uint8_t value){
//  printf("camera_write_register()\r\n");
//  i2c_read_data(I2C1,((uint8_t)CAMERA_READ_ADDRESS), regAddr,1,buffer);
//  printf("writing %X at address %X\r\n",value,regAddr);
  i2c_write_data(I2C1, (uint8_t)CAMERA_WRITE_ADDRESS, regAddr, 1, value & 0xFF);// register are all 8bits on this camera
  delay_ms(50);
}


uint8_t camera_read_register(uint32_t regAddr){
  #ifdef DEBUG
  printf("camera_read_register()\r\n"); 
  #endif
  uint8_t res = 0;
    // check if register exists on camera
  if(0){//camera_registers(regAddr) == NULL){
    #ifdef DEBUG
      printf("register %d doesnt exist on this camera\r\n",regAddr);
    #endif
  }
  // register are all 8bits on this camera
  res = i2c_read_data(I2C1,((uint16_t)CAMERA_READ_ADDRESS), regAddr,1); //
//  i2c_read_data(I2C1,((uint16_t)CAMERA_ADDRESS), regAddr,1,buffer); //

  return res;
}

