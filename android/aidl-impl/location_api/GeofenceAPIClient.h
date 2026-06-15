/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#ifndef GEOFENCE_API_CLINET_H
#define GEOFENCE_API_CLINET_H

#include <mutex>
#include <LocationAPIClientBase.h>
#include <aidl/android/hardware/gnss/IGnssGeofenceCallback.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
using ::aidl::android::hardware::gnss::IGnssGeofenceCallback;
using ::std::shared_ptr;


class GeofenceAPIClient : public LocationAPIClientBase
{
public:
    GeofenceAPIClient(const shared_ptr<IGnssGeofenceCallback>& callback);
    void upcateCallback(const shared_ptr<IGnssGeofenceCallback>& callback);
    void geofenceAdd(uint32_t geofence_id, double latitude, double longitude,
            double radius_meters, int32_t last_transition, int32_t monitor_transitions,
            uint32_t notification_responsiveness_ms, uint32_t unknown_timer_ms);
    void geofencePause(uint32_t geofence_id);
    void geofenceResume(uint32_t geofence_id, int32_t monitor_transitions);
    void geofenceRemove(uint32_t geofence_id);
    void geofenceRemoveAll();

    // callbacks
    void onGeofenceBreachCb(const GeofenceBreachNotification& geofenceBreachNotification) final;
    void onGeofenceStatusCb(GeofenceStatusNotification geofenceStatusNotification) final;
    void onAddGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onRemoveGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onPauseGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;
    void onResumeGeofencesCb(size_t count, LocationError* errors, uint32_t* ids) final;

private:
    virtual ~GeofenceAPIClient() = default;
    std::mutex mMutex;
    shared_ptr<IGnssGeofenceCallback> mGnssGeofencingCbIface;
};

}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // GEOFENCE_API_CLINET_H
