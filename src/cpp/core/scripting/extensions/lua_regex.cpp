#include <regex>
#include <vector>
#include <format>

#include <plog/Log.h>

#include "lua_regex.h"


namespace Scripting::Extensions::Regex {

    static std::regex_constants::match_flag_type get_match_flags(std::vector<std::string> match_flags) {
        std::regex_constants::match_flag_type retVal = (std::regex_constants::match_flag_type)0;
        for (auto &it : match_flags) {
            if (it == "match_not_bol") retVal |= std::regex_constants::match_flag_type::match_not_bol;
            if (it == "match_not_eol") retVal |= std::regex_constants::match_flag_type::match_not_eol;
            if (it == "match_not_bow") retVal |= std::regex_constants::match_flag_type::match_not_bow;
            if (it == "match_not_eow") retVal |= std::regex_constants::match_flag_type::match_not_eow;
            if (it == "match_any") retVal |= std::regex_constants::match_flag_type::match_any;
            if (it == "match_not_null") retVal |= std::regex_constants::match_flag_type::match_not_null;
            if (it == "match_continuous") retVal |= std::regex_constants::match_flag_type::match_continuous;
            if (it == "match_prev_avail") retVal |= std::regex_constants::match_flag_type::match_prev_avail;
            if (it == "format_default") retVal |= std::regex_constants::match_flag_type::format_default;
            if (it == "format_sed") retVal |= std::regex_constants::match_flag_type::format_sed;
            if (it == "format_no_copy") retVal |= std::regex_constants::match_flag_type::format_no_copy;
            if (it == "format_default") retVal |= std::regex_constants::match_flag_type::format_first_only;
        }

        return retVal;
    }

    // generates std::regex_constants::syntax_option_type from strings:
    static std::regex_constants::syntax_option_type get_syntax_options(std::vector<std::string> syntax_options) {
        std::regex_constants::syntax_option_type retVal = (std::regex_constants::syntax_option_type)0;
        for (auto &it : syntax_options) {
            if (it == "icase") retVal |= std::regex_constants::syntax_option_type::icase;
            if (it == "nosubs") retVal |= std::regex_constants::syntax_option_type::nosubs;
            if (it == "optimize") retVal |= std::regex_constants::syntax_option_type::optimize;
            if (it == "collate") retVal |= std::regex_constants::syntax_option_type::collate;
            if (it == "ECMAScript") retVal |= std::regex_constants::syntax_option_type::ECMAScript;
            if (it == "basic") retVal |= std::regex_constants::syntax_option_type::basic;
            if (it == "extended") retVal |= std::regex_constants::syntax_option_type::extended;
            if (it == "awk") retVal |= std::regex_constants::syntax_option_type::awk;
            if (it == "grep") retVal |= std::regex_constants::syntax_option_type::grep;
            if (it == "egrep") retVal |= std::regex_constants::syntax_option_type::egrep;
        }

        return retVal;
    }

    static void smatch_to_table(std::smatch& m, sol::table& t){
        t.clear();

        t["ready"] = m.ready();
        t["empty"] = m.empty();
        t["size"] = m.size();

        if (m.size() > 0) {
            sol::table prefix_table = t.create("prefix");
            prefix_table["length"] = m.prefix().length();
            prefix_table["matched"] = m.prefix().matched;
            prefix_table["str"] = m.prefix().str();

            sol::table suffix_table = t.create("suffix");
            suffix_table["length"] = m.suffix().length();
            suffix_table["matched"] = m.suffix().matched;
            suffix_table["str"] = m.suffix().str();

            sol::table index_table = t.create("index");
            for (int i = 0; i < m.size(); i++) {
                sol::table entry_table = index_table.create();
                entry_table["length"] = m[i].length();
                entry_table["matched"] = m[i].matched;
                entry_table["str"] = m[i].str();
                index_table.add(entry_table);
            }
        }
    }

    static bool _lua_regex_match(std::string str, std::string regex_str, sol::table output, std::vector<std::string> syntax_options, std::vector<std::string> match_flags) {
        PLOGV << regex_str;
        
        std::regex re(regex_str, get_syntax_options(syntax_options));
        std::smatch m;

        bool result = std::regex_match(str, m, re, get_match_flags(match_flags));
        smatch_to_table(m, output);
        
        return result;
    }
    
    // Only returns the first match.
    static bool _lua_regex_search(std::string str, std::string regex_str, sol::table output, std::vector<std::string> syntax_options, std::vector<std::string> match_flags) {
        std::regex re(regex_str, get_syntax_options(syntax_options));
        std::smatch m;

        bool result = std::regex_search(str, m, re, get_match_flags(match_flags));
        smatch_to_table(m, output);

        return result;
    }

    // Returns all matches:
    static bool _lua_regex_search_iter(std::string str, std::string regex_str, sol::table output, std::vector<std::string> syntax_options, std::vector<std::string> match_flags) {
        std::regex re(regex_str, get_syntax_options(syntax_options));

        // Constructin the regex iterator:
        std::sregex_iterator iter_begin(str.begin(), str.end(), re, get_match_flags(match_flags));
        std::sregex_iterator iter_end;
        
        output.clear();
        for (std::sregex_iterator i = iter_begin; i != iter_end; ++i) {
            std::smatch match = *i;
            sol::table match_table = output.create();
            smatch_to_table(match, match_table);
            output.add(match_table);
        }
        
        return output.size() > 0;
    }

    void create_lua_regex_module(sol::state_view* lua) {
        sol::table regex_table = lua->create_named_table("regex");

        regex_table.set_function("match", &_lua_regex_match);
        regex_table.set_function("search", &_lua_regex_search);
        regex_table.set_function("search_iter", &_lua_regex_search_iter);
    }
}