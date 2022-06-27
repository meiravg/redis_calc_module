# redis_calc_module

## Please read calc.h for special module notes

### creating and running the module in your local redis:

To **create** module run:

 `make`

To **run** redis server & **load module** run:

`make run_server SERVER_PATH=/your/local/redis/server/path`

**or**

change SERVER_PATH variable to your local redis server path
run:

`make run_server`

To **remove** module run:

`make clean`
