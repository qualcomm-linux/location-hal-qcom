/*
 * Copyright (c) 2021, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/
#ifndef ANDROID_HARDWARE_GNSS_AIDL_GNSSCONFIGURATION_H
#define ANDROID_HARDWARE_GNSS_AIDL_GNSSCONFIGURATION_H
#include <aidl/android/hardware/gnss/BnGnssConfiguration.h>
#include <aidl/android/hardware/gnss/GnssConstellationType.h>
#include <aidl/android/hardware/gnss/BlocklistedSource.h>
#include <mutex>
#include <unordered_set>
#include <vector>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {

using ::aidl::android::hardware::gnss::GnssConstellationType;
using ::aidl::android::hardware::gnss::BlocklistedSource;
using ::aidl::android::hardware::gnss::BnGnssConfiguration;
using ::ndk::ScopedAStatus;

struct BlocklistedSourceHash {
    inline int operator()(const BlocklistedSource& source) const {
        return int(source.constellation) * 1000 + int(source.svid);
    }
};

struct BlocklistedSourceEqual {
    inline bool operator()(const BlocklistedSource& s1, const BlocklistedSource& s2) const {
        return (s1.constellation == s2.constellation) && (s1.svid == s2.svid);
    }
};

using std::vector;
using BlocklistedSourceSet =
        std::unordered_set<BlocklistedSource, BlocklistedSourceHash, BlocklistedSourceEqual>;
using BlocklistedConstellationSet = std::unordered_set<GnssConstellationType>;

struct GnssConfiguration : public BnGnssConfiguration {
public:
    GnssConfiguration(Gnss* gnss);
    ScopedAStatus setSuplVersion(int) override;

    ScopedAStatus setSuplMode(int) override;

    ScopedAStatus setLppProfile(int) override;

    ScopedAStatus setGlonassPositioningProtocol(int) override;

    ScopedAStatus setEmergencySuplPdn(bool) override;

    ScopedAStatus setEsExtensionSec(int) override;

    ScopedAStatus setBlocklist(const vector<BlocklistedSource>& blocklist) override;


private:
    Gnss* mGnss = nullptr;
    bool setBlocklistedSource(GnssSvIdSource& copyToSource,
            const BlocklistedSource& copyFromSource);
    BlocklistedSourceSet mBlocklistedSourceSet;
    BlocklistedConstellationSet mBlocklistedConstellationSet;
    mutable std::recursive_mutex mMutex;
};

}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif //ANDROID_HARDWARE_GNSS_AIDL_GNSSCONFIGURATION_H
