/*
 * plantformcontrl.hpp
 *
 *  Created on: 2018年10月25日
 *      Author: wj
 */

#ifndef PLANTFORMCONTRL_HPP_
#define PLANTFORMCONTRL_HPP_

void plantformcontrlinit();
void setpanoscan();
void setpanoscanstop();
void setpanopanpos(double value);
void setpanotitlepos(double value);
void getpanopanpos();
void getpanotitlepos();
double getpanopan();
double getpanotitle();
void initptzpos(double pan,double title);
void setplantformcalibration(int flag);
int  getplantformcalibration();
void setpanoantiscan();

#endif /* PLANTFORMCONTRL_HPP_ */
