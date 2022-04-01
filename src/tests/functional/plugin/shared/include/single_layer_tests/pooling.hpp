// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/pooling.hpp"

namespace LayerTestsDefinitions {

TEST_P(PoolingLayerTest, CompareWithRefs) {
    Run();
}

TEST_P(GlobalPoolingLayerTest, CompareWithRefs) {
    Run();

    if (targetDevice == std::string{CommonTestUtils::DEVICE_GPU}) {
        PluginCache::get().reset();
    }
}

TEST_P(MaxPoolingV8LayerTest, CompareWithRefs) {
    Run();
}

TEST_P(PoolingLayerTest, QueryNetwork) {
    QueryNetwork();
}

TEST_P(GlobalPoolingLayerTest, QueryNetwork) {
    QueryNetwork();

    if (targetDevice == std::string{CommonTestUtils::DEVICE_GPU}) {
        PluginCache::get().reset();
    }
}

TEST_P(MaxPoolingV8LayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions
