#include <regx52.h>
#include "MPU6050.h"
void main(){
	short gx, gy, gz;
	Uart_Init(0xFA); //11.0592 9600
	if(mpu_init()){
		printf("error");
	}else{
		printf("ok");
	}

	while(1){
		delay1ms(1000);
		if(mpu_get_gyro(&gx, &gy, &gz)==0){
			printf("%d,%d,%d\n", gx, gy, gz);
		}else{
			printf(".");
		}
		
	}
}