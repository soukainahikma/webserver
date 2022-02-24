#include "request_handler.hpp"

RequestHandler::RequestHandler(std::vector<Server> &Servs) { this->Servs = Servs; };
RequestHandler::RequestHandler(Request &req, std::vector<Server> &Serv)
{
	this->req = req;
	this->Servs = Serv;
};

void RequestHandler::setRequest(Request &req)
{
	this->req = req;
}

Response RequestHandler::Bootstrap()
{
	if (req.getRequest()["Method"] == "GET")
		return GETRequest(req, Servs).returnedResponse();
	if (req.getRequest()["Method"] == "POST")
		return POSTRequest(req, Servs).returnedResponse();
	if (req.getRequest()["Method"] == "DELETE")
		return DELETERequest(req, Servs).returnedResponse();
	return GETRequest(req, Servs).returnedResponse();
}

std::vector<Server> RequestHandler::get_servers() const{
	return (Servs);
}

size_t RequestHandler::get_default_index(int port) {
	size_t i = 0;
	for (i = 0; i < Servs.size(); i++)
	{
		if(Servs[i].get_listen() == port)
			return (i);
	}
	return (0);
}

RequestHandler::~RequestHandler(){};