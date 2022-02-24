<?php

if ($_SERVER["REQUEST_METHOD"] == "DELETE")
{
    // $myEnt÷ireBody = file_get_contents('php://input');
    parse_str(file_get_contents('php://input'), $myEntireBody);
    // $myEntireBody = json_decode(file_get_contents('php://input'), true);
    var_dump($myEntireBody["test"]);
    unlink($myEntireBody["test"]);
}
// in the same folder
?>
