// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/minimum_maximum.hpp"

namespace LayerTestsDefinitions {

TEST_P(MaxMinLayerTest, CompareWithRefs){
    Run();
};

TEST_P(MaxMinLayerTest, QueryNetwork) {
    QueryNetwork();
}

}  // namespace LayerTestsDefinitions
