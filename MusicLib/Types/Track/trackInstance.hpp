/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <BabelWiresLib/TypeSystem/valueHolder.hpp>
#include <BabelWiresLib/Instance/instanceTemplates.hpp>

namespace babelwires {
    template <typename VALUE_TREE_NODE, typename TRACK_TYPE>
        requires std::is_base_of_v<bw_music::TrackType, TRACK_TYPE>
    class InstanceImpl<VALUE_TREE_NODE, TRACK_TYPE> : public InstanceCommonBase<VALUE_TREE_NODE, TRACK_TYPE> {
      public:
        InstanceImpl(VALUE_TREE_NODE& valueFeature)
            : InstanceCommonBase<VALUE_TREE_NODE, TRACK_TYPE>(valueFeature) {}

        const typename bw_music::Track& get() const {
            return this->m_valueTreeNode.getValue()->template as<bw_music::Track>();
        }
        template <typename VALUE_TREE_NODE_M = VALUE_TREE_NODE>
        std::enable_if_t<!std::is_const_v<VALUE_TREE_NODE_M>, void> set(babelwires::ValueHolder newValue) {
            this->m_valueTreeNode.setValue(std::move(newValue));
        }
    };

} // namespace babelwires
