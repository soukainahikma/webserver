#!/usr/bin/python3
import os

# Hello world python program

page = ''
get_request = ''
handler = {}
if 'QUERY_STRING' in os.environ:
    get_request = os.environ['QUERY_STRING']
    get_request = get_request.split('&')
# if 'HTTP_COOKIE' in os.environ:
#     cookies = os.environ['HTTP_COOKIE']
#     cookies = cookies.split('&')

    for get in get_request:
        get = get.split('=')
        handler[get[0]] = get[1]

for k in handler:
    page += "<p> <span style='font-weight: bold'>" + k + "</span>: " + handler[k] + " </p>"
    print ('Set-Cookie:' + k + '=' + handler[k])

print ("Content-type:text/html\r\n\r\n")
print (page)
