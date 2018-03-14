/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Andrey Hristov <andrey@php.net>                             |
  +----------------------------------------------------------------------+
*/
#ifndef MYSQLX_NODE_TABLE__SELECT_H
#define MYSQLX_NODE_TABLE__SELECT_H

namespace Mysqlx { namespace Crud { class Find; } }

namespace mysqlx {

namespace devapi {

extern zend_class_entry* mysqlx_table__select_class_entry;

void mysqlx_new_node_table__select(zval * return_value,
						drv::st_xmysqlnd_table* schema,
						const zend_bool clone,
						zval * columns,
						const int num_of_columns);
void mysqlx_register_node_table__select_class(INIT_FUNC_ARGS, zend_object_handlers * mysqlx_std_object_handlers);
void mysqlx_unregister_node_table__select_class(SHUTDOWN_FUNC_ARGS);

Mysqlx::Crud::Find* get_stmt_from_table_select(zval* object_zv);

} // namespace devapi

} // namespace mysqlx

#endif /* MYSQLX_NODE_TABLE__SELECT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */