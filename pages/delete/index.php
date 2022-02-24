<?php

if ($_SERVER["REQUEST_METHOD"] == "DELETE")
{
    // $myEnt÷ireBody = file_get_contents('php://input');
    parse_str(file_get_contents('php://input'), $body);
    // $body = json_decode(file_get_contents('php://input'), true);
    var_dump($body["test"]);
    unlink($body["test"]);
}
// in the same folder
?>
