/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#include <aidl/android/hardware/gnss/IGnssAntennaInfoCallback.h>
#include "Gnss.h"
#include "GnssAntennaInfo.h"
#include <gps_extended_c.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
using ::aidl::android::hardware::gnss::IGnss;

static void convertGnssAntennaInfo(std::vector<GnssAntennaInformation>& in,
        std::vector<IGnssAntennaInfoCallback::GnssAntennaInfo>& antennaInfos);

static void convertGnssAntennaInfo(std::vector<GnssAntennaInformation>& in,
        std::vector<IGnssAntennaInfoCallback::GnssAntennaInfo>& out) {

    uint32_t vecSize, numberOfRows, numberOfColumns;
    vecSize = in.size();
    out.resize(vecSize);
    for (uint32_t i = 0; i < vecSize; i++) {
        out[i].carrierFrequencyHz = in[i].carrierFrequencyMHz * 1000000;
        out[i].phaseCenterOffsetCoordinateMillimeters.x =
                in[i].phaseCenterOffsetCoordinateMillimeters.x;
        out[i].phaseCenterOffsetCoordinateMillimeters.xUncertainty =
                in[i].phaseCenterOffsetCoordinateMillimeters.xUncertainty;
        out[i].phaseCenterOffsetCoordinateMillimeters.y =
                in[i].phaseCenterOffsetCoordinateMillimeters.y;
        out[i].phaseCenterOffsetCoordinateMillimeters.yUncertainty =
                in[i].phaseCenterOffsetCoordinateMillimeters.yUncertainty;
        out[i].phaseCenterOffsetCoordinateMillimeters.z =
                in[i].phaseCenterOffsetCoordinateMillimeters.z;
        out[i].phaseCenterOffsetCoordinateMillimeters.zUncertainty =
                in[i].phaseCenterOffsetCoordinateMillimeters.zUncertainty;

        numberOfRows = in[i].phaseCenterVariationCorrectionMillimeters.size();
        out[i].phaseCenterVariationCorrectionMillimeters.resize(numberOfRows);
        for (uint32_t j = 0; j < numberOfRows; j++) {
            numberOfColumns = in[i].phaseCenterVariationCorrectionMillimeters[j].size();
            out[i].phaseCenterVariationCorrectionMillimeters[j].row.resize(numberOfColumns);
            for (uint32_t k = 0; k < numberOfColumns; k++) {
                out[i].phaseCenterVariationCorrectionMillimeters[j].row[k] =
                        in[i].phaseCenterVariationCorrectionMillimeters[j][k];
            }
        }

        numberOfRows = in[i].phaseCenterVariationCorrectionUncertaintyMillimeters.size();
        out[i].phaseCenterVariationCorrectionUncertaintyMillimeters.resize(numberOfRows);
        for (uint32_t j = 0; j < numberOfRows; j++) {
            numberOfColumns = in[i].phaseCenterVariationCorrectionUncertaintyMillimeters[j].size();
            out[i].phaseCenterVariationCorrectionUncertaintyMillimeters[j].
                    row.resize(numberOfColumns);
            for (uint32_t k = 0; k < numberOfColumns; k++) {
                out[i].phaseCenterVariationCorrectionUncertaintyMillimeters[j].row[k] =
                        in[i].phaseCenterVariationCorrectionUncertaintyMillimeters[j][k];
            }
        }

        numberOfRows = in[i].signalGainCorrectionDbi.size();
        out[i].signalGainCorrectionDbi.resize(numberOfRows);
        for (uint32_t j = 0; j < numberOfRows; j++) {
            numberOfColumns = in[i].signalGainCorrectionDbi[j].size();
            out[i].signalGainCorrectionDbi[j].row.resize(numberOfColumns);
            for (uint32_t k = 0; k < numberOfColumns; k++) {
                out[i].signalGainCorrectionDbi[j].row[k] = in[i].signalGainCorrectionDbi[j][k];
            }
        }

        numberOfRows = in[i].signalGainCorrectionUncertaintyDbi.size();
        out[i].signalGainCorrectionUncertaintyDbi.resize(numberOfRows);
        for (uint32_t j = 0; j < numberOfRows; j++) {
            numberOfColumns = in[i].signalGainCorrectionUncertaintyDbi[j].size();
            out[i].signalGainCorrectionUncertaintyDbi[j].row.resize(numberOfColumns);
            for (uint32_t k = 0; k < numberOfColumns; k++) {
                out[i].signalGainCorrectionUncertaintyDbi[j].row[k] =
                        in[i].signalGainCorrectionUncertaintyDbi[j][k];
            }
        }
    }
}

void gnssAntennaInfoServiceDied(void* cookie) {
    LOC_LOGe("IGnssAntennaInfo AIDL service died");
    GnssAntennaInfo* iface = static_cast<GnssAntennaInfo*>(cookie);
    if (iface != nullptr) {
        iface->close();
        iface = nullptr;
    }
}
GnssAntennaInfo::GnssAntennaInfo(Gnss* gnss) : mGnss(gnss),
    mDeathRecipient(AIBinder_DeathRecipient_new(&gnssAntennaInfoServiceDied)),
    mAntennaInfoCb(*this) { }

ScopedAStatus GnssAntennaInfo::setCallback(
        const shared_ptr<IGnssAntennaInfoCallback>& callback) {
    if (mGnss == nullptr) {
        LOC_LOGe("]: mGnss is nullptr");
        return ScopedAStatus::fromExceptionCode(IGnss::ERROR_GENERIC);
    }

    mMutex.lock();
    if (mGnssAntennaInfoCbIface != nullptr) {
        AIBinder_unlinkToDeath(mGnssAntennaInfoCbIface->asBinder().get(), mDeathRecipient, this);
    }

    mGnssAntennaInfoCbIface = callback;
    if (mGnssAntennaInfoCbIface != nullptr) {
        AIBinder_linkToDeath(mGnssAntennaInfoCbIface->asBinder().get(), mDeathRecipient, this);
    }
    mMutex.unlock();

    mGnss->getApi().locAPIGetAntennaInfo(&mAntennaInfoCb);
    return ScopedAStatus::ok();
}
ScopedAStatus GnssAntennaInfo::close() {
    if (mGnss == nullptr) {
        LOC_LOGe("]: mGnss is nullptr");
        return ScopedAStatus::fromExceptionCode(STATUS_INVALID_OPERATION);
    }

    mGnssAntennaInfoCbIface = nullptr;
    return ScopedAStatus::ok();

}

void GnssAntennaInfo::gnssAntennaInfoCb
        (std::vector<GnssAntennaInformation>& gnssAntennaInformations) {

    mMutex.lock();
    auto gnssAntennaInfoCb = mGnssAntennaInfoCbIface;
    mMutex.unlock();
    if (gnssAntennaInfoCb != nullptr) {
        std::vector<IGnssAntennaInfoCallback::GnssAntennaInfo> antennaInfos;

        // Convert from one structure to another
        convertGnssAntennaInfo(gnssAntennaInformations, antennaInfos);

        auto r = gnssAntennaInfoCb->gnssAntennaInfoCb(antennaInfos);
        if (!r.isOk()) {
            LOC_LOGw("Error antenna info cb");
        }
    } else {
        LOC_LOGw("setCallback has not been called yet");
    }
}

}
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
