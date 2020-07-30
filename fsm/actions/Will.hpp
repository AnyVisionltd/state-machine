#pragma once

namespace {

template <typename... Handlers>
struct Will : Handlers... {
    using Handlers::handle...;
};

}  // namespace
