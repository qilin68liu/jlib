#include <stdlib.h>
#include "jevent.h"

typedef struct _j_event_handler JEventHandler;

struct _j_event_handler {
    JEventHandlerFunc func;
    void *user_data;
};

static JEventHandler *new_handler(JEventHandlerFunc func, void *user_data);
static int func_eq(JEventHandler *a, JEventHandlerFunc func);
static void call(JEventHandler *handler, void *user_data);

JEvent *j_event_new() {
    return (JEvent *)j_array_new();
}

void j_event_free(JEvent *event) {
    if (event == NULL) {
        return;
    }

    j_array_free_deep(event, (JFreeFunc)free);
}

int j_event_register(JEvent *event, JEventHandlerFunc func, void *user_data) {
    if (event == NULL || func == NULL) {
        return 0;
    }

    JEventHandler * handler = new_handler(func, user_data);
    return j_array_add(event, handler);
}

int j_event_unregister(JEvent *event, JEventHandlerFunc func) {
    if (event == NULL || func == NULL) {
        return 0;
    }

    return j_array_remove_deep_if(event, (JPredicateFunc)func_eq, func, (JFreeFunc)free);
}

int j_event_invoke(void *object, JEvent *event) {
    if (object == NULL || event == NULL) {
        return 0;
    }

    j_array_foreach(event, (JFunc)call, object);
    return 1;
}

static JEventHandler *new_handler(JEventHandlerFunc func, void *user_data) {
    JEventHandler *handler = (JEventHandler *)malloc(sizeof(JEventHandler));
    handler->func = func;
    handler->user_data = user_data;
    return handler;
}

static int func_eq(JEventHandler *a, JEventHandlerFunc func) {
    return a->func == func;
}

static void call(JEventHandler *handler, void *user_data) {
    handler->func(user_data, handler->user_data);
}
