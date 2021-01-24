#include <stdio.h>
#include <math.h>

#define IMMOBILITY_THRSHLD   200
#define STORAGE_SIZE         1
#define X_AXIS_START_DATA          14
#define X_AXIS_END_DATA		   15
#define Y_AXIS_START_DATA          16
#define Y_AXIS_END_DATA            17
#define Z_AXIS_START_DATA          18
#define Z_AXIS_END_DATA            19

#define BEACON_FRAME		   26
#define IBEACON_FRAME              24 


typedef struct store
{
  int x;
  int y;
  int z;
  
}storage_t;


unsigned int beacon_data_frame[BEACON_FRAME] = {0x02, 0x01, 0x06, 0x03, 0x03, 0xE1, 0xFF, 0x12, 0x16, 0xE1, 0xFF, 0xA1, 0x03, 0x64, 0xFF, 0xF4, 0x00, 0x0F, 0xFF, 0x00, 0x37, 0x72, 0xA3, 0x3F, 0x23, 0xAC};
unsigned int ibeacon_data_frame[IBEACON_FRAME] = {0};
storage_t store_buf[STORAGE_SIZE] = {0};

volatile char byte_count;

/*Validate mobility data*/

float get_mobility_data()
{
	float xyz = 0;

	printf("%X %X %X", store_buf[0].x, store_buf[0].y, store_buf[0].z);

	xyz = (float)(store_buf[0].x*store_buf[0].x) + (float)(store_buf[0].y*store_buf[0].y) + (float)(store_buf[0].z*store_buf[0].z);
	xyz = sqrtf(xyz);
	xyz = xyz/4;

	printf("%f\r\n", xyz);

	return xyz;
}


/*Check device is in mobile or immobile*/

void make_decision_on_data_frame()
{
	float motion_data = get_mobility_data();

	if(motion_data < IMMOBILITY_THRSHLD)
	  printf("Tag is stationary\r\n");
	else
	  printf("Tag is moving\r\n");

}


/* Data decoding for beacon frame*/

void BLE_adv_data_search(unsigned int data_decode[], size_t size)
{
	for (size_t i = 0; i < size; ++i) 
	{
		 unsigned char byte = (*(unsigned char*)data_decode + i);
		 byte_count++;

		 if(size == BEACON_FRAME)
		 {
			if(i == X_AXIS_START_DATA)
			{
			  store_buf[0].x = byte;	//Combining as a 16bit data
			  store_buf[0].x <<= 8;
			}
			else if(i == X_AXIS_END_DATA)
			{
			  store_buf[0].x = byte;
			}
			else if(i == Y_AXIS_START_DATA)
			{
			  store_buf[0].y = byte;
			  store_buf[0].y <<= 8;
			}
			else if(i == Y_AXIS_END_DATA)
			{
			  store_buf[0].y = byte;
			}
			else if(i == Z_AXIS_START_DATA)
			{
			  store_buf[0].z = byte;
			  store_buf[0].z <<= 8;
			}
			else if(i == Z_AXIS_END_DATA)
			{
			  store_buf[0].z = byte;
			}
		}
        }
}

void main()
{
	printf("*****Code Begins*****\r\n");
	BLE_adv_data_search(beacon_data_frame, BEACON_FRAME);
	make_decision_on_data_frame();
}




