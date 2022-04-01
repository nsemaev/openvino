// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/gather.hpp"

namespace LayerTestsDefinitions {

TEST_P(GatherLayerTest, CompareWithRefs) {
    Run();
};

TEST_P(Gather7LayerTest, CompareWithRefs) {
    Run();
};

TEST_P(Gather8LayerTest, CompareWithRefs) {
    Run();
};

TEST_P(GatherLayerTest, QueryNetwork) {
    QueryNetwork();
}

TEST_P(Gather7LayerTest, QueryNetwork) {
    QueryNetwork();
}

TEST_P(Gather8LayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions
