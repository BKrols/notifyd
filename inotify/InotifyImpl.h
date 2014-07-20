#ifndef _INOTIFY_IMPL_H_
#define _INOTIFY_IMPL_H_

#include <memory>
#include <sys/inotify.h>
#include <unistd.h>

#include "Item.h"
#include "Node.h"

class InotifyImpl
{
public:
    #define EVENT_MASK (/*IN_CLOSE_WRITE|*/IN_CREATE|IN_DELETE|IN_DELETE_SELF|/*IN_MODIFY|*/IN_MOVE_SELF|\
                        IN_MOVED_FROM|IN_MOVED_TO|IN_DONT_FOLLOW|IN_ONLYDIR/*|IN_ATTRIB*/)
    #define EVENT_MASK_LITE (IN_CREATE|IN_DELETE)

    static const size_t MAX_WATCHES_NUMBER = 8*1024; // TODO from /proc/sys/fs/inotify/max_user_watches
    static const size_t EVENT_SIZE = sizeof(inotify_event);
    static const size_t BUF_LEN = 1024*(EVENT_SIZE+16);

public:
    InotifyImpl(const char* root_path) : _tree(new Node<Item>(root_path)), _inotify_fd(-1) {}
    ~InotifyImpl() { if(_inotify_fd >= 0) close(_inotify_fd); }

    bool init();

    bool readDir(const char* dir_path, Node<Item>* node);
    bool run();

private:
    std::unique_ptr<Node<Item>> _tree;

    int _inotify_fd;
    std::vector<Node<Item>*> _watches;
};

#endif // _INOTIFY_IMPL_H_
