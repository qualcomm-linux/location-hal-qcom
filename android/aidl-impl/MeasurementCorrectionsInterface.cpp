/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#include <aidl/android/hardware/gnss/measurement_corrections/IMeasurementCorrectionsCallback.h>
#include <aidl/android/hardware/gnss/measurement_corrections/MeasurementCorrections.h>
#include "Gnss.h"
#include "MeasurementCorrectionsInterface.h"
#include <LocationUtil.h>

namespace android {
namespace hardware {
namespace gnss {
namespace measurement_corrections {
namespace aidl {
namespace implementation {
using ::aidl::android::hardware::gnss::GnssConstellationType;
static MeasurementCorrectionsInterface* spMeasurementCorrections = nullptr;

void measurementCorrectionsInterfaceDied(void* cookie) {
    LOC_LOGe("IGnssAntennaInfo AIDL service died");
    MeasurementCorrectionsInterface* iface = static_cast<MeasurementCorrectionsInterface*>(cookie);
    if (iface != nullptr) {
        iface->setCallback(nullptr);
        iface = nullptr;
    }
}
MeasurementCorrectionsInterface::MeasurementCorrectionsInterface(Gnss* gnss) : mGnss(gnss),
    mDeathRecipient(AIBinder_DeathRecipient_new(&measurementCorrectionsInterfaceDied)) {
    spMeasurementCorrections = this;
}

MeasurementCorrectionsInterface::~MeasurementCorrectionsInterface() {
    spMeasurementCorrections = nullptr;
}

void MeasurementCorrectionsInterface::measCorrSetCapabilitiesCb(
        GnssMeasurementCorrectionsCapabilitiesMask capabilities) {
    if (nullptr != spMeasurementCorrections) {
        spMeasurementCorrections->setCapabilitiesCb(capabilities);
    }
}

void MeasurementCorrectionsInterface::setCapabilitiesCb(
    GnssMeasurementCorrectionsCapabilitiesMask capabilities) {
    std::unique_lock<std::mutex> lock(mMutex);
    auto measCorrCbIface(mMeasurementCorrectionsCbIface);
    lock.unlock();
    if (measCorrCbIface != nullptr) {
        uint32_t measCorrCapabilities = 0;

        // Convert from one enum to another
        if (capabilities & GNSS_MEAS_CORR_LOS_SATS) {
            measCorrCapabilities |=
                    IMeasurementCorrectionsCallback::CAPABILITY_LOS_SATS;
        }
        if (capabilities & GNSS_MEAS_CORR_EXCESS_PATH_LENGTH) {
            measCorrCapabilities |=
                    IMeasurementCorrectionsCallback::CAPABILITY_EXCESS_PATH_LENGTH;
        }
        if (capabilities & GNSS_MEAS_CORR_REFLECTING_PLANE) {
            measCorrCapabilities |=
                    IMeasurementCorrectionsCallback::CAPABILITY_REFLECTING_PLANE;
        }

        auto r = measCorrCbIface->setCapabilitiesCb(measCorrCapabilities);
        if (!r.isOk()) {
            LOC_LOGw("Error invoking setCapabilitiesCb");
        }
    } else {
        LOC_LOGw("setCallback has not been called yet");
    }
}


ScopedAStatus MeasurementCorrectionsInterface::setCorrections(
        const MeasurementCorrections& corrections) {
    GnssMeasurementCorrections gnssMeasurementCorrections = {};

    gnss::aidl::implementation::convertMeasurementCorrections(corrections,
            gnssMeasurementCorrections);

    gnssMeasurementCorrections.hasEnvironmentBearing = corrections.hasEnvironmentBearing;
    gnssMeasurementCorrections.environmentBearingDegrees =
            corrections.environmentBearingDegrees;
    gnssMeasurementCorrections.environmentBearingUncertaintyDegrees =
            corrections.environmentBearingUncertaintyDegrees;

    for (int i = 0; i < corrections.satCorrections.size(); i++) {
        GnssSingleSatCorrection gnssSingleSatCorrection = {};

        gnss::aidl::implementation::convertSingleSatCorrections(corrections.satCorrections[i],
                gnssSingleSatCorrection);
        switch (corrections.satCorrections[i].constellation) {
        case (GnssConstellationType::GPS):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_GPS;
            break;
        case (GnssConstellationType::SBAS):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_SBAS;
            break;
        case (GnssConstellationType::GLONASS):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_GLONASS;
            break;
        case (GnssConstellationType::QZSS):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_QZSS;
            break;
        case (GnssConstellationType::BEIDOU):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_BEIDOU;
            break;
        case (GnssConstellationType::GALILEO):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_GALILEO;
            break;
        case (GnssConstellationType::IRNSS):
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_NAVIC;
            break;
        case (GnssConstellationType::UNKNOWN):
        default:
            gnssSingleSatCorrection.svType = GNSS_SV_TYPE_UNKNOWN;
            break;
        }
        gnssMeasurementCorrections.satCorrections.push_back(gnssSingleSatCorrection);
    }

    mGnss->getLocationControlApi()->measCorrSetCorrections(gnssMeasurementCorrections);
   return ScopedAStatus::ok();
}

ScopedAStatus MeasurementCorrectionsInterface::setCallback(
        const shared_ptr<IMeasurementCorrectionsCallback>& callback) {
    if (nullptr == mGnss || nullptr == mGnss->getLocationControlApi()) {
        LOC_LOGe("Null GNSS interface");
        return ScopedAStatus::fromExceptionCode(STATUS_INVALID_OPERATION);
    }
    std::unique_lock<std::mutex> lock(mMutex);
    if (mMeasurementCorrectionsCbIface != nullptr) {
        AIBinder_unlinkToDeath(mMeasurementCorrectionsCbIface->asBinder().get(), mDeathRecipient,
                this);
    }
    mMeasurementCorrectionsCbIface = callback;
    if (mMeasurementCorrectionsCbIface != nullptr) {
        AIBinder_linkToDeath(mMeasurementCorrectionsCbIface->asBinder().get(), mDeathRecipient,
                this);
    }
    lock.unlock();

    LocationControlCallbacks locCtrlCbs;
    memset(&locCtrlCbs, 0, sizeof(locCtrlCbs));
    locCtrlCbs.size = sizeof(LocationControlCallbacks);

    locCtrlCbs.measCorrSetCapabilitiesCb =
            [this] (GnssMeasurementCorrectionsCapabilitiesMask capabilities) {
            measCorrSetCapabilitiesCb(capabilities);
    };
    mGnss->getLocationControlApi()->updateCallbacks(locCtrlCbs);
    return ScopedAStatus::ok();}

}
}  // namespace aidl
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android
