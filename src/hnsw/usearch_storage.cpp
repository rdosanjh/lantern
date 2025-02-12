
#include <usearch.h>
#include <usearch/index.hpp>
extern "C" {
#include "usearch_storage.hpp"
}

#include <cassert>

#include "usearch/index_dense.hpp"

uint32_t UsearchNodeBytes(metadata_t *metadata, int vector_bytes, int level)
{
    const int NODE_HEAD_BYTES = sizeof(usearch_label_t) + sizeof(unum::usearch::level_t) /*sizeof level*/;
    uint32_t  node_bytes = 0;
    node_bytes += NODE_HEAD_BYTES + metadata->neighbors_base_bytes;
    node_bytes += metadata->neighbors_bytes * level;
    node_bytes += vector_bytes;
    return node_bytes;
}

void usearch_init_node(
    metadata_t *meta, char *tape, unsigned long key, uint32_t level, uint32_t slot_id, void *vector, size_t vector_len)
{
    using namespace unum::usearch;
    using node_t = unum::usearch::node_at<default_key_t, default_slot_t>;
    int node_size = UsearchNodeBytes(meta, vector_len, level);
    std::memset(tape, 0, node_size);
    node_t node = node_t{tape};
    assert(level == uint16_t(level));
    node.level(level);
    node.key(key);
    std::memcpy(tape + node_size - vector_len, vector, vector_len);
}

char *extract_node(char              *data,
                   uint64_t           progress,
                   int                dim,
                   metadata_t        *metadata,
                   /*->>output*/ int *node_size,
                   int               *level)
{
    using namespace unum::usearch;
    using node_t = unum::usearch::node_at<default_key_t, default_slot_t>;
    char  *tape = data + progress;
    node_t node = node_t{tape};

    *level = node.level();
    const int VECTOR_BYTES = dim * sizeof(float);
    *node_size = UsearchNodeBytes(metadata, VECTOR_BYTES, *level);
    return tape;
}
