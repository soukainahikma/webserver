from os import environ
import cgi, cgitb
if environ.has_key('HTTP_COOKIE'):
   print (environ['HTTP_COOKIE'])
