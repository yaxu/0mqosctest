0mqosctest
==========
Proof of concept for passing osc via zeromq (http://zeromq.org)

* test.hs - a haskell program for sending a test osc message
* server.c - a c program that receives that osc message and publishes it to a zeromq socket
* client.c - another c program that subscribes to the zeromq messages, and processes them as osc

compile the c with something like gcc client.c -g -llo -lzmq -o client
