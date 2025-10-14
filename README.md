# NovaCore
A simple script for requesting Discord applications. So far it only returns the name... because, yeah, it still doesn't do ANYTHING interesting.

compile:
```sh
# If this process shows errors... well, I don't know how to do anything...
cmake . && make
```

use:
```sh
# If this is not used within the project, the ASCII may not show up... but you can create a linker.
./NovaCore <application token>
```
Note: The token must be from your Discord application (bot); if it is from another user (I don't want to know how you got access to it), it will not work.# NovaCore
