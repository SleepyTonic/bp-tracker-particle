#include "BPT_Module.h"

#ifndef BPT_GPS_h
#define BPT_GPS_h

#define DEFAULT_DISTANCE_CALC = ((uint8_t)0x00); // HAVERSINE_FORMULA

class BPT_GPS: public BPT_Module {

  public:

    BPT_GPS(application_ctx_t *appCtx);

    ~BPT_GPS();

    virtual void init(void);

    virtual void init(external_device_t *device);

    virtual bool enable(void);

    virtual bool disable(void);

    virtual bool reset(void);

    // returns true if module has a GPS fix and coords have been updated
    virtual bool getGPSCoord(gps_coord_t *gpsCoord);

    float getDistanceTo(gps_coord_t *gpsCoord);

    float getDistanceTo(gps_coord_t *gpsCoord, distance_calc_t formula);

};

#endif
