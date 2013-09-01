#include <lo/lo.h>
#include <stdlib.h>
#include <zmq.h>
#include <assert.h>

#define MAXSZ 1024

lo_server s;

void error(int num, const char *msg, const char *path) {
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
}

/**/

int generic_handler(const char *path, const char *types, lo_arg **argv,
		    int argc, lo_message msg, void *user_data) {
    int i;
    int sz = lo_message_length(msg, path);
    printf("path: <%s> size: %d\n", path, sz);
    for (i=0; i<argc; i++) {
      printf("arg %d '%c' ", i, types[i]);
      lo_arg_pp(types[i], argv[i]);
      printf("\n");
    }
    printf("\n");

    return 1;
}

void osc_init() {
  s = lo_server_new("12321", error);
  lo_server_add_method(s, NULL, NULL, generic_handler, NULL);
}

int main (int argc, char *argv [])
{
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    void *buffer = (void *) malloc(MAXSZ);
    lo_message msg;
    int rc = zmq_connect (subscriber, "tcp://localhost:5556");
    assert (rc == 0);

    //  Subscribe to all
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE,
			NULL, 0);
    assert (rc == 0);

    osc_init();
    while(1) {
      int size = zmq_recv(subscriber, buffer, MAXSZ, 0);
      msg = (lo_message) buffer;
      if (size > 0) {
	printf("received a message that's %d big\n", size);
	lo_server_dispatch_data(s, buffer, size);
      }
      else {
	printf("oops.\n");
      }
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
