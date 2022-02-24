#include"GETRequest.hpp"
    GETRequest::GETRequest(Request &req, std::vector<Server> &servs): ARequest(req, servs) {
        this->method = "GET";

    };

    Response GETRequest::logic_function(Server server, Location location) {
        if (req.getBodyString().length() > server.get_client_max_body_size())
				return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413", req);
        return Response(server, location, req);
    }
    GETRequest::~GETRequest() {
    };