/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#include <aidl/android/hardware/gnss/GnssConstellationType.h>
#include <aidl/android/hardware/gnss/IGnssDebug.h>
#include "Gnss.h"
#include "GnssDebug.h"
#include <LocationUtil.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
#define GNSS_DEBUG_UNKNOWN_HORIZONTAL_ACCURACY_METERS (20000000)
#define GNSS_DEBUG_UNKNOWN_VERTICAL_ACCURACY_METERS   (20000)
#define GNSS_DEBUG_UNKNOWN_SPEED_ACCURACY_PER_SEC     (500)
#define GNSS_DEBUG_UNKNOWN_BEARING_ACCURACY_DEG       (180)

#define GNSS_DEBUG_UNKNOWN_UTC_TIME            (1483228800000LL) // 1/1/2017 00:00 GMT
#define GNSS_DEBUG_UNKNOWN_UTC_TIME_UNC_MIN    (999) // 999 ns
#define GNSS_DEBUG_UNKNOWN_UTC_TIME_UNC_MAX    (1.57783680E17) // 5 years in ns
#define GNSS_DEBUG_UNKNOWN_FREQ_UNC_NS_PER_SEC (2.0e5)  // ppm
GnssDebug::GnssDebug(Gnss* gnss) : mGnss(gnss) {}
  GnssDebug::~GnssDebug() {}

  ScopedAStatus GnssDebug::getDebugData(IGnssDebug::DebugData* _aidl_return) {
      LOC_LOGd("]: ");

    IGnssDebug::DebugData data = { };

    if (nullptr == mGnss) {
        LOC_LOGe("GnssDebug - Null GNSS interface");
        *_aidl_return = data;
        return ScopedAStatus::fromExceptionCode(STATUS_INVALID_OPERATION);
    }

    // get debug report snapshot via hal interface
    GnssDebugReport reports = { };
    mGnss->getApi().locAPIGetDebugReport(reports);

    // location block
    if (reports.mLocation.mValid) {
        data.position.valid = true;
        data.position.latitudeDegrees = reports.mLocation.mLocation.latitude;
        data.position.longitudeDegrees = reports.mLocation.mLocation.longitude;
        data.position.altitudeMeters = reports.mLocation.mLocation.altitude;

        data.position.speedMetersPerSec =
            (double)(reports.mLocation.mLocation.speed);
        data.position.bearingDegrees =
            (double)(reports.mLocation.mLocation.bearing);
        data.position.horizontalAccuracyMeters =
            (double)(reports.mLocation.mLocation.accuracy);
        data.position.verticalAccuracyMeters =
            reports.mLocation.verticalAccuracyMeters;
        data.position.speedAccuracyMetersPerSecond =
            reports.mLocation.speedAccuracyMetersPerSecond;
        data.position.bearingAccuracyDegrees =
            reports.mLocation.bearingAccuracyDegrees;

        timeval tv_now, tv_report;
        tv_report.tv_sec  = reports.mLocation.mUtcReported.tv_sec;
        tv_report.tv_usec = reports.mLocation.mUtcReported.tv_nsec / 1000ULL;
        gettimeofday(&tv_now, NULL);
        data.position.ageSeconds =
            (tv_now.tv_sec - tv_report.tv_sec) +
            (float)((tv_now.tv_usec - tv_report.tv_usec)) / 1000000;
    }
    else {
        data.position.valid = false;
    }

    if (data.position.horizontalAccuracyMeters <= 0 ||
        data.position.horizontalAccuracyMeters > GNSS_DEBUG_UNKNOWN_HORIZONTAL_ACCURACY_METERS) {
        data.position.horizontalAccuracyMeters = GNSS_DEBUG_UNKNOWN_HORIZONTAL_ACCURACY_METERS;
    }
    if (data.position.verticalAccuracyMeters <= 0 ||
        data.position.verticalAccuracyMeters > GNSS_DEBUG_UNKNOWN_VERTICAL_ACCURACY_METERS) {
        data.position.verticalAccuracyMeters = GNSS_DEBUG_UNKNOWN_VERTICAL_ACCURACY_METERS;
    }
    if (data.position.speedAccuracyMetersPerSecond <= 0 ||
        data.position.speedAccuracyMetersPerSecond > GNSS_DEBUG_UNKNOWN_SPEED_ACCURACY_PER_SEC) {
        data.position.speedAccuracyMetersPerSecond = GNSS_DEBUG_UNKNOWN_SPEED_ACCURACY_PER_SEC;
    }
    if (data.position.bearingAccuracyDegrees <= 0 ||
        data.position.bearingAccuracyDegrees > GNSS_DEBUG_UNKNOWN_BEARING_ACCURACY_DEG) {
        data.position.bearingAccuracyDegrees = GNSS_DEBUG_UNKNOWN_BEARING_ACCURACY_DEG;
    }

    // time block
    if (reports.mTime.mValid) {
        data.time.timeEstimateMs = reports.mTime.timeEstimate;
        data.time.timeUncertaintyNs = reports.mTime.timeUncertaintyNs;
        data.time.frequencyUncertaintyNsPerSec =
            reports.mTime.frequencyUncertaintyNsPerSec;
    }

    if (data.time.timeEstimateMs < GNSS_DEBUG_UNKNOWN_UTC_TIME) {
        data.time.timeEstimateMs = GNSS_DEBUG_UNKNOWN_UTC_TIME;
    }
    if (data.time.timeUncertaintyNs <= 0) {
        data.time.timeUncertaintyNs = (float)GNSS_DEBUG_UNKNOWN_UTC_TIME_UNC_MIN;
    }
    else if (data.time.timeUncertaintyNs > GNSS_DEBUG_UNKNOWN_UTC_TIME_UNC_MAX) {
        data.time.timeUncertaintyNs = (float)GNSS_DEBUG_UNKNOWN_UTC_TIME_UNC_MAX;
    }
    if (data.time.frequencyUncertaintyNsPerSec <= 0 ||
        data.time.frequencyUncertaintyNsPerSec > (float)GNSS_DEBUG_UNKNOWN_FREQ_UNC_NS_PER_SEC) {
        data.time.frequencyUncertaintyNsPerSec = (float)GNSS_DEBUG_UNKNOWN_FREQ_UNC_NS_PER_SEC;
    }

    // satellite data block
    IGnssDebug::SatelliteData s = { };
    std::vector<IGnssDebug::SatelliteData> s_array;

    for (uint32_t i=0; i<reports.mSatelliteInfo.size(); i++) {
        memset(&s, 0, sizeof(s));
        s.svid = reports.mSatelliteInfo[i].svid;
        convertGnssConstellationType(
            reports.mSatelliteInfo[i].constellation, s.constellation);
        convertGnssEphemerisType(
            reports.mSatelliteInfo[i].mEphemerisType, s.ephemerisType);
        convertGnssEphemerisSource(
            reports.mSatelliteInfo[i].mEphemerisSource, s.ephemerisSource);
        convertGnssEphemerisHealth(
            reports.mSatelliteInfo[i].mEphemerisHealth, s.ephemerisHealth);

        s.ephemerisAgeSeconds =
            reports.mSatelliteInfo[i].ephemerisAgeSeconds;
        s.serverPredictionIsAvailable =
            reports.mSatelliteInfo[i].serverPredictionIsAvailable;
        s.serverPredictionAgeSeconds =
            reports.mSatelliteInfo[i].serverPredictionAgeSeconds;

        s_array.push_back(s);
    }
    data.satelliteDataArray = s_array;

        *_aidl_return = data;
      return ScopedAStatus::ok();
  }
}
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
