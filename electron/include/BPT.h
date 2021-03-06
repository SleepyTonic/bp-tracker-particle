/**************************************************************************/
/*!
    @file     BPT.h (BP Tracker firmware)
    @author   Derek Benda
    @license  MIT (see Licence.txt)

    v0.5  - First release
*/
/**************************************************************************/

#include "application.h"
#include "BPT_Device.h"
#include "BPT_Storage.h"

#ifndef _BPT_h_
#define _BPT_h_

// Global defines go here

/* test data types for the controller in the CONTROLLER_MODE_TEST mode */
typedef enum {
  TEST_INPUT_GPS        = ((uint8_t)1), /* sets the GPS coordinate of device. format: lat,lon */
  TEST_INPUT_AUTO_GPS   = ((uint8_t)2), /* TODO: like TEST_INPUT_GPS expect the coordinate is arbitrarily chosen */
  TEST_INPUT_ACCEL_INT  = ((uint8_t)3)  /* TODO: triggers a 'wake' event on the accelerometer */
} test_input_t;


// bpt:event event codes
// when adding or modifying events also update the libraries in tools/nodejs
typedef enum {
  EVENT_STATE_CHANGE       = ((uint8_t)0x01), /* controller changed state */
  EVENT_REQUEST_GPS        = ((uint8_t)0x02), /* TODO: can this ack also come from a bpt:gps event */
  EVENT_BATTERY_LOW 	     = ((uint8_t)0x03), /* requires ACK event */
  EVENT_NO_GPS_SIGNAL      = ((uint8_t)0x04), /* data includes the age of the last known coord */
  EVENT_SOFT_PANIC         = ((uint8_t)0x05), /* not enough data to determine panic state */
  EVENT_PANIC              = ((uint8_t)0x06), /* requires ACK event */
  EVENT_PROBE_CONTROLLER   = ((uint8_t)0x07), /* this is a special event a remote device can send to probe the controller */
  EVENT_TEST               = ((uint8_t)0x08), /* when the controller is in test mode, all btp:event's use this code */
  EVENT_SERIAL_COMMAND     = ((uint8_t)0x09), /* a command was received via the serial interface */
  EVENT_STATUS_UPDATE      = ((uint8_t)0x0A), /* a status update of the device. Triggered when calling bpt:status */
  EVENT_ERROR              = ((uint8_t)0x0B), /* TODO: triggered when...  */
  EVENT_HARDWARE_FAULT     = ((uint8_t)0x0C) /* TODO: can this be trapped? */
} application_event_t;


#define NUM_CONTROLLER_STATES 13 /* NB: update this when states are added/removed */

/* the controller_state_t number that begins the internal states */
#define INTERNAL_STATES_INDEX 9

// NB: The values need to be sequential beginning at 1
typedef enum {
  /* Public states */
  STATE_OFFLINE          = ((uint8_t)0x01),
  STATE_STOPPED          = ((uint8_t)0x02),
  STATE_RESET            = ((uint8_t)0x03),
  STATE_ARMED            = ((uint8_t)0x04),
  STATE_DISARMED         = ((uint8_t)0x05),
  STATE_PANIC            = ((uint8_t)0x06),
  STATE_PAUSED           = ((uint8_t)0x07), // for testing
  STATE_RESUMED          = ((uint8_t)0x08), // for testing

  /* Private states - these should not be set from the cloud/client. See BPT_Controller::setState. */
  STATE_ACTIVATED        = ((uint8_t)0x09),
  STATE_SOFT_PANIC       = ((uint8_t)0x0A),
  STATE_ONLINE_WAIT      = ((uint8_t)0x0B),
  STATE_RESET_WAIT       = ((uint8_t)0x0C),
  STATE_SLEEP            = ((uint8_t)0x0D)
} controller_state_t;


#define NUM_CONTROLLER_MODES 3

typedef enum {
  /*
    The default mode, uses accelerometer for power saving
  */
  CONTROLLER_MODE_NORMAL        = ((uint8_t)0x01),

  /*
    Always on, disables accelerometer, GPS data is polled as
    fast as possible.
   */
  CONTROLLER_MODE_HIGH_SPEED    = ((uint8_t)0x02),

  /*
    Puts the controller into testing mode to permit mocking
    states such as the device's GPS coordinates and wake modes.

    In this mode all bpt:event event are of type EVENT_TEST //TODO: is this true?
  */
  CONTROLLER_MODE_TEST          = ((uint8_t)0x03)
} controller_mode_t;


typedef struct  {
  uint16_t hw_build_version;
  uint16_t sw_build_version;
  external_device_t *devices; //array of devices
  uint8_t device_count;
  controller_mode_t mode;
  BPT_Storage *storage;
} application_ctx_t;

typedef struct {
  float lat; // in degrees
  float lon; // in degrees
} gps_coord_t;

typedef struct {
  float x;
  float y;
  float z;
} accel_t;

// calculations taken from http://movable-type.co.uk/scripts/latlong.html
typedef enum {
  HAVERSINE_FORMULA       = ((uint8_t)0x01),
  LAW_OF_COSINES_FORMULA  = ((uint8_t)0x02),
  EQUIRECT_APPROXIMATION  = ((uint8_t)0x03),
} distance_calc_t;


class BPT {

   public:

    BPT(application_ctx_t *applicationCtx);

    ~BPT();

    template<typename T>
    bool registerProperty(application_property_t prop, T defaultValue,
                                          bool _forceDefault=false){

        return applicationCtx->storage->registerProperty(
                                prop, defaultValue, this, _forceDefault);
    }

    template<typename T>
    bool getProperty(application_property_t prop, T& destination){ //TODO
      return applicationCtx->storage->getProperty(prop, destination);
    }

    /**
      Subclasses override this method and support the properties
      they explicitly register
    **/
    virtual bool updateLocalProperty(BPT_Storage* storage,
                            application_property_t prop, String value,
                            bool persistent=true){
      return false;
    }

    application_ctx_t *applicationCtx;
};

#endif





// template<typename T>
// T getProperty2(application_property_t prop, T defaultValue){ //TODO
//
//   return defaultValue;
// }

    // template<class T> //TODO
    // bool registerProperty(application_property_t prop, BPT *owner,
    // 	T minValue, T maxValue){
    // 		return false;
    // } or use a function pointer to get the min/max?
