#pragma once

namespace utilities {
    template<typename t>
    inline auto valid_mem(t pointer) -> bool {
        return (pointer && pointer > (void*)0xFFFFFF && (void*)pointer < (void*)0x7FFFFFFFFFFF);
    }
}