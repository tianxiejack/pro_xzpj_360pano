/*
 * MPU9250.hpp
 *
 *  Created on: 2018年9月25日
 *      Author: wj
 */

#ifndef MPU9250_HPP_
#define MPU9250_HPP_


typedef struct 
{
	short 	gyro_axi;	
	short 	gyro_ayi;	
	short 	gyro_azi;

	short 	gyro_gxi;	
	short 	gyro_gyi;	
	short 	gyro_gzi;

	short 	gyro_mxi;	
	short 	gyro_myi;	
	short 	gyro_mzi;

	double 	gyro_ax;	
	double 	gyro_ay;	
	double 	gyro_az;

	double 	gyro_gx;	
	double 	gyro_gy;	
	double 	gyro_gz;

	double 	gyro_mx;	
	double 	gyro_my;	
	double 	gyro_mz;

	double  roll;
	double pitch;
	double yaw;

	unsigned int   gyro_timestamp;
	unsigned int   gyro_pretimestamp;
	
}GYRO_MPU ;
unsigned char  calibrate(short *gx, short *gy, short *gz);
void AHRSupdate(GYRO_MPU*mpu);
void CalibrateToZero(GYRO_MPU*mpu);
void GetMpugyro(GYRO_MPU*mpu);
void setgyyawbase(double angle);
#define GYRO_NOFILTER 1
#endif /* MPU9250_HPP_ */
