#include <assert.h>
#include <pthread.h>

#include "channels.h"

int main() {
  channel_ty *chan = (channel_ty *)chan_create();
  chan_send(chan, (int64_t *)(10));
  chan_send(chan, (int64_t *)(20));
  chan_send(chan, (int64_t *)(30));
  assert(chan_recv(chan) == (int64_t *)(10));
  assert(chan_recv(chan) == (int64_t *)(20));
  assert(chan_recv(chan) == (int64_t *)(30));
}
