// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/constant.hpp"

namespace LayerTestsDefinitions {

TEST_P(ConstantLayerTest, CompareWithRefs) {
    Run();
};

TEST_P(ConstantLayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions
