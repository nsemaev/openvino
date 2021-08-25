// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

#include "gtest/gtest.h"
#include "ngraph/ngraph.hpp"
#include "util/all_close.hpp"
#include "util/all_close_f.hpp"
#include "util/engine/test_engines.hpp"
#include "util/ndarray.hpp"
#include "util/random.hpp"
#include "util/test_case.hpp"
#include "util/test_control.hpp"
#include "util/test_tools.hpp"

using namespace std;
using namespace ngraph;

static string s_manifest = "${MANIFEST}";

using TestEngine = test::ENGINE_CLASS_NAME(${BACKEND_NAME});

static void normalize_l2_results_test(std::vector<float>& data,
                                      Shape& data_shape,
                                      std::vector<int32_t>& axes,
                                      ngraph::op::EpsMode eps_mode,
                                      float eps,
                                      std::vector<float>& expected_output) {
    auto data_input = std::make_shared<op::Parameter>(element::f32, data_shape);
    const auto axes_input = std::make_shared<op::Constant>(element::i32, Shape{axes.size()}, axes);

    auto normalize = std::make_shared<op::v0::NormalizeL2>(data_input, axes_input, eps, eps_mode);
    auto function = std::make_shared<Function>(normalize, ParameterVector{data_input});

    auto test_case = test::TestCase<TestEngine>(function);
    test_case.add_input<float>(data);
    test_case.add_expected_output<float>(data_shape, expected_output);

    test_case.run(DEFAULT_FLOAT_TOLERANCE_BITS + 4);
}

// 1D
NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_1D_axes_empty_add) {
    std::vector<float> data{0, 3, 0, 8};
    Shape data_shape{4};
    std::vector<int32_t> axes{};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0, 1, 0, 1};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_1D_axes_empty_max) {
    std::vector<float> data{0, 3, 0, 8};
    Shape data_shape{4};
    std::vector<int32_t> axes{};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0, 1, 0, 1};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_1D_axes_0_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{4};
    std::vector<int32_t> axes{0};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.18257418, 0.36514837, 0.5477226, 0.73029673};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_1D_axes_0_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{4};
    std::vector<int32_t> axes{0};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.18257418, 0.36514837, 0.5477226, 0.73029673};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

// 2D
NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_empty_add) {
    std::vector<float> data{0, 3, 0, 8};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0, 1, 0, 1};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_empty_max) {
    std::vector<float> data{0, 3, 0, 8};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0, 1, 0, 1};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_0_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{0};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.31622776, 0.4472136, 0.94868326, 0.8944272};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_0_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{0};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.31622777, 0.4472136, 0.9486833, 0.89442719};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_1_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.4472136, 0.8944272, 0.6, 0.8};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_1_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.4472136, 0.89442719, 0.6, 0.8};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_01_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{0, 1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.18257418, 0.36514837, 0.5477226, 0.73029673};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_2D_axes_01_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{2, 2};
    std::vector<int32_t> axes{0, 1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.18257419, 0.36514837, 0.54772256, 0.73029674};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

// 3D

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_3D_axes_1_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{1, 2, 2};
    std::vector<int32_t> axes{1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.31622776, 0.4472136, 0.94868326, 0.8944272};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_3D_axes_1_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{1, 2, 2};
    std::vector<int32_t> axes{1};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.31622776, 0.4472136, 0.94868326, 0.8944272};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_3D_axes_2_add) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{1, 2, 2};
    std::vector<int32_t> axes{2};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0.4472136, 0.8944272, 0.6, 0.8};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_3D_axes_2_max) {
    std::vector<float> data{1, 2, 3, 4};
    Shape data_shape{1, 2, 2};
    std::vector<int32_t> axes{2};
    float eps = 1e-7;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{0.4472136, 0.8944272, 0.6, 0.8};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

// 4D

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_empty_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 1);
    std::vector<int32_t> axes{};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output(shape_size(data_shape), 1);

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_empty_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 1);
    std::vector<int32_t> axes{};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output(shape_size(data_shape), 1);

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_0_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{0};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.03996804, 0.07669649, 0.11043153, 0.14142135, 0.1699069,  0.19611612, 0.22026087,
        0.2425356,  0.26311737, 0.2821663,  0.2998266,  0.31622776, 0.331486,   0.34570533, 0.35897905,
        0.37139067, 0.38301498, 0.3939193,  0.40416384, 0.41380292, 0.42288542, 0.43145543, 0.43955287,
        0.99999994, 0.9992009,  0.9970544,  0.9938838,  0.98994946, 0.98546,    0.9805806,  0.97544104,
        0.9701424,  0.96476364, 0.9593654,  0.95399374, 0.9486833,  0.9434601,  0.93834305, 0.93334556,
        0.9284767,  0.923742,   0.919145,   0.91468656, 0.9103665,  0.90618306, 0.90213406, 0.8982167};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_0_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{0};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.0399648, 0.0766909, 0.110424, 0.141413, 0.169897, 0.196106, 0.22025,
                                       0.242524, 0.263106,  0.282155,  0.299815, 0.316217, 0.331475, 0.345695, 0.358969,
                                       0.371381, 0.383005,  0.39391,   0.404155, 0.413794, 0.422877, 0.431447, 0.439545,
                                       0.999913, 0.999121,  0.996981,  0.993816, 0.989888, 0.985403, 0.980528, 0.975393,
                                       0.970098, 0.964723,  0.959327,  0.953958, 0.94865,  0.94343,  0.938315, 0.933319,
                                       0.928452, 0.923719,  0.919123,  0.914666, 0.910347, 0.906165, 0.902117, 0.8982};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_1_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.07669649, 0.14142135, 0.19611612, 0.2425356,  0.2821663,  0.31622776, 0.34570533,
        0.37139067, 0.3939193,  0.41380292, 0.43145543, 0.99999994, 0.9970544,  0.98994946, 0.9805806,
        0.9701424,  0.9593654,  0.9486833,  0.93834305, 0.9284767,  0.919145,   0.9103665,  0.90213406,
        0.5547002,  0.55985737, 0.56468385, 0.5692099,  0.57346237, 0.57746464, 0.58123815, 0.5848015,
        0.58817166, 0.59136367, 0.59439105, 0.5972662,  0.83205026, 0.82858896, 0.8253072,  0.8221921,
        0.8192319,  0.81641555, 0.8137334,  0.8111763,  0.808736,   0.80640495, 0.8041761,  0.8020432};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_1_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.07667395, 0.14138602, 0.19607423, 0.24249104, 0.28212142, 0.31618387, 0.3456632,
        0.37135068, 0.3938816,  0.41376755, 0.43142232, 0.9996529,  0.9967614,  0.9897021,  0.9803712,
        0.96996415, 0.9592128,  0.94855154, 0.93822867, 0.9283767,  0.9190571,  0.9102886,  0.9020648,
        0.5546854,  0.55984336, 0.56467056, 0.56919736, 0.5734503,  0.57745326, 0.58122724, 0.5847912,
        0.58816177, 0.59135413, 0.594382,   0.59725744, 0.8320281,  0.8285682,  0.82528776, 0.82217395,
        0.8192147,  0.8163994,  0.8137182,  0.81116194, 0.8087224,  0.806392,   0.8041638,  0.8020314};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_2_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.09667365, 0.16903085, 0.22423053, 0.4472136,  0.48336822, 0.50709254, 0.52320457,
        0.8944272,  0.8700628,  0.8451542,  0.8221786,  0.42426404, 0.4335743,  0.4418361,  0.4492145,
        0.5656854,  0.5669818,  0.56807494, 0.569005,   0.7071067,  0.70038927, 0.6943139,  0.68879557,
        0.49153918, 0.4945891,  0.49743116, 0.5000857,  0.57346237, 0.5737234,  0.57395905, 0.5741725,
        0.65538555, 0.6528576,  0.6504869,  0.6482592,  0.51789176, 0.5193782,  0.52079225, 0.5221394,
        0.5754353,  0.5755272,  0.57561255, 0.5756921,  0.63297886, 0.6316762,  0.6304327,  0.62924486};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_2_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.0966285, 0.168971, 0.224168, 0.446934, 0.483143, 0.506912, 0.523058,
                                       0.893869, 0.869657,  0.844853, 0.821949, 0.424238, 0.43355,  0.441814, 0.449194,
                                       0.56565,  0.56695,   0.568047, 0.56898,  0.707063, 0.70035,  0.694279, 0.688765,
                                       0.491529, 0.494579,  0.497422, 0.500077, 0.57345,  0.573712, 0.573949, 0.574163,
                                       0.655372, 0.652845,  0.650475, 0.648248, 0.517886, 0.519373, 0.520787, 0.522135,
                                       0.575429, 0.575521,  0.575607, 0.575687, 0.632972, 0.63167,  0.630427, 0.629239};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_3_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.26726124, 0.5345225,  0.8017837,  0.3563483,  0.44543537, 0.5345225,  0.62360954,
        0.41816667, 0.4704375,  0.52270836, 0.5749792,  0.44292808, 0.47983873, 0.5167494,  0.5536601,
        0.45621273, 0.484726,   0.5132393,  0.54175264, 0.4644887,  0.4877131,  0.5109376,  0.534162,
        0.47013652, 0.48972553, 0.50931454, 0.5289036,  0.47423577, 0.49117276, 0.50810975, 0.5250467,
        0.47734618, 0.49226326, 0.50718033, 0.5220974,  0.4797868,  0.49311423, 0.50644165, 0.5197691,
        0.48175293, 0.49379677, 0.5058406,  0.51788443, 0.48337057, 0.49435627, 0.50534195, 0.5163277};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_3_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.266312, 0.532624, 0.798935, 0.356207, 0.445259, 0.534311, 0.623362,
                                       0.41811,  0.470373, 0.522637, 0.574901, 0.442898, 0.479806, 0.516714, 0.553622,
                                       0.456194, 0.484706, 0.513219, 0.541731, 0.464476, 0.4877,   0.510924, 0.534148,
                                       0.470128, 0.489716, 0.509305, 0.528893, 0.474229, 0.491166, 0.508102, 0.525039,
                                       0.477341, 0.492258, 0.507175, 0.522092, 0.479783, 0.49311,  0.506437, 0.519764,
                                       0.481749, 0.493793, 0.505837, 0.517881, 0.483368, 0.494353, 0.505339, 0.516325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_23_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2, 3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.04445542, 0.08891085, 0.13336627, 0.1778217,  0.22227712, 0.26673254, 0.31118798,
        0.3556434,  0.4000988,  0.44455424, 0.48900968, 0.19420628, 0.21039014, 0.226574,   0.24275786,
        0.2589417,  0.27512556, 0.29130942, 0.30749327, 0.32367712, 0.339861,   0.35604486, 0.3722287,
        0.23326269, 0.24298197, 0.25270125, 0.26242054, 0.2721398,  0.28185907, 0.29157835, 0.30129763,
        0.31101692, 0.3207362,  0.33045548, 0.34017476, 0.24955511, 0.2564872,  0.26341927, 0.27035138,
        0.27728346, 0.28421554, 0.29114762, 0.2980797,  0.3050118,  0.3119439,  0.31887597, 0.32580805};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_23_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2, 3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.04445103, 0.08890206, 0.1333531,  0.17780413, 0.22225516, 0.2667062,  0.31115723,
        0.35560825, 0.40005928, 0.4445103,  0.48896134, 0.19420375, 0.2103874,  0.22657104, 0.24275468,
        0.2589383,  0.275122,   0.29130563, 0.30748928, 0.32367292, 0.33985656, 0.3560402,  0.37222385,
        0.2332616,  0.24298084, 0.25270006, 0.2624193,  0.27213854, 0.2818578,  0.291577,   0.30129623,
        0.3110155,  0.3207347,  0.33045393, 0.34017318, 0.2495545,  0.25648656, 0.26341864, 0.27035072,
        0.27728277, 0.28421485, 0.29114693, 0.29807898, 0.30501106, 0.3119431,  0.3188752,  0.32580727};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_123_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1, 2, 3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.01520748, 0.03041495, 0.04562243, 0.06082991, 0.07603738, 0.09124486, 0.10645234,
        0.12165982, 0.13686728, 0.15207477, 0.16728225, 0.18248972, 0.19769719, 0.21290468, 0.22811216,
        0.24331963, 0.2585271,  0.27373457, 0.28894207, 0.30414954, 0.319357,   0.3345645,  0.34977198,
        0.13544846, 0.14109215, 0.14673583, 0.15237951, 0.15802321, 0.16366689, 0.16931057, 0.17495427,
        0.18059795, 0.18624163, 0.19188532, 0.197529,   0.20317268, 0.20881638, 0.21446006, 0.22010374,
        0.22574744, 0.23139112, 0.2370348,  0.2426785,  0.24832217, 0.25396585, 0.25960955, 0.26525325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_123_big_eps_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1, 2, 3};
    float eps = 100;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.01520748, 0.03041495, 0.04562243, 0.06082991, 0.07603738, 0.09124486, 0.10645234,
        0.12165982, 0.13686728, 0.15207477, 0.16728225, 0.18248972, 0.19769719, 0.21290468, 0.22811216,
        0.24331963, 0.2585271,  0.27373457, 0.28894207, 0.30414954, 0.319357,   0.3345645,  0.34977198,
        0.13544846, 0.14109215, 0.14673583, 0.15237951, 0.15802321, 0.16366689, 0.16931057, 0.17495427,
        0.18059795, 0.18624163, 0.19188532, 0.197529,   0.20317268, 0.20881638, 0.21446006, 0.22010374,
        0.22574744, 0.23139112, 0.2370348,  0.2426785,  0.24832217, 0.25396585, 0.25960955, 0.26525325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_123_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1, 2, 3};
    float eps = 1e-9;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0,        0.0152075, 0.030415, 0.0456224, 0.0608299, 0.0760374, 0.0912449, 0.106452, 0.12166,  0.136867,
        0.152075, 0.167282,  0.18249,  0.197697,  0.212905,  0.228112,  0.24332,   0.258527, 0.273735, 0.288942,
        0.30415,  0.319357,  0.334565, 0.349772,  0.135448,  0.141092,  0.146736,  0.15238,  0.158023, 0.163667,
        0.169311, 0.174954,  0.180598, 0.186242,  0.191885,  0.197529,  0.203173,  0.208816, 0.21446,  0.220104,
        0.225747, 0.231391,  0.237035, 0.242678,  0.248322,  0.253966,  0.25961,   0.265253};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_123_big_eps_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1, 2, 3};
    float eps = 0.5;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0,        0.0152066, 0.0304132, 0.0456198, 0.0608264, 0.076033, 0.0912396, 0.106446, 0.121653, 0.136859,
        0.152066, 0.167273,  0.182479,  0.197686,  0.212892,  0.228099, 0.243306,  0.258512, 0.273719, 0.288925,
        0.304132, 0.319339,  0.334545,  0.349752,  0.135447,  0.141091, 0.146735,  0.152378, 0.158022, 0.163666,
        0.169309, 0.174953,  0.180597,  0.18624,   0.191884,  0.197527, 0.203171,  0.208815, 0.214458, 0.220102,
        0.225746, 0.231389,  0.237033,  0.242677,  0.24832,   0.253964, 0.259607,  0.265251};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_unsorted_312_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3, 1, 2};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.01520748, 0.03041495, 0.04562243, 0.06082991, 0.07603738, 0.09124486, 0.10645234,
        0.12165982, 0.13686728, 0.15207477, 0.16728225, 0.18248972, 0.19769719, 0.21290468, 0.22811216,
        0.24331963, 0.2585271,  0.27373457, 0.28894207, 0.30414954, 0.319357,   0.3345645,  0.34977198,
        0.13544846, 0.14109215, 0.14673583, 0.15237951, 0.15802321, 0.16366689, 0.16931057, 0.17495427,
        0.18059795, 0.18624163, 0.19188532, 0.197529,   0.20317268, 0.20881638, 0.21446006, 0.22010374,
        0.22574744, 0.23139112, 0.2370348,  0.2426785,  0.24832217, 0.25396585, 0.25960955, 0.26525325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_unsorted_312_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3, 1, 2};
    float eps = 1e-9;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0,        0.0152075, 0.030415, 0.0456224, 0.0608299, 0.0760374, 0.0912449, 0.106452, 0.12166,  0.136867,
        0.152075, 0.167282,  0.18249,  0.197697,  0.212905,  0.228112,  0.24332,   0.258527, 0.273735, 0.288942,
        0.30415,  0.319357,  0.334565, 0.349772,  0.135448,  0.141092,  0.146736,  0.15238,  0.158023, 0.163667,
        0.169311, 0.174954,  0.180598, 0.186242,  0.191885,  0.197529,  0.203173,  0.208816, 0.21446,  0.220104,
        0.225747, 0.231391,  0.237035, 0.242678,  0.248322,  0.253966,  0.25961,   0.265253};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_0123_max) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{0, 1, 2, 3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.00529108, 0.01058216, 0.01587324, 0.02116432, 0.02645539, 0.03174648, 0.03703756,
        0.04232863, 0.04761971, 0.05291079, 0.05820187, 0.06349295, 0.06878403, 0.07407511, 0.07936618,
        0.08465727, 0.08994835, 0.09523942, 0.10053051, 0.10582158, 0.11111266, 0.11640374, 0.12169482,
        0.12698591, 0.13227698, 0.13756806, 0.14285913, 0.14815022, 0.1534413,  0.15873237, 0.16402346,
        0.16931453, 0.17460561, 0.1798967,  0.18518777, 0.19047885, 0.19576994, 0.20106101, 0.20635208,
        0.21164316, 0.21693425, 0.22222532, 0.2275164,  0.23280749, 0.23809856, 0.24338964, 0.24868073};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_4D_axes_0123_add) {
    Shape data_shape{2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{0, 1, 2, 3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.00529108, 0.01058216, 0.01587324, 0.02116432, 0.02645539, 0.03174648, 0.03703756,
        0.04232863, 0.04761971, 0.05291079, 0.05820187, 0.06349295, 0.06878403, 0.07407511, 0.07936618,
        0.08465727, 0.08994835, 0.09523942, 0.10053051, 0.10582158, 0.11111266, 0.11640374, 0.12169482,
        0.12698591, 0.13227698, 0.13756806, 0.14285913, 0.14815022, 0.1534413,  0.15873237, 0.16402346,
        0.16931453, 0.17460561, 0.1798967,  0.18518777, 0.19047885, 0.19576994, 0.20106101, 0.20635208,
        0.21164316, 0.21693425, 0.22222532, 0.2275164,  0.23280749, 0.23809856, 0.24338964, 0.24868073};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_empty_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 1);
    std::vector<int32_t> axes{};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output(shape_size(data_shape), 1);

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_empty_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 1);
    std::vector<int32_t> axes{};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output(shape_size(data_shape), 1);

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_1_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.03996804, 0.07669649, 0.11043153, 0.14142135, 0.1699069,  0.19611612, 0.22026087,
        0.2425356,  0.26311737, 0.2821663,  0.2998266,  0.31622776, 0.331486,   0.34570533, 0.35897905,
        0.37139067, 0.38301498, 0.3939193,  0.40416384, 0.41380292, 0.42288542, 0.43145543, 0.43955287,
        0.99999994, 0.9992009,  0.9970544,  0.9938838,  0.98994946, 0.98546,    0.9805806,  0.97544104,
        0.9701424,  0.96476364, 0.9593654,  0.95399374, 0.9486833,  0.9434601,  0.93834305, 0.93334556,
        0.9284767,  0.923742,   0.919145,   0.91468656, 0.9103665,  0.90618306, 0.90213406, 0.8982167};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_1_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{1};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.0399648, 0.0766909, 0.110424, 0.141413, 0.169897, 0.196106, 0.22025,
                                       0.242524, 0.263106,  0.282155,  0.299815, 0.316217, 0.331475, 0.345695, 0.358969,
                                       0.371381, 0.383005,  0.39391,   0.404155, 0.413794, 0.422877, 0.431447, 0.439545,
                                       0.999913, 0.999121,  0.996981,  0.993816, 0.989888, 0.985403, 0.980528, 0.975393,
                                       0.970098, 0.964723,  0.959327,  0.953958, 0.94865,  0.94343,  0.938315, 0.933319,
                                       0.928452, 0.923719,  0.919123,  0.914666, 0.910347, 0.906165, 0.902117, 0.8982};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_2_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.07669649, 0.14142135, 0.19611612, 0.2425356,  0.2821663,  0.31622776, 0.34570533,
        0.37139067, 0.3939193,  0.41380292, 0.43145543, 0.99999994, 0.9970544,  0.98994946, 0.9805806,
        0.9701424,  0.9593654,  0.9486833,  0.93834305, 0.9284767,  0.919145,   0.9103665,  0.90213406,
        0.5547002,  0.55985737, 0.56468385, 0.5692099,  0.57346237, 0.57746464, 0.58123815, 0.5848015,
        0.58817166, 0.59136367, 0.59439105, 0.5972662,  0.83205026, 0.82858896, 0.8253072,  0.8221921,
        0.8192319,  0.81641555, 0.8137334,  0.8111763,  0.808736,   0.80640495, 0.8041761,  0.8020432};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_2_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.07667395, 0.14138602, 0.19607423, 0.24249104, 0.28212142, 0.31618387, 0.3456632,
        0.37135068, 0.3938816,  0.41376755, 0.43142232, 0.9996529,  0.9967614,  0.9897021,  0.9803712,
        0.96996415, 0.9592128,  0.94855154, 0.93822867, 0.9283767,  0.9190571,  0.9102886,  0.9020648,
        0.5546854,  0.55984336, 0.56467056, 0.56919736, 0.5734503,  0.57745326, 0.58122724, 0.5847912,
        0.58816177, 0.59135413, 0.594382,   0.59725744, 0.8320281,  0.8285682,  0.82528776, 0.82217395,
        0.8192147,  0.8163994,  0.8137182,  0.81116194, 0.8087224,  0.806392,   0.8041638,  0.8020314};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_3_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.09667365, 0.16903085, 0.22423053, 0.4472136,  0.48336822, 0.50709254, 0.52320457,
        0.8944272,  0.8700628,  0.8451542,  0.8221786,  0.42426404, 0.4335743,  0.4418361,  0.4492145,
        0.5656854,  0.5669818,  0.56807494, 0.569005,   0.7071067,  0.70038927, 0.6943139,  0.68879557,
        0.49153918, 0.4945891,  0.49743116, 0.5000857,  0.57346237, 0.5737234,  0.57395905, 0.5741725,
        0.65538555, 0.6528576,  0.6504869,  0.6482592,  0.51789176, 0.5193782,  0.52079225, 0.5221394,
        0.5754353,  0.5755272,  0.57561255, 0.5756921,  0.63297886, 0.6316762,  0.6304327,  0.62924486};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_3_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.0966285, 0.168971, 0.224168, 0.446934, 0.483143, 0.506912, 0.523058,
                                       0.893869, 0.869657,  0.844853, 0.821949, 0.424238, 0.43355,  0.441814, 0.449194,
                                       0.56565,  0.56695,   0.568047, 0.56898,  0.707063, 0.70035,  0.694279, 0.688765,
                                       0.491529, 0.494579,  0.497422, 0.500077, 0.57345,  0.573712, 0.573949, 0.574163,
                                       0.655372, 0.652845,  0.650475, 0.648248, 0.517886, 0.519373, 0.520787, 0.522135,
                                       0.575429, 0.575521,  0.575607, 0.575687, 0.632972, 0.63167,  0.630427, 0.629239};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_4_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.26726124, 0.5345225,  0.8017837,  0.3563483,  0.44543537, 0.5345225,  0.62360954,
        0.41816667, 0.4704375,  0.52270836, 0.5749792,  0.44292808, 0.47983873, 0.5167494,  0.5536601,
        0.45621273, 0.484726,   0.5132393,  0.54175264, 0.4644887,  0.4877131,  0.5109376,  0.534162,
        0.47013652, 0.48972553, 0.50931454, 0.5289036,  0.47423577, 0.49117276, 0.50810975, 0.5250467,
        0.47734618, 0.49226326, 0.50718033, 0.5220974,  0.4797868,  0.49311423, 0.50644165, 0.5197691,
        0.48175293, 0.49379677, 0.5058406,  0.51788443, 0.48337057, 0.49435627, 0.50534195, 0.5163277};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_4_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{0,        0.266312, 0.532624, 0.798935, 0.356207, 0.445259, 0.534311, 0.623362,
                                       0.41811,  0.470373, 0.522637, 0.574901, 0.442898, 0.479806, 0.516714, 0.553622,
                                       0.456194, 0.484706, 0.513219, 0.541731, 0.464476, 0.4877,   0.510924, 0.534148,
                                       0.470128, 0.489716, 0.509305, 0.528893, 0.474229, 0.491166, 0.508102, 0.525039,
                                       0.477341, 0.492258, 0.507175, 0.522092, 0.479783, 0.49311,  0.506437, 0.519764,
                                       0.481749, 0.493793, 0.505837, 0.517881, 0.483368, 0.494353, 0.505339, 0.516325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_34_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3, 4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.04445542, 0.08891085, 0.13336627, 0.1778217,  0.22227712, 0.26673254, 0.31118798,
        0.3556434,  0.4000988,  0.44455424, 0.48900968, 0.19420628, 0.21039014, 0.226574,   0.24275786,
        0.2589417,  0.27512556, 0.29130942, 0.30749327, 0.32367712, 0.339861,   0.35604486, 0.3722287,
        0.23326269, 0.24298197, 0.25270125, 0.26242054, 0.2721398,  0.28185907, 0.29157835, 0.30129763,
        0.31101692, 0.3207362,  0.33045548, 0.34017476, 0.24955511, 0.2564872,  0.26341927, 0.27035138,
        0.27728346, 0.28421554, 0.29114762, 0.2980797,  0.3050118,  0.3119439,  0.31887597, 0.32580805};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_34_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{3, 4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.04445103, 0.08890206, 0.1333531,  0.17780413, 0.22225516, 0.2667062,  0.31115723,
        0.35560825, 0.40005928, 0.4445103,  0.48896134, 0.19420375, 0.2103874,  0.22657104, 0.24275468,
        0.2589383,  0.275122,   0.29130563, 0.30748928, 0.32367292, 0.33985656, 0.3560402,  0.37222385,
        0.2332616,  0.24298084, 0.25270006, 0.2624193,  0.27213854, 0.2818578,  0.291577,   0.30129623,
        0.3110155,  0.3207347,  0.33045393, 0.34017318, 0.2495545,  0.25648656, 0.26341864, 0.27035072,
        0.27728277, 0.28421485, 0.29114693, 0.29807898, 0.30501106, 0.3119431,  0.3188752,  0.32580727};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_234_max) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2, 3, 4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::MAX;
    std::vector<float> expected_output{
        0.,         0.01520748, 0.03041495, 0.04562243, 0.06082991, 0.07603738, 0.09124486, 0.10645234,
        0.12165982, 0.13686728, 0.15207477, 0.16728225, 0.18248972, 0.19769719, 0.21290468, 0.22811216,
        0.24331963, 0.2585271,  0.27373457, 0.28894207, 0.30414954, 0.319357,   0.3345645,  0.34977198,
        0.13544846, 0.14109215, 0.14673583, 0.15237951, 0.15802321, 0.16366689, 0.16931057, 0.17495427,
        0.18059795, 0.18624163, 0.19188532, 0.197529,   0.20317268, 0.20881638, 0.21446006, 0.22010374,
        0.22574744, 0.23139112, 0.2370348,  0.2426785,  0.24832217, 0.25396585, 0.25960955, 0.26525325};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}

NGRAPH_TEST(${BACKEND_NAME}, normalize_l2_5D_axes_234_add) {
    Shape data_shape{1, 2, 2, 3, 4};
    std::vector<float> data(shape_size(data_shape));
    iota(begin(data), end(data), 0);
    std::vector<int32_t> axes{2, 3, 4};
    float eps = 0.1;
    auto eps_mode = ngraph::op::EpsMode::ADD;
    std::vector<float> expected_output{
        0.,         0.0152073,  0.0304146,  0.0456219,  0.0608292,  0.07603651, 0.0912438,  0.10645111,
        0.12165841, 0.1368657,  0.15207301, 0.16728032, 0.1824876,  0.19769491, 0.21290222, 0.22810951,
        0.24331681, 0.25852412, 0.2737314,  0.28893873, 0.30414602, 0.3193533,  0.33456063, 0.34976792,
        0.13544825, 0.14109191, 0.1467356,  0.15237927, 0.15802296, 0.16366662, 0.1693103,  0.17495398,
        0.18059766, 0.18624133, 0.19188501, 0.19752869, 0.20317237, 0.20881604, 0.21445972, 0.2201034,
        0.22574706, 0.23139074, 0.23703443, 0.2426781,  0.24832177, 0.25396547, 0.25960913, 0.2652528};

    normalize_l2_results_test(data, data_shape, axes, eps_mode, eps, expected_output);
}
