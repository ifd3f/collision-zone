#include "WebSocketListener.hpp"

#include <websocketpp/connection.hpp>
#include <memory>


using namespace websocketpp;
using namespace snowplowderby::websocket;
using websocketpp::lib::bind;

util::Logger WebSocketListener::logger = util::get_logger("SPD-WebSocketListener");

WebSocketListener::WebSocketListener(int port) : port(port) {
    
}

WebSocketListener::~WebSocketListener() {

}

void WebSocketListener::begin() {
    LOG_INFO(logger) << "Beginning websocket listener";
    server.init_asio();
    set_up_handlers();
    server.listen(port);
    server.start_accept();
    server.run();
}

void WebSocketListener::set_up_handlers() {
    server.set_open_handler([this](connection_hdl handle) {
        LOG_INFO(logger) << "A new connection was made with handle: " << handle.lock().get();
        auto conn = server.get_con_from_hdl(handle);
        std::shared_ptr<WebSocketClient> client(new WebSocketClient(conn));
        clients.push_back(client);
    });
}
