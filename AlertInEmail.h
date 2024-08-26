#ifndef ALERT_IN_EMAIL_H
#define ALERT_IN_EMAIL_H

enum breach_type {
    NORMAL,
    LOW_BREACH,
    HIGH_BREACH
};

void send_to_email(breach_type breach);

#endif // ALERT_IN_EMAIL_H
