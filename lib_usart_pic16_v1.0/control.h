/* 
 * File:   control.h
 * Author: alansalinas
 *
 * Created on April 9, 2015, 1:01 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_H */

  void rotationControl(char sensor_set);
  void positionControlPing(short setpoint);
  void positionControlHC(short setpoint);
    void positionControlPieza(short setpoint);

    void Control(char sensor_set);
  