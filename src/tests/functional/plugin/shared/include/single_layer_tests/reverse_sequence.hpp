// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "shared_test_classes/single_layer/reverse_sequence.hpp"

namespace LayerTestsDefinitions {

TEST_P(ReverseSequenceLayerTest, CompareWithRefs) {
    Run();
};

TEST_P(ReverseSequenceLayerTest, QueryNetwork) {
    QueryNetwork();
}
} // namespace LayerTestsDefinitions