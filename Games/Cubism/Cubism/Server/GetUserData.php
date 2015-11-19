<?php
header("Access-Control-Allow-Origin: *");
?>

<?php
$username = "pocketgames_dk";
$password = "mHiRL4bV";
$hostname = "pocketgames.dk.mysql"; 
    
//connection to the database
$dbhandle = mysql_connect($hostname, $username, $password) or die("Unable to connect to MySQL");

//select a database to work with
$selected = mysql_select_db("pocketgames_dk",$dbhandle) or die("Could not select database");

$id = $_GET['id'];

//execute the SQL query and return records
$result = mysql_query("SELECT facebookID, Level FROM Cubism WHERE facebookID='".$id."'");

$rows = array();
while($r = mysql_fetch_assoc($result)) {
    $rows[] = $r;
}
    
$str = json_encode($rows);
    
echo $str;
mysql_close($dbhandle);

?>