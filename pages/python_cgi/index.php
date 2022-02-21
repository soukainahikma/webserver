<h1> sdfsdigcvfchsdh </h1>
<?php 
session_start();
var_dump($_POST);
setcookie("name", "choaib");
$_SESSION["newsession"] = "session";
if ($_SERVER["REQUEST_METHOD"] == 'DELETE')
{
    echo "HELLO <br>";
    $file_pointer = "test_delete.txt"; 
   
    // Use unlink() function to delete a file 
    if (!unlink($file_pointer)) { 
        echo ("$file_pointer cannot be deleted due to an error"); 
    } 
    else { 
        echo ("$file_pointer has been deleted"); 
    } 

}
    // phpinfo();
?>