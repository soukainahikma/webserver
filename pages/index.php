<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <div><?php var_dump($_POST); ?></div>
    <!-- <div><?php var_dump($_GET); ?></div> -->
</body>
</html>
<!-- 

 ------ GET REQUEST EXAMPLE --------
GET /?key=value HTTP/1.1
User-Agent: PostmanRuntime/7.25.0
Accept: */*
Postman-Token: 71ba13ca-8eb4-44aa-b6e9-2da78487ac48
Host: 0.0.0.0:5000
Accept-Encoding: gzip, deflate, br
Connection: keep-alive

 ------ POST REQUEST EXAMPLE --------
POST / HTTP/1.1
Transfer-Encoding: chunked
User-Agent: PostmanRuntime/7.25.0
Accept: */*
Postman-Token: 71ba13ca-8eb4-44aa-b6e9-2da78487ac48
Host: 0.0.0.0:5000
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: application/x-www-form-urlencoded
Content-Length: 15


f
test=delete.txt
0

 ------ DELETE REQUEST EXAMPLE --------
POST /delete/index.php HTTP/1.1
Transfer-Encoding: chunked
User-Agent: PostmanRuntime/7.25.0
Accept: */*
Postman-Token: 71ba13ca-8eb4-44aa-b6e9-2da78487ac48
Host: 0.0.0.0:5000
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: application/x-www-form-urlencoded
Content-Length: 15


f
test=delete.txt
0



 -->