#include "server.cpp"

if(local->actingLikeServer()) {
    try {
        local->handleConnections(&*sinRemote);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }
}
