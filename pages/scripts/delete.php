<?php

if ($_SERVER["REQUEST_METHOD"] == "POST")
{
    // $myEnt÷ireBody = file_get_contents('php://input');
    parse_str(file_get_contents('php://input'), $body);
    // $myEntireBody = json_decode(file_get_contents('php://input'), true);
    var_dump($body["text"]);
    unlink($body["text"]);
}
// in the same folder
?>
