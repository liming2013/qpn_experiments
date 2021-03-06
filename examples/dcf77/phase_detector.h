/*****************************************************************************
* Model: dcf77.qm
* File:  ./phase_detector.h
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${AOs::.::phase_detector.h} ..............................................*/
#ifndef __PHASE_DETECTOR_H__
#define __PHASE_DETECTOR_H__

enum PhaseDetectorSignals {
    SAMPLE_READY_SIG = Q_USER_SIG,
    PHASE_UPDATE_SIG,
    DCF_DATA_SIG,
    LED_PULSE_SIG,
    BUTTON_PRESSED_SIG
};

// active objects ................................................*/
/*${AOs::PhaseDetector_ct~} ................................................*/
void PhaseDetector_ctor(void);


extern struct PhaseDetector AO_PhaseDetector;

#endif /* __PHASE_DETECTOR_H__ */
