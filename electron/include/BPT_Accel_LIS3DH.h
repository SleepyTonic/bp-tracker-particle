#include "BPT_Device_Impl.h"
#include "Adafruit_LIS3DH.h"

#ifndef BPT_Accel_LIS3DH_h
#define BPT_Accel_LIS3DH_h

#define DEFAULT_PROP_ACCEL_THRESHOLD ((uint8_t)16)

class BPT_Accel_LIS3DH: public BPT_Device_Impl {

  public:

    BPT_Accel_LIS3DH(application_ctx_t *appCtx);

    ~BPT_Accel_LIS3DH();

    void init();

    bool enable(void);

    bool disable(void);

    bool reset(void);

    void shutdown(void);

    bool update(void);

    // Override
    bool getStatus(uint16_t mask);

    // override
    int getIntData(void *accel, int size);

    // override
    virtual bool updateLocalProperty(BPT_Storage* storage,
        application_property_t prop, String value, bool persistent);


  protected:

    static Adafruit_LIS3DH driver;

  private:

    // if this is enabled, then the module will not turn on the GPS
    bool simulationMode;
    external_device_t *device;
};

#endif
