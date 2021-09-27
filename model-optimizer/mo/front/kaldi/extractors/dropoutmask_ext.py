# Copyright (C) 2018-2021 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
from mo.front.extractor import FrontExtractorOp
from mo.front.kaldi.loader.utils import collect_until_token, collect_until_token_and_read, read_binary_float_token
from mo.ops.dropoutmask import DropoutMask


class DropoutMaskComponentFrontExtractor(FrontExtractorOp):
    op = 'dropoutmaskcomponent'
    enabled = True

    @classmethod
    def extract(cls, node):
        pb = node.parameters

        size = collect_until_token_and_read(pb, b'<OutputDim>')
        collect_until_token(pb, b'<DropoutProportion>')
        dropout_proportion = read_binary_float_token(pb)
        DropoutMask.update_node_stat(node, {'dropout_proportion': 1.0-dropout_proportion,
                                            'size': size})

        return cls.enabled
