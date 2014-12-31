/*****************************************************************************
* Model: dcf77.qm
* File:  ./phase_detector.c
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
/*${AOs::.::phase_detector.c} ..............................................*/
#include "qpn_port.h"
#include "bsp.h"
#include "phase_detector.h"

/*Q_DEFINE_THIS_FILE*/

#define PAUSE_MS             (BSP_TICKS_PER_SEC / 10)
#define BINNING_PERIOD_MS    (BSP_TICKS_PER_SEC / 10)
#define PHASE_UPDATE_MS      (BSP_TICKS_PER_SEC * 5)

/* PhaseDetector class declaration -----------------------------------------------*/
/*${AOs::PhaseDetector} ....................................................*/
typedef struct PhaseDetector {
/* protected: */
    QMActive super;
} PhaseDetector;

/* protected: */
static QState PhaseDetector_initial(PhaseDetector * const me);
static QState PhaseDetector_NOT_LOCKED  (PhaseDetector * const me);
static QState PhaseDetector_NOT_LOCKED_e(PhaseDetector * const me);
static QState PhaseDetector_NOT_LOCKED_x(PhaseDetector * const me);
static QMState const PhaseDetector_NOT_LOCKED_s = {
    (QMState const *)0, /* superstate (top) */
    Q_STATE_CAST(&PhaseDetector_NOT_LOCKED),
    Q_ACTION_CAST(&PhaseDetector_NOT_LOCKED_e),
    Q_ACTION_CAST(&PhaseDetector_NOT_LOCKED_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState PhaseDetector_LOCKED  (PhaseDetector * const me);
static QState PhaseDetector_LOCKED_e(PhaseDetector * const me);
static QState PhaseDetector_LOCKED_x(PhaseDetector * const me);
static QMState const PhaseDetector_LOCKED_s = {
    (QMState const *)0, /* superstate (top) */
    Q_STATE_CAST(&PhaseDetector_LOCKED),
    Q_ACTION_CAST(&PhaseDetector_LOCKED_e),
    Q_ACTION_CAST(&PhaseDetector_LOCKED_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};


/* Global objects ----------------------------------------------------------*/
PhaseDetector AO_PhaseDetector;

/* Blink class definition --------------------------------------------------*/
/*${AOs::PhaseDetector_ct~} ................................................*/
void PhaseDetector_ctor(void) {
    QMActive_ctor(&AO_PhaseDetector.super, Q_STATE_CAST(&PhaseDetector_initial));
}
/*${AOs::PhaseDetector} ....................................................*/
/*${AOs::PhaseDetector::SM} ................................................*/
static QState PhaseDetector_initial(PhaseDetector * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { /* transition-action table */
        &PhaseDetector_NOT_LOCKED_s, /* target state */
        {
            Q_ACTION_CAST(&PhaseDetector_NOT_LOCKED_e), /* entry */
            Q_ACTION_CAST(0) /* zero terminator */
        }
    };
    /* ${AOs::PhaseDetector::SM::initial} */
    return QM_TRAN_INIT(&tatbl_);
}
/*${AOs::PhaseDetector::SM::NOT_LOCKED} ....................................*/
/* ${AOs::PhaseDetector::SM::NOT_LOCKED} */
static QState PhaseDetector_NOT_LOCKED_e(PhaseDetector * const me) {
    QActive_arm((QActive *)me, BINNING_PERIOD_MS);
    return QM_ENTRY(&PhaseDetector_NOT_LOCKED_s);
}
/* ${AOs::PhaseDetector::SM::NOT_LOCKED} */
static QState PhaseDetector_NOT_LOCKED_x(PhaseDetector * const me) {
    QActive_disarm((QActive *)me);
    return QM_EXIT(&PhaseDetector_NOT_LOCKED_s);
}
/* ${AOs::PhaseDetector::SM::NOT_LOCKED} */
static QState PhaseDetector_NOT_LOCKED(PhaseDetector * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::PhaseDetector::SM::NOT_LOCKED::SAMPLE_READY} */
        case SAMPLE_READY_SIG: {
            BSP_binning(Q_PAR(me));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::PhaseDetector::SM::NOT_LOCKED::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            /* ${AOs::PhaseDetector::SM::NOT_LOCKED::Q_TIMEOUT::[BSP_convolution~} */
            if (BSP_convolution()) {
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { /* transition-action table */
                    &PhaseDetector_LOCKED_s, /* target state */
                    {
                        Q_ACTION_CAST(&PhaseDetector_NOT_LOCKED_x), /* exit */
                        Q_ACTION_CAST(&PhaseDetector_LOCKED_e), /* entry */
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            /* ${AOs::PhaseDetector::SM::NOT_LOCKED::Q_TIMEOUT::[else]} */
            else {
                QActive_arm((QActive *)me, BINNING_PERIOD_MS);
                status_ = QM_HANDLED();
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
/*${AOs::PhaseDetector::SM::LOCKED} ........................................*/
/* ${AOs::PhaseDetector::SM::LOCKED} */
static QState PhaseDetector_LOCKED_e(PhaseDetector * const me) {
    QActive_arm((QActive *)me, PHASE_UPDATE_MS);
    return QM_ENTRY(&PhaseDetector_LOCKED_s);
}
/* ${AOs::PhaseDetector::SM::LOCKED} */
static QState PhaseDetector_LOCKED_x(PhaseDetector * const me) {
    QActive_disarm((QActive *)me);
    return QM_EXIT(&PhaseDetector_LOCKED_s);
}
/* ${AOs::PhaseDetector::SM::LOCKED} */
static QState PhaseDetector_LOCKED(PhaseDetector * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::PhaseDetector::SM::LOCKED::SAMPLE_READY} */
        case SAMPLE_READY_SIG: {
            BSP_decoding(Q_PAR(me));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::PhaseDetector::SM::LOCKED::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            BSP_binning(Q_PAR(me));
            /* ${AOs::PhaseDetector::SM::LOCKED::Q_TIMEOUT::[BSP_convolution~} */
            if (BSP_convolution()) {
                QActive_arm((QActive *)me, PHASE_UPDATE_MS);
                status_ = QM_HANDLED();
            }
            /* ${AOs::PhaseDetector::SM::LOCKED::Q_TIMEOUT::[else]} */
            else {
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { /* transition-action table */
                    &PhaseDetector_NOT_LOCKED_s, /* target state */
                    {
                        Q_ACTION_CAST(&PhaseDetector_LOCKED_x), /* exit */
                        Q_ACTION_CAST(&PhaseDetector_NOT_LOCKED_e), /* entry */
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                status_ = QM_TRAN(&tatbl_);
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

