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
#include "php.h"
#include "ext/mysqlnd/mysqlnd.h"
#include "ext/mysqlnd/mysqlnd_debug.h"
#include "ext/mysqlnd/mysqlnd_alloc.h"
#include "ext/mysqlnd/mysqlnd_statistics.h"
#include "xmysqlnd.h"
#include "xmysqlnd_node_session.h"
#include "php_mysqlx.h"
#include "mysqlx_class_properties.h"
#include "mysqlx_node_connection.h"
#include "mysqlx_node_pfc.h"

#include "xmysqlnd_wireprotocol.h"

#include <new>
#include "proto_gen/mysqlx_session.pb.h"

#include "mysqlx_message__ok.h"
#include "mysqlx_message__error.h"
#include "mysqlx_message__stmt_execute.h"
#include "mysqlx_message__stmt_execute_ok.h"
#include "mysqlx_resultset__column_metadata.h"
#include "mysqlx_resultset__data_row.h"
#include "mysqlx_message__data_fetch_done.h"

static zend_class_entry *mysqlx_message__stmt_execute_class_entry;

struct st_mysqlx_message__stmt_execute
{
	zend_bool persistent;
};

#define MYSQLX_FETCH_MESSAGE__STMT_EXECUTE_FROM_ZVAL(_to, _from) \
{ \
	struct st_mysqlx_object * mysqlx_object = Z_MYSQLX_P((_from)); \
	(_to) = (struct st_mysqlx_message__stmt_execute *) mysqlx_object->ptr; \
	if (!(_to)) { \
		php_error_docref(NULL, E_WARNING, "invalid object or resource %s", ZSTR_VAL(mysqlx_object->zo.ce->name)); \
		RETVAL_NULL(); \
		DBG_VOID_RETURN; \
	} \
} \

#if 0
/* */
ZEND_BEGIN_ARG_INFO_EX(mysqlx_message__stmt_execute__send, 0, ZEND_RETURN_VALUE, 5)
	ZEND_ARG_TYPE_INFO(0, namespace_, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, stmt, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, compact_metadata, IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, node_pfc, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, node_connection, IS_OBJECT, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(mysqlx_message__stmt_execute__read_response, 0, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_TYPE_INFO(0, node_pfc, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, node_connection, IS_OBJECT, 0)
ZEND_END_ARG_INFO()


/* {{{ proto long mysqlx_message__stmt_execute::send(object messsage, string auth_mechanism, string auth_data, object pfc, object connection) */
PHP_METHOD(mysqlx_message__stmt_execute, send)
{
	zval * message_zv;
	zval * codec_zv;
	zval * connection_zv;
	char * namespace_ = NULL;
	size_t namespace_len = 0;
	char * stmt = NULL;
	size_t stmt_len = 0;
	zend_bool compact_metadata;
	struct st_mysqlx_node_connection * connection;
	struct st_mysqlx_node_pfc * codec;

	DBG_ENTER("mysqlx_message__stmt_execute::send");
	if (FAILURE == zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "OssbOO",
												&message_zv, mysqlx_message__stmt_execute_class_entry,
												&namespace_, &namespace_len,
												&stmt, &stmt_len,
												&compact_metadata, 
												&codec_zv, mysqlx_node_pfc_class_entry,
												&connection_zv, mysqlx_node_connection_class_entry))
	{
		DBG_VOID_RETURN;
	}

	MYSQLX_FETCH_NODE_PFC_FROM_ZVAL(codec, codec_zv);
	MYSQLX_FETCH_NODE_CONNECTION_FROM_ZVAL(connection, connection_zv);

	{
		Mysqlx::Sql::StmtExecute proto_message;
		proto_message.set_namespace_(namespace_, namespace_len);
		proto_message.set_stmt(stmt, stmt_len);
		proto_message.set_compact_metadata(compact_metadata);

		RETVAL_LONG(xmysqlnd_send_protobuf_message(connection, codec, Mysqlx::ClientMessages_Type_SQL_STMT_EXECUTE, proto_message, false));
	}
	DBG_VOID_RETURN;
}
/* }}} */


/* {{{ proto long mysqlx_message__stmt_execute::read_response(object messsage, object pfc, object connection) */
PHP_METHOD(mysqlx_message__stmt_execute, read_response)
{
	zval * object_zv;
	zval * codec_zv;
	zval * connection_zv;
	struct st_mysqlx_message__stmt_execute * object;
	struct st_mysqlx_node_connection * connection;
	struct st_mysqlx_node_pfc * codec;
	size_t ret = 0;

	DBG_ENTER("mysqlx_message__stmt_execute::read_response");
	if (FAILURE == zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "OOO",
												&object_zv, mysqlx_message__stmt_execute_class_entry,
												&codec_zv, mysqlx_node_pfc_class_entry,
												&connection_zv, mysqlx_node_connection_class_entry))
	{
		DBG_VOID_RETURN;
	}

	MYSQLX_FETCH_MESSAGE__STMT_EXECUTE_FROM_ZVAL(object, object_zv);
	MYSQLX_FETCH_NODE_PFC_FROM_ZVAL(codec, codec_zv);
	MYSQLX_FETCH_NODE_CONNECTION_FROM_ZVAL(connection, connection_zv);

	RETVAL_FALSE;

	{
		zend_uchar packet_type;
		do {
			size_t payload_size;
			zend_uchar * payload;
			ret = codec->pfc->data->m.receive(codec->pfc, connection->vio,
											  &packet_type,
											  &payload, &payload_size,
											  connection->stats,
											  connection->error_info);
			if (ret == PASS) {
				const Mysqlx::ServerMessages_Type type = (Mysqlx::ServerMessages_Type)(packet_type);
				switch (type) {
					case Mysqlx::ServerMessages_Type_ERROR: {
						Mysqlx::Error error;
						error.ParseFromArray(payload, payload_size);
						mysqlx_new_message__error(return_value, error);
						break;
					}
					case Mysqlx::ServerMessages_Type_NOTICE:
						break;
					case Mysqlx::ServerMessages_Type_SQL_STMT_EXECUTE_OK: {
						Mysqlx::Sql::StmtExecuteOk message;
						message.ParseFromArray(payload, payload_size);
						mysqlx_new_stmt_execute_ok(return_value, message);
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_COLUMN_META_DATA: {
						Mysqlx::Resultset::ColumnMetaData message;
						message.ParseFromArray(payload, payload_size);
						mysqlx_new_column_metadata(return_value, message);
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_ROW: {
						Mysqlx::Resultset::Row message;
						message.ParseFromArray(payload, payload_size);
						mysqlx_new_data_row(return_value, message);
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_FETCH_SUSPENDED: {
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_FETCH_DONE: {
						Mysqlx::Resultset::FetchDone message;
						message.ParseFromArray(payload, payload_size);
						mysqlx_new_data_fetch_done(return_value, message);
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_FETCH_DONE_MORE_RESULTSETS: {
						break;
					}
					case Mysqlx::ServerMessages_Type_RESULTSET_FETCH_DONE_MORE_OUT_PARAMS: {
						Mysqlx::Resultset::Row message;
						message.ParseFromArray(payload, payload_size);
//						mysqlx_new_column_metadata(return_value, message);
						break;
					}
					default:
						php_error_docref(NULL, E_WARNING, "Returned unexpected packet type %s", Mysqlx::ServerMessages_Type_Name(type).c_str());
				}
				mnd_efree(payload);
			}
		} while (packet_type == Mysqlx::ServerMessages_Type_NOTICE);
	}
	
	DBG_VOID_RETURN;
}
/* }}} */


/* {{{ mysqlx_message__stmt_execute_methods[] */
static const zend_function_entry mysqlx_message__stmt_execute_methods[] = {
	PHP_ME(mysqlx_message__stmt_execute, send,				NULL,			ZEND_ACC_PUBLIC)
	PHP_ME(mysqlx_message__stmt_execute, read_response,		mysqlx_message__stmt_execute__read_response,	ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */


static zend_object_handlers mysqlx_object_message__stmt_execute_handlers;
static HashTable mysqlx_message__stmt_execute_properties;

/* {{{ mysqlx_message__stmt_execute_free_storage */
static void
mysqlx_message__stmt_execute_free_storage(zend_object * object)
{
	struct st_mysqlx_object * mysqlx_object = mysqlx_fetch_object_from_zo(object);
	struct st_mysqlx_message__stmt_execute * message = (struct st_mysqlx_message__stmt_execute  *) mysqlx_object->ptr;

	delete message;
	mysqlx_object_free_storage(object); 
}
/* }}} */


/* {{{ php_mysqlx_message__stmt_execute_object_allocator */
static zend_object *
php_mysqlx_message__stmt_execute_object_allocator(zend_class_entry * class_type)
{
	const zend_bool persistent = FALSE;
	struct st_mysqlx_object * mysqlx_object = (struct st_mysqlx_object *) mnd_pecalloc(1, sizeof(struct st_mysqlx_object) + zend_object_properties_size(class_type), persistent);
	struct st_mysqlx_message__stmt_execute * message = new (std::nothrow) struct st_mysqlx_message__stmt_execute();

	DBG_ENTER("php_mysqlx_message__stmt_execute_object_allocator");
	if (!mysqlx_object || !message) {
		goto err;
	}
	mysqlx_object->ptr = message;

	message->persistent = persistent;
	zend_object_std_init(&mysqlx_object->zo, class_type);
	object_properties_init(&mysqlx_object->zo, class_type);

	mysqlx_object->zo.handlers = &mysqlx_object_message__stmt_execute_handlers;
	mysqlx_object->properties = &mysqlx_message__stmt_execute_properties;

	DBG_RETURN(&mysqlx_object->zo);

err:
	if (mysqlx_object) {
		mnd_pefree(mysqlx_object, persistent);
	}
	delete message;
	DBG_RETURN(NULL);
}
/* }}} */


/* {{{ mysqlx_register_message__stmt_execute_class */
extern "C" void
mysqlx_register_message__stmt_execute_class(INIT_FUNC_ARGS, zend_object_handlers * mysqlx_std_object_handlers)
{
	mysqlx_object_message__stmt_execute_handlers = *mysqlx_std_object_handlers;
	mysqlx_object_message__stmt_execute_handlers.free_obj = mysqlx_message__stmt_execute_free_storage;

	{
		zend_class_entry tmp_ce;
		INIT_CLASS_ENTRY(tmp_ce, "mysqlx_message__stmt_execute", mysqlx_message__stmt_execute_methods);
//		INIT_NS_CLASS_ENTRY(tmp_ce, "mysqlx", "node_pfc", mysqlx_message__stmt_execute_methods);
		tmp_ce.create_object = php_mysqlx_message__stmt_execute_object_allocator;
		mysqlx_message__stmt_execute_class_entry = zend_register_internal_class(&tmp_ce);
	}

	zend_hash_init(&mysqlx_message__stmt_execute_properties, 0, NULL, mysqlx_free_property_cb, 1);
}
/* }}} */


/* {{{ mysqlx_unregister_message__stmt_execute_class */
extern "C" void
mysqlx_unregister_message__stmt_execute_class(SHUTDOWN_FUNC_ARGS)
{
	zend_hash_destroy(&mysqlx_message__stmt_execute_properties);
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */