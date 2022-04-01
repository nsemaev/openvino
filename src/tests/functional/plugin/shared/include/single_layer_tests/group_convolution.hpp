// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/group_convolution.hpp"

namespace LayerTestsDefinitions {
TEST_P(GroupConvolutionLayerTest, CompareWithRefs) {
    Run();
}

TEST_P(GroupConvolutionLayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions