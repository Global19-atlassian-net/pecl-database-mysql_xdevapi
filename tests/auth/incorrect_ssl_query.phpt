--TEST--
mysqlx authentication mechanisms - incorrect ssl query
--SKIPIF--
--INI--
error_reporting=1
default_socket_timeout=4
--FILE--
<?php
require_once(__DIR__."/../connect.inc");
require_once(__DIR__."/auth_utils.inc");

test_incorrect_connection($disable_ssl_opt.'&');
test_incorrect_connection($ssl_query.'&=');
test_incorrect_connection($disable_ssl_opt.'&=&');
test_incorrect_connection($ssl_query.'&&&');
test_incorrect_connection($disable_ssl_opt.'&=mysql41&');
test_incorrect_connection($ssl_query.'&auth&');
test_incorrect_connection($disable_ssl_opt.'&auth');
test_incorrect_connection($ssl_query.'&auth=');
test_incorrect_connection($disable_ssl_opt.'&auth==');
test_incorrect_connection($ssl_query.'&auth==&');
test_incorrect_connection($disable_ssl_opt.'&auth=plain&&');
test_incorrect_connection($ssl_query.'&&auth=&');
test_incorrect_connection($disable_ssl_opt.'&&auth=mysql41');
test_incorrect_connection($ssl_query.'&&auth&=plain');
test_incorrect_connection($disable_ssl_opt.'&auth=plain&&auth=external');
test_incorrect_connection($ssl_query.'auth=plain');
test_incorrect_connection($disable_ssl_opt.'auth=');
test_incorrect_connection($ssl_query.'auth=sha256_memory&');
test_incorrect_connection($disable_ssl_opt.'&auth=sha256_memory&&&');
test_incorrect_connection('&'.$ssl_query);
test_incorrect_connection('&'.$disable_ssl_opt);
test_incorrect_connection('&'.$disable_ssl_opt.'&auth=mysql41');
test_incorrect_connection('&'.$disable_ssl_opt.'auth=sha256_memory&');

verify_expectations();
print "done!\n";
?>
--CLEAN--
<?php
	require_once(__DIR__."/../connect.inc");
	clean_test_db();
?>
--EXPECTF--
done!%A