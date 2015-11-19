<?php
header("Access-Control-Allow-Origin: *");
?>

<?php
    $username = "pocketgames_dk";
    $password = "mHiRL4bV";
    $hostname = "pocketgames.dk.mysql";
    
    $dbhandle = mysql_connect($hostname, $username, $password) or die("Unable to connect to MySQL");
    $selected = mysql_select_db("pocketgames_dk",$dbhandle) or die("Could not select database");
    
    $id = $_GET['id'];
    $level = mysql_real_escape_string($_GET['level']);
    
    echo $id;
    echo $level;
    
    $sql="UPDATE Cubism SET Level='".$level."' WHERE facebookID='".$id."'";
    
    if (!mysql_query($sql)) {
        die('Error: ' . mysql_error($dbhandle));
    }
    
    mysql_close($dbhandle);
?>
