#include "lua_web_requests.h"

#include <plog/Log.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

namespace Scripting::Extensions::WebRequests {
    
    void _lua_post_request(std::string address, sol::table args) {
        curlpp::Cleanup myCleanup;

		// Our request to be sent.
		curlpp::Easy myRequest;

		// Set the URL.
		myRequest.setOpt<curlpp::options::Url>(address);

		// Send request and get a result.
		// By default the result goes to standard output.
		myRequest.perform();
    }

    void create_lua_web_requests_module (sol::state* lua) {
        sol::table requests_table = lua->create_named_table("web_requests");

        requests_table.set_function("post", &_lua_post_request);
    }
}