/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#ifndef BATCHING_API_CLINET_H
#define BATCHING_API_CLINET_H

#include <mutex>
#include <pthread.h>

#include <LocationAPIClientBase.h>
#include <aidl/android/hardware/gnss/BnGnssBatching.h>
#include <aidl/android/hardware/gnss/IGnssBatchingCallback.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {

using ::std::shared_ptr;
using ::aidl::android::hardware::gnss::IGnssBatching;
using ::aidl::android::hardware::gnss::IGnssBatchingCallback;

enum BATCHING_STATE { STARTED, STOPPING, STOPPED };

class BatchingAPIClient : public LocationAPIClientBase
{
public:
    BatchingAPIClient(const shared_ptr<IGnssBatchingCallback>& callback);
    void gnssUpdateCallbacks(const shared_ptr<IGnssBatchingCallback>& callback);
    int getBatchSize();
    int startSession(const IGnssBatching::Options& options);
    int updateSessionOptions(const IGnssBatching::Options& options);
    int stopSession();
    void getBatchedLocation(int last_n_locations);
    void flushBatchedLocations();

    inline LocationCapabilitiesMask getCapabilities() { return mLocationCapabilitiesMask; }

    // callbacks
    void onCapabilitiesCb(LocationCapabilitiesMask capabilitiesMask) final;
    void onBatchingCb(size_t count, Location* location, BatchingOptions batchOptions) final;

private:
    ~BatchingAPIClient();

    void setCallbacks();
    std::mutex mMutex;
    shared_ptr<IGnssBatchingCallback> mGnssBatchingCbIface;
    uint32_t mDefaultId;
    LocationCapabilitiesMask mLocationCapabilitiesMask;
    volatile BATCHING_STATE mState = STOPPED;

    std::vector<Location> mBatchedLocationInCache;
};

}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // BATCHING_API_CLINET_H
