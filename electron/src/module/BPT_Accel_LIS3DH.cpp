/* Logic borrowed from particle.io's AssetTracker library
  	(https://github.com/spark/AssetTracker)
*/
#include "BPT_Accel_LIS3DH.h"

BPT_Accel_LIS3DH::BPT_Accel_LIS3DH(application_ctx_t *applicationCtx)
  : BPT_Accel(applicationCtx){ }

BPT_Accel_LIS3DH::~BPT_Accel_LIS3DH(){ }

bool BPT_Accel_LIS3DH::enable(void){

  if(!getStatus(MOD_STATUS_ONLINE)){
    return false;
  }

  if(getStatus(MOD_STATUS_ENABLED)){
    return true;
  }

  //TODO

  return true;
}

bool BPT_Accel_LIS3DH::disable(void){

  // TODO: complete logic

  clearStatus(MOD_STATUS_ENABLED);
  return false;
}

bool BPT_Accel_LIS3DH::update(void){

  return true;
}

bool BPT_Accel_LIS3DH::reset(void){
  return false;
}

void BPT_Accel_LIS3DH::init(external_device_t *dev){ //TODO
  device = dev;

  if(device->type != DEVICE_TYPE_ACCEL){
    const char *m = "Device type not supported";
    setStatus(MOD_STATUS_ERROR, m);
    return;
  }
  init();
}

void BPT_Accel_LIS3DH::init(void){

  if(device == 0){
    const char *m = "Cannot call init without an external_device_t";
    setStatus(MOD_STATUS_ERROR, m);
    return;
  }

  registerProperty(PROP_ACCEL_THRESHOLD, this);

  driver.begin(LIS3DH_DEFAULT_ADDRESS);

  // Default to 5kHz low-power sampling
  driver.setDataRate(LIS3DH_DATARATE_LOWPOWER_5KHZ);

  // Default to 4 gravities range
  driver.setRange(LIS3DH_RANGE_4_G);

  setStatus(MOD_STATUS_ONLINE);
}

void BPT_Accel_LIS3DH::shutdown(void){
   if(getStatus(MOD_STATUS_ONLINE)){
    //TODO
     clearStatus(MOD_STATUS_ONLINE);
   }
}

int BPT_Accel_LIS3DH::getAcceleration(accel_t *accel){
  driver.read();
  memset(accel, 0, sizeof(accel_t)); // clears the data

  accel->x = driver.x_g;
  accel->y = driver.y_g;
  accel->z = driver.z_g;

  return 1;
}

// FIXME: find a way to configure the driver pins using external_device_type_t
Adafruit_LIS3DH BPT_Accel_LIS3DH::driver = Adafruit_LIS3DH( A2 );
