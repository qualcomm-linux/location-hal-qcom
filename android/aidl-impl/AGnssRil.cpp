/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#include <aidl/android/hardware/gnss/IAGnssRil.h>
#include <aidl/android/hardware/gnss/IAGnssRilCallback.h>
#include <aidl/android/hardware/gnss/BnAGnssRil.h>
#include "Gnss.h"
#include "AGnssRil.h"
#include <DataItemConcreteTypes.h>
#include <log_util.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
AGnssRil::AGnssRil(Gnss* gnss) : mGnss(gnss) {
    ENTRY_LOG_CALLFLOW();
}

AGnssRil::~AGnssRil() {
    ENTRY_LOG_CALLFLOW();
}

ScopedAStatus AGnssRil::updateNetworkState(const IAGnssRil::NetworkAttributes& attributes) {
    ENTRY_LOG_CALLFLOW();
    std::string apn = attributes.apn;
    if (nullptr != mGnss && (nullptr != mGnss->getLocationControlApi())) {
        int8_t typeout = loc_core::TYPE_UNKNOWN;
        bool roaming = false;
        if (attributes.capabilities & IAGnssRil::NETWORK_CAPABILITY_NOT_METERED) {
            typeout = loc_core::TYPE_WIFI;
        } else {
            typeout = loc_core::TYPE_MOBILE;
        }
        if (attributes.capabilities & IAGnssRil::NETWORK_CAPABILITY_NOT_ROAMING) {
            roaming = false;
        }
        LOC_LOGd("apn string received is: %s", apn.c_str());
        mGnss->getLocationControlApi()->updateConnectionStatus(attributes.isConnected,
                typeout, roaming, (NetworkHandle) attributes.networkHandle, apn);
    }
    return ScopedAStatus::ok();
}
}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
