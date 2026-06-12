/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
*/

#ifndef LOCATION_UTIL_H
#define LOCATION_UTIL_H

#include <LocationAPI.h>
#include <aidl/android/hardware/gnss/IGnssDebug.h>
#include <aidl/android/hardware/gnss/measurement_corrections/SingleSatCorrection.h>
#include <aidl/android/hardware/gnss/measurement_corrections/MeasurementCorrections.h>
#include <aidl/android/hardware/gnss/GnssLocation.h>
#include <aidl/android/hardware/gnss/SatellitePvt.h>
#include <aidl/android/hardware/gnss/GnssConstellationType.h>

namespace android {
namespace hardware {
namespace gnss {
namespace aidl {
namespace implementation {
using ::aidl::android::hardware::gnss::measurement_corrections::SingleSatCorrection;
using ::aidl::android::hardware::gnss::measurement_corrections::MeasurementCorrections;
using ::aidl::android::hardware::gnss::IGnssDebug;
using ::aidl::android::hardware::gnss::GnssLocation;
using ::aidl::android::hardware::gnss::GnssConstellationType;
using ::aidl::android::hardware::gnss::SatellitePvt;

void convertGnssLocation(const GnssLocation& in, Location& out);
void convertGnssLocation(const Location& in, GnssLocation& out);
void convertGnssConstellationType(const GnssSvType& in, GnssConstellationType& out);
void convertGnssSvid(const GnssSv& in, int& out);
void convertGnssSvid(const GnssMeasurementsData& in, int16_t& out);
void convertGnssEphemerisType(const GnssEphemerisType& in, IGnssDebug::SatelliteEphemerisType& out);
void convertGnssEphemerisSource(const GnssEphemerisSource& in,
        SatellitePvt::SatelliteEphemerisSource& out);
void convertGnssEphemerisHealth(const GnssEphemerisHealth& in,
        IGnssDebug::SatelliteEphemerisHealth& out);
void convertSingleSatCorrections(const SingleSatCorrection& in, GnssSingleSatCorrection& out);
void convertMeasurementCorrections(const MeasurementCorrections& in,
                                   GnssMeasurementCorrections& out);

}  // namespace implementation
}  // namespace aidl
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif // LOCATION_UTIL_H
