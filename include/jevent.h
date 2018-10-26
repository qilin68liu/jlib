#ifndef _J_EVENT_H
#define _J_EVENT_H

#include "jtypes.h"
#include "jlist.h"

typedef JList JEvent;

JEvent *j_event_new();
void    j_event_free(JEvent *event);

int j_event_register(JEvent *event, JEventHandlerFunc func, void *user_data);
int j_event_unregister(JEvent *event, JEventHandlerFunc func);
int j_event_invoke(void *object, JEvent *event);

#endif // !_J_EVENT_H
