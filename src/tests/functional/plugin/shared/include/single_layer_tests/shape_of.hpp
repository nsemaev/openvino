// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/shape_of.hpp"

namespace LayerTestsDefinitions {

TEST_P(ShapeOfLayerTest, CompareWithRefs) {
    Run();
}

TEST_P(ShapeOfLayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions