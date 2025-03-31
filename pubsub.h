#include <stdbool.h>

typedef struct PubsubPublisher
{
    char topic[20];
    void* data;
} PubsubPublisher;

typedef void* (*subscriber_cb_t)(void*);
typedef struct PubsubSubscriber {
    char topic[20];
    subscriber_cb_t subscriber_cb_fptr;
    void* sub_data;
}PubsubSubscriber;

bool pubsub_create(void);
bool pubsub_publish(PubsubPublisher event);
bool pubsub_subscribe(PubsubSubscriber sub);
