--TEST--
mysqlx collection modify arrayDelete
--SKIPIF--
--FILE--
<?php
	require("connect.inc");

	$nodeSession = create_test_db();
	$schema = $nodeSession->getSchema($db);
	$coll = $schema->getCollection("test_collection");

	fill_db_collection($coll);


	$coll->modify('job in (\'Programmatore\', \'Cantante\')')->arrayAppend('job', 'Volontario')->arrayAppend('job', 'Tassinaro')->execute();
	$coll->modify('name in (\'Riccardo\', \'Carlo\')')->arrayDelete('job[0]')->execute();
	$coll->modify('name in (\'Alfredo\', \'Leonardo\')')->arrayDelete('job[1]')->execute();
	$coll->modify('name like \'Lonardo\'')->arrayDelete('job[0]')->execute();
	var_dump($coll->find()->execute()->fetchAll());
	print "done!\n";
?>
--CLEAN--
<?php
	require("connect.inc");
	clean_test_db();
?>
--EXPECTF--
array(16) {
  [0]=>
  array(4) {
    ["_id"]=>
    int(1)
    ["age"]=>
    int(19)
    ["job"]=>
    array(3) {
      [0]=>
      string(13) "Programmatore"
      [1]=>
      string(10) "Volontario"
      [2]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(5) "Marco"
  }
  [1]=>
  array(4) {
    ["_id"]=>
    int(10)
    ["age"]=>
    int(29)
    ["job"]=>
    string(11) "Disoccupato"
    ["name"]=>
    string(6) "Giulio"
  }
  [2]=>
  array(4) {
    ["_id"]=>
    int(11)
    ["age"]=>
    int(47)
    ["job"]=>
    string(7) "Barista"
    ["name"]=>
    string(5) "Lucia"
  }
  [3]=>
  array(4) {
    ["_id"]=>
    int(12)
    ["age"]=>
    int(31)
    ["job"]=>
    string(8) "Spazzino"
    ["name"]=>
    string(7) "Filippo"
  }
  [4]=>
  array(4) {
    ["_id"]=>
    int(13)
    ["age"]=>
    int(15)
    ["job"]=>
    string(7) "Barista"
    ["name"]=>
    string(10) "Alessandra"
  }
  [5]=>
  array(4) {
    ["_id"]=>
    int(14)
    ["age"]=>
    int(22)
    ["job"]=>
    array(3) {
      [0]=>
      string(13) "Programmatore"
      [1]=>
      string(10) "Volontario"
      [2]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(7) "Massimo"
  }
  [6]=>
  array(3) {
    ["_id"]=>
    int(15)
    ["age"]=>
    int(37)
    ["name"]=>
    string(5) "Carlo"
  }
  [7]=>
  array(4) {
    ["_id"]=>
    int(16)
    ["age"]=>
    int(23)
    ["job"]=>
    array(2) {
      [0]=>
      string(13) "Programmatore"
      [1]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(8) "Leonardo"
  }
  [8]=>
  array(3) {
    ["_id"]=>
    int(2)
    ["age"]=>
    int(59)
    ["name"]=>
    string(7) "Lonardo"
  }
  [9]=>
  array(4) {
    ["_id"]=>
    int(3)
    ["age"]=>
    int(27)
    ["job"]=>
    array(2) {
      [0]=>
      string(10) "Volontario"
      [1]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(8) "Riccardo"
  }
  [10]=>
  array(4) {
    ["_id"]=>
    int(4)
    ["age"]=>
    int(23)
    ["job"]=>
    string(14) "Programmatrice"
    ["name"]=>
    string(8) "Carlotta"
  }
  [11]=>
  array(4) {
    ["_id"]=>
    int(5)
    ["age"]=>
    int(25)
    ["job"]=>
    array(2) {
      [0]=>
      string(10) "Volontario"
      [1]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(5) "Carlo"
  }
  [12]=>
  array(4) {
    ["_id"]=>
    int(6)
    ["age"]=>
    int(41)
    ["job"]=>
    string(14) "Programmatrice"
    ["name"]=>
    string(10) "Mariangela"
  }
  [13]=>
  array(4) {
    ["_id"]=>
    int(7)
    ["age"]=>
    int(27)
    ["job"]=>
    array(2) {
      [0]=>
      string(13) "Programmatore"
      [1]=>
      string(9) "Tassinaro"
    }
    ["name"]=>
    string(7) "Alfredo"
  }
  [14]=>
  array(4) {
    ["_id"]=>
    int(8)
    ["age"]=>
    int(42)
    ["job"]=>
    string(8) "Studente"
    ["name"]=>
    string(9) "Antonella"
  }
  [15]=>
  array(4) {
    ["_id"]=>
    int(9)
    ["age"]=>
    int(35)
    ["job"]=>
    string(9) "Ballerino"
    ["name"]=>
    string(6) "Monica"
  }
}
done!%A