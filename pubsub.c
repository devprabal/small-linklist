#include "pubsub.h"

#include "list.h"
#include "node.h"
#include "stdbool.h"

#define DBG printf("\n%s():L%d\n", __func__, __LINE__)

static Head* subscriber_list = NULL;

bool pubsub_create(void)
{
    subscriber_list = create_list(NULL);
    if (subscriber_list) {
        return true;
    }
    return false;
}

static bool compare_topic(void* t1, void* t2)
{
    char* topic1 = (char*)t1;
    char* topic2 = (char*)t2;
    if (topic1 && topic2) {
        if (strlen(topic1) != strlen(topic2)) {
            return false;
        }
        if (0 == strncmp(topic1, topic2, strlen(topic1))) {
            return true;
        }
    }
    return false;
}

bool pubsub_publish(PubsubPublisher event) {
    if(subscriber_list) {
        if(true == find_in_list(subscriber_list, event.topic, compare_topic)){
            printf("\nfound\n");
        } else {
            DBG;
        }
    } else {
        DBG;
    }
    return true;
}

bool pubsub_subscribe(PubsubSubscriber sub)
{
    if (SLL_SUCCESS == append_to_list(subscriber_list, sizeof(sub), &sub)) {
        DBG;
        return true;
    } else {
        DBG;
    }
    return false;
}
