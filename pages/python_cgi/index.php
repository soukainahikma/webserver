<h1> sdfsdigcvfchsdh </h1>
<?php 
// session_start();
// var_dump($_POST);

// function parse_raw_http_request()
// {
//   $data = [];
//   $input = file_get_contents('php://input');

//   // grab multipart boundary from content type header
//   preg_match('/boundary=(.*)$/', $_SERVER['CONTENT_TYPE'], $matches);
//   $boundary = $matches[1];

//   // split content by boundary and get rid of last -- element
//   $a_blocks = preg_split("/-+$boundary/", $input);
//   array_pop($a_blocks);

//   // loop data blocks
//   foreach ($a_blocks as $id => $block)
//   {
//     if (empty($block))
//       continue;

//     // you'll have to var_dump $block to understand this and maybe replace \n or \r with a visibile char

//     // parse uploaded files
//     if (strpos($block, 'application/octet-stream') !== FALSE)
//     {
//       // match "name", then everything after "stream" (optional) except for prepending newlines 
//       preg_match("/name=\"([^\"]*)\".*stream[\n|\r]+([^\n\r].*)?$/s", $block, $matches);
//     }
//     // parse all other fields
//     else
//     {
//       // match "name" and optional value in between newline sequences
//       preg_match('/name=\"([^\"]*)\"[\n|\r]+([^\n\r].*)?\r$/s', $block, $matches);
//     }
//     $data[$matches[1]] = $matches[2];
//     return (data);
//   }        
// }


var_dump($_GET);
var_dump($_POST);
var_dump($_FILES);
// setcookie("name", "choaib");
// $_SESSION["newsession"] = "session";
// $data = parse_raw_http_request();
// // parse_str(file_get_contents("php://input"),$data);
// var_dump(file_get_contents("php://input"));
// var_dump($_SERVER["CONTENT_TYPE"]);
// if ($_SERVER["CONTENT_TYPE"] == 'multipart/form-data')
// {
//     echo "HELLO <br>";   
//     // $data = [];
//     // parse_str(file_get_contents("php://input"),$data);
//     // var_dump($data);
//     // // Use unlink() function to delete a file 
//     // if (!unlink($data["filename"])) { 
//     //     echo ($data["filename"] . "cannot be deleted due to an error"); 
//     // } 
//     // else { 
//     //     echo ($data["filename"] . " has been deleted"); 
//     // } 

// }
    // phpinfo();
?>