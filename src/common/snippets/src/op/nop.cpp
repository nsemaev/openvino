// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "snippets/op/nop.hpp"

using namespace std;
using namespace ngraph;

snippets::op::Nop::Nop(const OutputVector& arguments, const OutputVector& results) : Op([arguments, results]() -> OutputVector {
    OutputVector x;
    x.insert(x.end(), arguments.begin(), arguments.end());
    x.insert(x.end(), results.begin(), results.end());
    return x;
    }()) {
}
