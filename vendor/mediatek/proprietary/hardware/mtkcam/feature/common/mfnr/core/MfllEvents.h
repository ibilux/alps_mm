#ifndef __MFLLEVENTS_H__
#define __MFLLEVENTS_H__

#include "IMfllEvents.h"

#include <utils/RefBase.h> // android::sp
#include <vector> // std::vector

using android::sp;

namespace mfll {

class MfllEvents : public IMfllEvents {
public:
    MfllEvents(void);
    virtual ~MfllEvents(void);

private:
    std::vector< std::vector< sp<IMfllEventListener> > > m_vvRegisteredListeners;

/* implementations */
public:
    virtual void registerEventListener(const enum EventType &t, const sp<IMfllEventListener> &event);
    virtual void removeEventListener(const enum EventType &t, const IMfllEventListener *event);
    virtual void onEvent(enum EventType t, MfllEventStatus_t &status, void *mfllCore, void *param1 = NULL, void *param2 = NULL);
    virtual void doneEvent(enum EventType t, MfllEventStatus_t &status, void *mfllCore, void *param1 = NULL, void *param2 = NULL);
}; /* class MfllEvents */

/* EventItr typedef */
typedef std::vector< sp<IMfllEventListener> >::iterator EventItr;

}; /* namespace mfll */
#endif /* __MFLLEVENTS_H__ */
