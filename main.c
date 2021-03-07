#include <assert.h>
#include <pthread.h>
#include <stdio.h>

#include "channels.h"

#define MAX_NUM 100000

void single_thread() {
  channel_ty *chan = (channel_ty *)chan_create();
  chan_send(chan, (int64_t *)(10));
  chan_send(chan, (int64_t *)(20));
  chan_send(chan, (int64_t *)(30));
  assert(chan_recv(chan) == (int64_t *)(10));
  assert(chan_recv(chan) == (int64_t *)(20));
  assert(chan_recv(chan) == (int64_t *)(30));
}

void *sender(void *c) {
  channel_ty *chan = (channel_ty *)(c);
  for (int64_t i = 0; i < MAX_NUM; i++) {
    chan_send(chan, (int64_t *)(i));
  }
  return NULL;
}

void *recv(void *c) {
  channel_ty *chan = (channel_ty *)(c);
  int seen[MAX_NUM];
  for (int i = 0; i < MAX_NUM; i++) {
    seen[i] = 0;
  }

  for (int i = 0; i < MAX_NUM * 2; i++) {
    int x = (int)chan_recv(chan);
    seen[x] += 1;
  }

  for (int i = 0; i < MAX_NUM; i++) {
    assert(seen[i] == 2);
  }
  return NULL;
}

void multi_thread() {
  channel_ty *chan = (channel_ty *)chan_create();

  // 2 senders, 1 consumer

  pthread_t send_1;
  pthread_t send_2;
  pthread_t consume_1;

  pthread_create(&send_1, NULL, sender, (void *)chan);
  pthread_create(&send_2, NULL, sender, (void *)chan);
  pthread_create(&consume_1, NULL, recv, (void *)chan);

  pthread_join(send_1, NULL);
  pthread_join(send_2, NULL);
  pthread_join(consume_1, NULL);
}

int main() {
  single_thread();
  multi_thread();
}
