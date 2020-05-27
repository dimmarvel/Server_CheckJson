#include "Precompiled.h"
#include "Json_parser.h"

namespace ServerClient {
	
	boost::property_tree::ptree root;

	std::string Json_parser::check_valid(char message[]) {
		std::stringstream stream(message);
		try {
			boost::property_tree::read_json(stream, root);
			return "json is valid.";
		}
		catch (boost::property_tree::json_parser::json_parser_error& ex) {
			return "unvalid, error message " + std::string(ex.what());
		}
	}
}