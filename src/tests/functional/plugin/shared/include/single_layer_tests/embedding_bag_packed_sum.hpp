// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/embedding_bag_packed_sum.hpp"

namespace LayerTestsDefinitions {

TEST_P(EmbeddingBagPackedSumLayerTest, CompareWithRefs) {
    Run();
}

TEST_P(EmbeddingBagPackedSumLayerTest, QueryNetwork) {
    QueryNetwork();
}
}  // namespace LayerTestsDefinitions
