

#include <steemit/plugins/raw_block/raw_block_api.hpp>
#include <steemit/plugins/raw_block/raw_block_plugin.hpp>

#include <string>

namespace steemit { namespace plugin { namespace raw_block {

raw_block_plugin::raw_block_plugin( application* app ) : plugin( app ) {}
raw_block_plugin::~raw_block_plugin() {}

std::string raw_block_plugin::plugin_name()const
{
   return "raw_block";
}

void raw_block_plugin::plugin_initialize( const boost::program_options::variables_map& options )
{
}

void raw_block_plugin::plugin_startup()
{
   app().register_api_factory< raw_block_api >( "raw_block_api" );
}

void raw_block_plugin::plugin_shutdown()
{
}

} } } // steemit::plugin::raw_block

/* 预编译替换后最终为：create_raw_block_plugin( app::application* app ) {std::make_shared< raw_block_plugin >( app );} */
STEEMIT_DEFINE_PLUGIN( raw_block, steemit::plugin::raw_block::raw_block_plugin )
