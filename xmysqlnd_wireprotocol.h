/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Andrey Hristov <andrey@mysql.com>                           |
  +----------------------------------------------------------------------+
*/

#ifndef XMYSQLND_WIREPROTOCOL_H
#define XMYSQLND_WIREPROTOCOL_H

#include "mysqlx_node_connection.h"
#include "mysqlx_node_pfc.h"

#ifdef __cplusplus

#include "proto_gen/mysqlx.pb.h"

extern "C" size_t
xmysqlnd_send_protobuf_message(struct st_mysqlx_node_connection * connection, struct st_mysqlx_node_pfc * codec,
							   const Mysqlx::ClientMessages::Type packet_type, ::google::protobuf::Message & proto_message,
							   const bool simulate);

extern "C"
{
#endif


void xmysqlnd_dump_server_message(const zend_uchar packet_type, const void * payload, const size_t payload_size);
void xmysqlnd_dump_client_message(const zend_uchar packet_type, const void * payload, const size_t payload_size);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif	/* XMYSQLND_WIREPROTOCOL_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */