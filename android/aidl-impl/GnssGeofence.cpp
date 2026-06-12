/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#include <aidl/android/hardware/gnss/IGnssGeofenceCallback.h>
#include "GnssGeofence.h"

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
void gnssGeofenceDied(void* cookie) {
    LOC_LOGe("IGnssGeofence AIDL service died");
    GnssGeofence* iface = static_cast<GnssGeofence*>(cookie);
    if (iface != nullptr) {
        iface->removeAllGeofences();
        iface->setCallback(nullptr);
        iface = nullptr;
    }
}
GnssGeofence::GnssGeofence(): mApi(nullptr),
    mDeathRecipient(AIBinder_DeathRecipient_new(&gnssGeofenceDied)) {}
GnssGeofence::~GnssGeofence() {}

ScopedAStatus GnssGeofence::setCallback(const shared_ptr<IGnssGeofenceCallback>& callback) {
    if (mApi != nullptr) {
        mApi->upcateCallback(callback);
    } else {
        mApi = new GeofenceAPIClient(callback);
    }
    if (mApi == nullptr) {
        LOC_LOGe("]: failed to create mApi");
    }
    mMutex.lock();
    if (mGnssGeofencingCbIface != nullptr) {
        AIBinder_unlinkToDeath(mGnssGeofencingCbIface->asBinder().get(), mDeathRecipient, this);
    }
    mGnssGeofencingCbIface = callback;
    if (mGnssGeofencingCbIface != nullptr) {
        AIBinder_linkToDeath(mGnssGeofencingCbIface->asBinder().get(), mDeathRecipient, this);
    }
    mMutex.unlock();
    return ScopedAStatus::ok();
}

ScopedAStatus GnssGeofence::addGeofence(int32_t geofenceId, double latitudeDegrees,
        double longitudeDegrees, double radiusMeters, int32_t lastTransition,
        int32_t monitorTransitions, int32_t notificationResponsivenessMs, int32_t unknownTimerMs) {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->geofenceAdd(
                geofenceId,
                latitudeDegrees,
                longitudeDegrees,
                radiusMeters,
                static_cast<int32_t>(lastTransition),
                monitorTransitions,
                notificationResponsivenessMs,
                unknownTimerMs);
    }
    return ScopedAStatus::ok();
}

ScopedAStatus GnssGeofence::pauseGeofence(int32_t geofenceId) {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->geofencePause(geofenceId);
    }
    return ScopedAStatus::ok();
}

ScopedAStatus GnssGeofence::resumeGeofence(int32_t geofenceId, int32_t monitorTransitions) {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->geofenceResume(geofenceId, monitorTransitions);
    }
    return ScopedAStatus::ok();
}

ScopedAStatus GnssGeofence::removeGeofence(int32_t geofenceId) {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->geofenceRemove(geofenceId);
    }
    return ScopedAStatus::ok();
}

void GnssGeofence::removeAllGeofences()  {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr, do nothing");
    } else {
        mApi->geofenceRemoveAll();
    }
}
}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
