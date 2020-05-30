/* SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * SPDX-FileCopyrightText: 2020 the prplMesh contributors (see AUTHORS.md)
 *
 * This code is subject to the terms of the BSD+Patent license.
 * See LICENSE file for more details.
 */

#include <bcl/beerocks_utils.h>
#include <bcl/son/son_wireless_utils.h>

#include <gtest/gtest.h>

namespace {

typedef std::tuple<beerocks::eWiFiBandwidth, uint8_t> tBandwidthOperatingClass;

/**
 * @brief Helper structure for value-parameterezed test WirelessUtilsGetOperatingClassByChannelTest.
 *
 * The first tuple element is the channel used for constructing the sWifiChannel.
 * The second element is a tuple of bandwidth and expected operating class.
 * It is constructed this way because that allows us to use a testing::Range for the channel and
 * combine it with a bandwidth, operating class tuple using testing::Combine
 */
typedef std::tuple<uint8_t, tBandwidthOperatingClass> tGetOperatingClassByChannelParam;

class WirelessUtilsGetOperatingClassByChannelTest
    : public testing::TestWithParam<tGetOperatingClassByChannelParam> {
};

TEST_P(WirelessUtilsGetOperatingClassByChannelTest, should_return_expected)
{
    beerocks::message::sWifiChannel ch;
    tBandwidthOperatingClass second;
    std::tie(ch.channel, second) = GetParam();
    uint8_t expected_operating_class;
    std::tie(ch.channel_bandwidth, expected_operating_class) = second;
    EXPECT_EQ(son::wireless_utils::get_operating_class_by_channel(ch), expected_operating_class);
}

// See https://docs.google.com/spreadsheets/d/1J2IYuGjFX_OQQpgb4OgsyDx73klAUk6qSmeB4acJ5us
// for the table we need to conform to.

// 2.4GHz
/* Note: we have no way to distinguish 40- and 40+, so operating class 83 is used for all bw=40 */
INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_1_9,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Range(uint8_t{1}, uint8_t{10}),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 81},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 83})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_10_13,
    WirelessUtilsGetOperatingClassByChannelTest,
        testing::Combine(testing::Range(uint8_t{10}, uint8_t{14}),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 81},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 84})));

INSTANTIATE_TEST_SUITE_P(get_operating_class_by_channel_should_return_valid_channels_14,
                         WirelessUtilsGetOperatingClassByChannelTest,
                         testing::Values(tGetOperatingClassByChannelParam{
                             14, {beerocks::BANDWIDTH_20, 82}}));

// 5GHz lower band
INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_36_44,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(36, 44),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 115},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 116})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_40_48,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(40, 48),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 115},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 117})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_52_60,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(52, 60),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 118},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 119})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_46_64,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(56, 64),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 118},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 120})));

// 5GHz upper band
INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_100_140,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Range(uint8_t{100}, uint8_t{141}, 8),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 121},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 122})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_104_144,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Range(uint8_t{104}, uint8_t{145}, 8),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 121},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 123})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_149_157,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(149, 157),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 124},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 126})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_153_161,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(153, 161),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 124},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_40, 127})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_165_169,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(165, 169),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_20, 125})));

// VHT bandwidths
INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_vht_80mhz,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(36, 52, 100, 116, 132, 149),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_80, 128},
                                     tBandwidthOperatingClass{beerocks::BANDWIDTH_80_80, 130})));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_should_return_valid_channels_vht_160mhz,
    WirelessUtilsGetOperatingClassByChannelTest,
    testing::Combine(testing::Values(36, 100),
                     testing::Values(tBandwidthOperatingClass{beerocks::BANDWIDTH_160, 129})));

/**
 * @brief Helper structure for value-parameterezed test
 * WirelessUtilsGetOperatingClassByChannelInvalidTest.
 *
 * This is a tuple of (channel, bandwidth) used for constructing the sWifiChannel.
 */
typedef std::tuple<uint8_t, beerocks::eWiFiBandwidth> tGetOperatingClassByChannelInvalidParam;

class WirelessUtilsGetOperatingClassByChannelInvalidTest
    : public testing::TestWithParam<tGetOperatingClassByChannelInvalidParam> {
};

TEST_P(WirelessUtilsGetOperatingClassByChannelInvalidTest, should_fail)
{
    beerocks::message::sWifiChannel ch;
    std::tie(ch.channel, ch.channel_bandwidth) = GetParam();
    EXPECT_EQ(son::wireless_utils::get_operating_class_by_channel(ch), 0);
}

static std::vector<tGetOperatingClassByChannelInvalidParam>
generate_invalid_channels_for_bandwidth(beerocks::eWiFiBandwidth bandwidth)
{
    std::vector<tGetOperatingClassByChannelInvalidParam> result;
    auto add_channel = [&](uint8_t channel) { result.push_back({channel, bandwidth});};
    auto add_channel_range = [&](uint8_t start, uint8_t end) {
        for (uint8_t channel = start; channel <= end; channel++) {
            add_channel(channel);
        }
    };

    // Channel 0 is never valid
    add_channel(0);
    // Channels 1-13 only valid for 20-40MHz
    if (bandwidth != beerocks::BANDWIDTH_20 && bandwidth != beerocks::BANDWIDTH_40) {
        add_channel_range(1, 13);
    }
    // Channel 14 only valid for 20MHz
    if (bandwidth != beerocks::BANDWIDTH_20) {
        add_channel(14);
    }
    // Channels 15-35 never valid
    add_channel_range(15, 35);

    uint8_t bandwidth_jump;
    switch (bandwidth) {
        case beerocks::BANDWIDTH_20:
        bandwidth_jump = 4;
        break;
        case beerocks::BANDWIDTH_40:
        bandwidth_jump = 4; // There's always a - and a + class
        break;
        case beerocks::BANDWIDTH_80:
        case beerocks::BANDWIDTH_80_80:
        bandwidth_jump = 16;
        break;
        case beerocks::BANDWIDTH_160:
        bandwidth_jump = 32;
        break;
    default:
        bandwidth_jump = 40; /* Bail out - everything is invalid */
        break;
    }

    auto add_channel_range_with_bandwidth_jump = [&](uint8_t start, uint8_t end) {
        for (uint8_t channel = start; channel <= end; channel++) {
            if ((channel - start) % bandwidth_jump != 0) {
                add_channel(channel);
            }
        }
    };

    // Channels 36-64 take bandwidth jumps
    add_channel_range_with_bandwidth_jump(36, 64);

    // Channels 65-99 are never valid
    add_channel_range(65, 99);

    // Channels 100-144 take bandwidth jumps
    add_channel_range_with_bandwidth_jump(100, 144);

    // Channels 145-148 are never valid
    add_channel_range(145, 148);

    // Channels 149-169 take bandwidth jumps
    add_channel_range_with_bandwidth_jump(149, 169);

    // Channels 165 and 169 are actually only valid for 20MHz
    if (bandwidth != beerocks::BANDWIDTH_20) {
        add_channel(165);
        add_channel(169);
    }

    // Anything higher is never valid
    add_channel_range(170, 254);

    // Due to uint8_t wraparound, we have to treat 255 specially
    add_channel(255);

    return result;
}

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_invalid_channel_20mhz,
    WirelessUtilsGetOperatingClassByChannelInvalidTest,
    testing::ValuesIn(generate_invalid_channels_for_bandwidth(beerocks::BANDWIDTH_20)));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_invalid_channel_40mhz,
    WirelessUtilsGetOperatingClassByChannelInvalidTest,
    testing::ValuesIn(generate_invalid_channels_for_bandwidth(beerocks::BANDWIDTH_40)));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_invalid_channel_80mhz,
    WirelessUtilsGetOperatingClassByChannelInvalidTest,
    testing::ValuesIn(generate_invalid_channels_for_bandwidth(beerocks::BANDWIDTH_80)));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_invalid_channel_80_80mhz,
    WirelessUtilsGetOperatingClassByChannelInvalidTest,
    testing::ValuesIn(generate_invalid_channels_for_bandwidth(beerocks::BANDWIDTH_80_80)));

INSTANTIATE_TEST_SUITE_P(
    get_operating_class_by_channel_invalid_channel_160mhz,
    WirelessUtilsGetOperatingClassByChannelInvalidTest,
    testing::ValuesIn(generate_invalid_channels_for_bandwidth(beerocks::BANDWIDTH_160)));

} // namespace
