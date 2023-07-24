#include "lua_web_requests.h"

#include <map>
#include <list>
#include <fstream>

#include <plog/Log.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curl/curl.h>

#include "ns_abbr/fs.h"

namespace Scripting::Extensions::WebRequests {
    
    // Simple POST Requests:
    static std::string _lua_post_request(std::string address, std::string args, std::list<std::string> header) {
        // Config the POST header:
        header.push_front("Content-Type: application/json");

        // Create request
        curlpp::Cleanup clean;
        curlpp::Easy r;
        r.setOpt(new curlpp::options::Url(address));
        r.setOpt(new curlpp::options::HttpHeader(header));
        r.setOpt(new curlpp::options::PostFields(args));
        r.setOpt(new curlpp::options::PostFieldSize(args.length()));

        // Generate output stream:
        std::ostringstream response;
        r.setOpt(new curlpp::options::WriteStream(&response));

        // Perform request:
        r.perform();

        return std::string(response.str());
    }

    // Simple GET Requests:
    static std::string _lua_get_request(std::string address, std::list<std::string> header) {
        // Create request
        curlpp::Cleanup clean;
        curlpp::Easy r;
        r.setOpt(new curlpp::options::Url(address));
        r.setOpt(new curlpp::options::HttpHeader(header));

        // Generate output stream:
        std::ostringstream response;
        r.setOpt(new curlpp::options::WriteStream(&response));

        // Perform request:
        r.perform();
        return std::string(response.str());
    }

    static std::string params_string(CURL* handle, std::map<std::string, std::string> params) {
        std::string url_prams = "";
        for(const auto& it : params) {
            std::string encoded_1 = curl_easy_escape(handle, it.first.c_str(), it.first.length());
            std::string encoded_2 = curl_easy_escape(handle, it.second.c_str(), it.second.length());

            url_prams += "&" + encoded_1 + "=" + encoded_2;
        }
        return url_prams;
    }

    static std::string _lua_get_request_args(std::string address, std::map<std::string, std::string> args, std::list<std::string> header) {
        // Create request:
        curlpp::Cleanup clean;
        curlpp::Easy r;

        // Encode the strings for URL inclusion:
        std::string query_address = address + "?" + params_string(r.getHandle(), args);

        r.setOpt(new curlpp::options::Url(query_address));
        r.setOpt(new curlpp::options::HttpHeader(header));

        // Generate output stream:
        std::ostringstream response;
        r.setOpt(new curlpp::options::WriteStream(&response));

        // Perform request:
        r.perform();
        

        return std::string(response.str());
    }

    static void _lua_download_file(std::string address, std::string save_path, std::list<std::string> header) {
        // Create request:
        curlpp::Cleanup clean;
        curlpp::Easy r;
        r.setOpt(new curlpp::options::Url(address));
        r.setOpt(new curlpp::options::HttpHeader(header));

        fs::path save_parent = fs::path(save_path).parent_path();
        // Make sure the output directory exist:
        if (!fs::exists(save_parent)) {
            fs::create_directories(save_parent);
        }

        // Generate output stream:
        std::ofstream response(save_path, std::ios::binary);
        
        // r.setOpt(new curlpp::options::WriteFile());
        r.setOpt(new curlpp::options::WriteStream(&response));
        
        // Perform request:
        r.perform();
        // return true;
    }

    void create_lua_web_requests_module (sol::state_view* lua) {
        sol::table requests_table = lua->create_named_table("web_requests");

        requests_table.set_function("post", &_lua_post_request);
        requests_table.set_function("get", &_lua_get_request);
        requests_table.set_function("get_args", &_lua_get_request_args);
        requests_table.set_function("download_file", &_lua_download_file);
    }
}