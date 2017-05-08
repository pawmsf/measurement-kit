// Part of measurement-kit <https://measurement-kit.github.io/>.
// Measurement-kit is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
#ifndef SRC_LIBMEASUREMENT_KIT_DNS_SYSTEM_RESOLVER_HPP
#define SRC_LIBMEASUREMENT_KIT_DNS_SYSTEM_RESOLVER_HPP

#include <measurement_kit/dns.hpp>

namespace mk {
namespace dns {

void system_resolver(
        NameServers name_servers,
        QueryClass dns_class,
        QueryType dns_type,
        std::string name,
        Settings settings,
        Var<Reactor> reactor,
        Var<Logger> logger,
        Callback<Error, Var<Message>> cb
);

} // namespace dns
} // namespace mk
#endif
