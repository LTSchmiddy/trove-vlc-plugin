#include "lua_config_access.h"

#include "ns_abbr/fs.h"
#include "assets/asset_globals.h"

namespace Scripting::Extensions::ConfigAccess {

    static std::string _lua_get_movie_poster_path() {
        return Global::asset_manager->getAssetRoot().string();
    }
    
    void create_lua_config_access_module (sol::state_view* lua) {
        sol::table config_access = lua->create_named_table("config_access");
         config_access.set_function("get_asset_root", &_lua_get_movie_poster_path);

    }
}