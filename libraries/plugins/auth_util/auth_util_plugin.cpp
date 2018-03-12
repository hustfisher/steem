

#include <steemit/plugins/auth_util/auth_util_api.hpp>
#include <steemit/plugins/auth_util/auth_util_plugin.hpp>

#include <string>

namespace steemit { namespace plugin { namespace auth_util {

auth_util_plugin::auth_util_plugin( application* app ) : plugin( app ) {}
auth_util_plugin::~auth_util_plugin() {}

std::string auth_util_plugin::plugin_name()const
{
   return "auth_util";
}

void auth_util_plugin::plugin_initialize( const boost::program_options::variables_map& options )
{
}

/**
 * plugin startup时注册auth_util_api.
 * auth_util_api主要提供check_authority_signature功能，其根据出入的authority signature param 构建authority，然后通过sign_state进行验证
 *
 */
void auth_util_plugin::plugin_startup()
{
   app().register_api_factory< auth_util_api >( "auth_util_api" );
}

void auth_util_plugin::plugin_shutdown()
{
}

} } } // steemit::plugin::auth_util

STEEMIT_DEFINE_PLUGIN( auth_util, steemit::plugin::auth_util::auth_util_plugin )
