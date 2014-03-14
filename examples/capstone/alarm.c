/*****************************************************************************
* Model: capstone.qm
* File:  ./alarm.c
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
/* @(/2/4) .................................................................*/
#include "qpn_port.h"
#include "bsp.h"
#include "capstone.h"
#include "alarm.h"

#ifndef NDEBUG
Q_DEFINE_THIS_FILE
#endif

int getBitPosition(uint8_t b) {
    uint8_t i;

    for(i=7; i>=0;i--)
    {
        if(b & _BV(i))
            return i;
    }
    return -1;

}

/* Pelican class declaration -----------------------------------------------*/
/* @(/1/1) .................................................................*/
typedef struct AlarmMgrTag {
/* protected: */
    QMActive super;

/* private: */
    uint8_t curr_alarm;
    uint8_t active_alarms;
    uint8_t count;
} AlarmMgr;

/* protected: */
static QState AlarmMgr_initial(AlarmMgr * const me);
static QState AlarmMgr_on  (AlarmMgr * const me);
static QState AlarmMgr_on_i(AlarmMgr * const me);
static QMState const AlarmMgr_on_s = {
    (QMState const *)0,
    Q_STATE_CAST(&AlarmMgr_on),
    Q_ACTION_CAST(0)
};
static QState AlarmMgr_silent  (AlarmMgr * const me);
static QState AlarmMgr_silent_e(AlarmMgr * const me);
static QMState const AlarmMgr_silent_s = {
    &AlarmMgr_on_s,
    Q_STATE_CAST(&AlarmMgr_silent),
    Q_ACTION_CAST(0)
};
static QState AlarmMgr_playing  (AlarmMgr * const me);
static QState AlarmMgr_playing_e(AlarmMgr * const me);
static QState AlarmMgr_playing_i(AlarmMgr * const me);
static QMState const AlarmMgr_playing_s = {
    &AlarmMgr_on_s,
    Q_STATE_CAST(&AlarmMgr_playing),
    Q_ACTION_CAST(0)
};
static QState AlarmMgr_beep  (AlarmMgr * const me);
static QState AlarmMgr_beep_e(AlarmMgr * const me);
static QState AlarmMgr_beep_x(AlarmMgr * const me);
static QMState const AlarmMgr_beep_s = {
    &AlarmMgr_playing_s,
    Q_STATE_CAST(&AlarmMgr_beep),
    Q_ACTION_CAST(&AlarmMgr_beep_x)
};
static QState AlarmMgr_off  (AlarmMgr * const me);
static QState AlarmMgr_off_e(AlarmMgr * const me);
static QState AlarmMgr_off_x(AlarmMgr * const me);
static QMState const AlarmMgr_off_s = {
    &AlarmMgr_playing_s,
    Q_STATE_CAST(&AlarmMgr_off),
    Q_ACTION_CAST(&AlarmMgr_off_x)
};
static QState AlarmMgr_long_off  (AlarmMgr * const me);
static QState AlarmMgr_long_off_e(AlarmMgr * const me);
static QState AlarmMgr_long_off_x(AlarmMgr * const me);
static QMState const AlarmMgr_long_off_s = {
    &AlarmMgr_playing_s,
    Q_STATE_CAST(&AlarmMgr_long_off),
    Q_ACTION_CAST(&AlarmMgr_long_off_x)
};


/* Global objects ----------------------------------------------------------*/
AlarmMgr AO_AlarmMgr;

/* Capstone class definition -----------------------------------------------*/
/* @(/1/5) .................................................................*/
void AlarmMgr_ctor(void) {
    QMActive_ctor(&AO_AlarmMgr.super, Q_STATE_CAST(&AlarmMgr_initial));
}
/* @(/1/1) .................................................................*/
/* @(/1/1/3) ...............................................................*/
/* @(/1/1/3/0) */
static QState AlarmMgr_initial(AlarmMgr * const me) {
    static QActionHandler const act_[] = {
        Q_ACTION_CAST(&AlarmMgr_on_i),
        Q_ACTION_CAST(0)
    };
    return QM_INITIAL(&AlarmMgr_on_s, &act_[0]);
}
/* @(/1/1/3/1) .............................................................*/
static QState AlarmMgr_on_i(AlarmMgr * const me) {
    static QActionHandler const act_[] = {
        Q_ACTION_CAST(&AlarmMgr_silent_e),
        Q_ACTION_CAST(0)
    };
    return QM_INITIAL(&AlarmMgr_silent_s, &act_[0]);
}
static QState AlarmMgr_on(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/3/1/1) */
        case ALARM_REQUEST_SIG: {
            uint8_t alarm_type = (uint8_t)Q_PAR(me);
            /* @(/1/1/3/1/1/0) */
            if (me->active_alarms & _BV(alarm_type)) {
                status_ = QM_HANDLED();
            }
            /* @(/1/1/3/1/1/1) */
            else {
                static QActionHandler const act_[] = {
                    Q_ACTION_CAST(&AlarmMgr_playing_e),
                    Q_ACTION_CAST(&AlarmMgr_playing_i),
                    Q_ACTION_CAST(0)
                };
                me->active_alarms |= _BV(alarm_type);
                me->curr_alarm = getBitPosition(me->active_alarms);
                status_ = QM_TRAN(&AlarmMgr_playing_s, &act_[0]);
            }
            break;
        }
        /* @(/1/1/3/1/2) */
        case ALARM_SILENCE_SIG: {
            uint8_t alarm_type = (uint8_t)Q_PAR(me);
            /* @(/1/1/3/1/2/0) */
            if (alarm_type == ALL_ALARMS) {
                static QActionHandler const act_[] = {
                    Q_ACTION_CAST(&AlarmMgr_silent_e),
                    Q_ACTION_CAST(0)
                };
                me->active_alarms = 0;
                status_ = QM_TRAN(&AlarmMgr_silent_s, &act_[0]);
            }
            /* @(/1/1/3/1/2/1) */
            else {
                me->active_alarms &= ~_BV(alarm_type);
                /* @(/1/1/3/1/2/1/0) */
                if (me->active_alarms == 0) {
                    static QActionHandler const act_[] = {
                        Q_ACTION_CAST(&AlarmMgr_silent_e),
                        Q_ACTION_CAST(0)
                    };
                    status_ = QM_TRAN(&AlarmMgr_silent_s, &act_[0]);
                }
                /* @(/1/1/3/1/2/1/1) */
                else {
                    alarm_type = getBitPosition(me->active_alarms);
                    /* @(/1/1/3/1/2/1/1/0) */
                    if (me->curr_alarm != alarm_type) {
                        static QActionHandler const act_[] = {
                            Q_ACTION_CAST(&AlarmMgr_playing_e),
                            Q_ACTION_CAST(&AlarmMgr_playing_i),
                            Q_ACTION_CAST(0)
                        };
                        me->curr_alarm = alarm_type;
                        status_ = QM_TRAN(&AlarmMgr_playing_s, &act_[0]);
                    }
                    /* @(/1/1/3/1/2/1/1/1) */
                    else {
                        status_ = QM_HANDLED();
                    }
                }
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/3/1/3) ...........................................................*/
static QState AlarmMgr_silent_e(AlarmMgr * const me) {
    BSP_ledOff(BUZZER);
    me->curr_alarm = 0;
    me->active_alarms = 0;
    return QM_ENTRY(&AlarmMgr_silent_s);
}
static QState AlarmMgr_silent(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/3/1/4) ...........................................................*/
static QState AlarmMgr_playing_e(AlarmMgr * const me) {
    me->count = 0;
    return QM_ENTRY(&AlarmMgr_playing_s);
}
static QState AlarmMgr_playing_i(AlarmMgr * const me) {
    static QActionHandler const act_[] = {
        Q_ACTION_CAST(&AlarmMgr_beep_e),
        Q_ACTION_CAST(0)
    };
    return QM_INITIAL(&AlarmMgr_beep_s, &act_[0]);
}
static QState AlarmMgr_playing(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/3/1/4/1) .........................................................*/
static QState AlarmMgr_beep_e(AlarmMgr * const me) {
    BSP_ledOn(BUZZER);
    QActive_arm((QActive *)me, SHORT_PULSE);
    return QM_ENTRY(&AlarmMgr_beep_s);
}
static QState AlarmMgr_beep_x(AlarmMgr * const me) {
    QActive_disarm((QActive *)me);
    return QM_EXIT(&AlarmMgr_beep_s);
}
static QState AlarmMgr_beep(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/3/1/4/1/0) */
        case Q_TIMEOUT_SIG: {
            ++me->count;
            /* @(/1/1/3/1/4/1/0/0) */
            if (me->count == me->curr_alarm) {
                static QActionHandler const act_[] = {
                    Q_ACTION_CAST(&AlarmMgr_beep_x),
                    Q_ACTION_CAST(&AlarmMgr_long_off_e),
                    Q_ACTION_CAST(0)
                };
                status_ = QM_TRAN(&AlarmMgr_long_off_s, &act_[0]);
            }
            /* @(/1/1/3/1/4/1/0/1) */
            else {
                static QActionHandler const act_[] = {
                    Q_ACTION_CAST(&AlarmMgr_beep_x),
                    Q_ACTION_CAST(&AlarmMgr_off_e),
                    Q_ACTION_CAST(0)
                };
                status_ = QM_TRAN(&AlarmMgr_off_s, &act_[0]);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/3/1/4/2) .........................................................*/
static QState AlarmMgr_off_e(AlarmMgr * const me) {
    BSP_ledOff(BUZZER);
    QActive_arm((QActive *)me, SHORT_PULSE);
    return QM_ENTRY(&AlarmMgr_off_s);
}
static QState AlarmMgr_off_x(AlarmMgr * const me) {
    QActive_disarm((QActive *)me);
    return QM_EXIT(&AlarmMgr_off_s);
}
static QState AlarmMgr_off(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/3/1/4/2/0) */
        case Q_TIMEOUT_SIG: {
            static QActionHandler const act_[] = {
                Q_ACTION_CAST(&AlarmMgr_off_x),
                Q_ACTION_CAST(&AlarmMgr_beep_e),
                Q_ACTION_CAST(0)
            };
            status_ = QM_TRAN(&AlarmMgr_beep_s, &act_[0]);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/3/1/4/3) .........................................................*/
static QState AlarmMgr_long_off_e(AlarmMgr * const me) {
    BSP_ledOff(BUZZER);
    QActive_arm((QActive *)me, LONG_PULSE);
    return QM_ENTRY(&AlarmMgr_long_off_s);
}
static QState AlarmMgr_long_off_x(AlarmMgr * const me) {
    QActive_disarm((QActive *)me);
    return QM_EXIT(&AlarmMgr_long_off_s);
}
static QState AlarmMgr_long_off(AlarmMgr * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/3/1/4/3/0) */
        case Q_TIMEOUT_SIG: {
            static QActionHandler const act_[] = {
                Q_ACTION_CAST(&AlarmMgr_long_off_x),
                Q_ACTION_CAST(&AlarmMgr_beep_e),
                Q_ACTION_CAST(0)
            };
            me->count = 0;
            status_ = QM_TRAN(&AlarmMgr_beep_s, &act_[0]);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

