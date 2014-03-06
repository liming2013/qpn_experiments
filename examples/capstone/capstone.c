/*****************************************************************************
* Model: capstone.qm
* File:  ./capstone.c
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
/* @(/2/1) .................................................................*/
#include "qpn_port.h"
#include "bsp.h"
#include "capstone.h"

#ifndef NDEBUG
Q_DEFINE_THIS_FILE
#endif

/* Pelican class declaration -----------------------------------------------*/
/* @(/1/0) .................................................................*/
typedef struct CapstoneTag {
/* protected: */
    QActive super;

/* public: */
    uint8_t bar;
} Capstone;

/* protected: */
static QState Capstone_initial(Capstone * const me);
static QState Capstone_btn1_down(Capstone * const me);
static QState Capstone_btn1_up(Capstone * const me);


/* Global objects ----------------------------------------------------------*/
Capstone AO_Capstone;     /* the single instance of the Blink active object */

/* Capstone class definition -----------------------------------------------*/
/* @(/1/2) .................................................................*/
void Capstone_ctor(void) {
    QActive_ctor(&AO_Capstone.super, Q_STATE_CAST(&Capstone_initial));
}
/* @(/1/0) .................................................................*/
/* @(/1/0/1) ...............................................................*/
/* @(/1/0/1/0) */
static QState Capstone_initial(Capstone * const me) {
    return Q_TRAN(&Capstone_btn1_up);
}
/* @(/1/0/1/1) .............................................................*/
static QState Capstone_btn1_down(Capstone * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1) */
        case Q_ENTRY_SIG: {
            BSP_signalLeds(DOWN);
            status_ = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/0) */
        case BTN1_UP_SIG: {
            status_ = Q_TRAN(&Capstone_btn1_up);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/* @(/1/0/1/2) .............................................................*/
static QState Capstone_btn1_up(Capstone * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/2) */
        case Q_ENTRY_SIG: {
            BSP_signalLeds(UP);
            BSP_progressBar(me->bar, 40, 8);
            me->bar++;
            status_ = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/2/0) */
        case BTN1_DOWN_SIG: {
            status_ = Q_TRAN(&Capstone_btn1_down);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

