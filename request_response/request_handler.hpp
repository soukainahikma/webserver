#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"

class RequestHandler
{
private:
	Request req;
	
public:
	RequestHandler(Request &req) {
		this->req = req;
	};

	Response generateResponse () {
		
	}

	~RequestHandler() {
		
	};
};

#endif