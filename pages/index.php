<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
<?php 
    echo "COOOKIE <br>";
    var_dump($_COOKIE);
if ($_POST) { ?>
    <h1><?php var_dump($_POST) ?></h1>
<? } else { ?>
    <h1>NO POST REQUEST</h1>
<? } ?>
</body>
</html>