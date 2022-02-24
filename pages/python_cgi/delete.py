import os
if os.path.exists("demofile.txt"):
  os.remove("demofile.txt")
  print("deleted")
else:
  print("The file does not exist")