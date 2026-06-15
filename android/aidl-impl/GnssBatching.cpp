/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#define LOG_TAG "GnssBatchingAidl"

#include "GnssBatching.h"
#include <android/binder_auto_utils.h>
#include <log_util.h>
#include <inttypes.h>
#include "loc_misc_utils.h"


namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
void gnssBatchingDied(void* cookie) {
    LOC_LOGe("IGnssBatchingCallback service died");
    GnssBatching* iface = static_cast<GnssBatching*>(cookie);
    if (iface != nullptr) {
        iface->cleanup();
        iface = nullptr;
    }
}
GnssBatching::GnssBatching(): mBatchSize(0),
    mDeathRecipient(AIBinder_DeathRecipient_new(gnssBatchingDied)) {
}

GnssBatching::~GnssBatching() {
}


ScopedAStatus GnssBatching::init(const shared_ptr<IGnssBatchingCallback>& callback) {
    if (mApi != nullptr) {
        mApi->gnssUpdateCallbacks(callback);
    } else {
        mApi = new BatchingAPIClient(callback);
    }

    mMutex.lock();
    if (mGnssBatchingCbIface != nullptr) {
        AIBinder_unlinkToDeath(mGnssBatchingCbIface->asBinder().get(), mDeathRecipient, this);
    }

    mGnssBatchingCbIface = callback;
    if (mGnssBatchingCbIface != nullptr) {
        AIBinder_linkToDeath(mGnssBatchingCbIface->asBinder().get(), mDeathRecipient, this);
    }
    mMutex.unlock();
    return ScopedAStatus::ok();
}
ScopedAStatus GnssBatching::getBatchSize(int32_t* _aidl_return) {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mBatchSize = mApi->getBatchSize();
    }
    *_aidl_return = mBatchSize;
    return ScopedAStatus::ok();
}
ScopedAStatus GnssBatching::start(const IGnssBatching::Options& options) {
    if (nullptr != mApi) {
        mApi->startSession(options);
    } else {
        LOC_LOGe("mGnssInterface is nullptr");
    }

    return ScopedAStatus::ok();
}
ScopedAStatus GnssBatching::flush() {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->flushBatchedLocations();
    }
    return ScopedAStatus::ok();
}
ScopedAStatus GnssBatching::stop() {
    if (mApi == nullptr) {
        LOC_LOGe("]: mApi is nullptr");
    } else {
        mApi->stopSession();
    }
    return ScopedAStatus::ok();
}
ScopedAStatus GnssBatching::cleanup() {
    if (mApi != nullptr) {
        mApi->gnssUpdateCallbacks(nullptr);
        mApi->stopSession();
    }
    mMutex.lock();
    if (mGnssBatchingCbIface != nullptr) {
        mGnssBatchingCbIface = nullptr;
    }
    mMutex.unlock();
    return ScopedAStatus::ok();
}

}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
