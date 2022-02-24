#include "POSTRequest.hpp"

    POSTRequest::POSTRequest(Request &req, std::vector<Server> &servs): ARequest(req, servs) {
       this->method = "POST";
    };

    Response POSTRequest::logic_function(Server server, Location location) {
		struct stat fileStat;
		std::string upload_path = location.get_upload_store() != "" ? location.get_upload_store() : location.get_path();

		if (req.getBodyString().length() > server.get_client_max_body_size())
			return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413", req);
		if (req.get_content_type() == "multipart/form-data" && location.get_upload_enable() == "on")
		{
			std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
			{
				if (body[i].filename != "" && location.get_upload_enable() == "on")
				{					
					if(stat((server.get_root() + upload_path).c_str(), &fileStat) < 0)    
        				return Response(server, server.get_root() + server.get_error_page()["500"], this->method, "500", req);
					if(!(fileStat.st_mode & S_IWUSR))
        				return Response(server, server.get_root() + server.get_error_page()["403"], this->method, "403", req);
					int fd = open((server.get_root() + "/" + upload_path + body[i].filename).c_str(), O_CREAT | O_RDWR, 0644);
					write(fd, body[i].body.c_str(), body[i].body.length());

				}
				else if (location.get_upload_enable() != "on" && body[i].filename != "")
					return Response(server, server.get_root() + server.get_error_page()["405"], this->method, "405", req);
			}
		}
		return Response(server, location, req);
	}

    POSTRequest::~POSTRequest() {};
