#pragma once

#include <map>
#include <string>
#include <utility>

#include <fc/api.hpp>

namespace fc { namespace rpc {

class websocket_api_connection;

} }

namespace steemit { namespace app {

class application;

/**
 * Contains state shared by all API's on the same connection.
 * Anything in here is owned by FC and cleaned up when the connection dies.
 * 包含连接整个生命周期中的各种共享状态数据，在连接关闭时由FC清理；
 */

struct api_session_data
{
   /* websocket api connection */
   std::shared_ptr< fc::rpc::websocket_api_connection >        wsc;
   std::map< std::string, fc::api_ptr >                        api_map;
};

/**
 * Contains information needed by API class constructors.
 */

struct api_context
{
   api_context( application& _app, const std::string& _api_name, std::weak_ptr< api_session_data > _session );

   application&                              app;
   std::string                               api_name;
   std::weak_ptr< api_session_data >         session;
};

} }
