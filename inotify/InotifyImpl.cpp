#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sstream>
#include <string.h>

#include "InotifyImpl.h"

bool InotifyImpl::init()
{
    _inotify_fd = inotify_init();
    if (_inotify_fd < 0)
    {
        fprintf(stderr,
            "InotifyImpl::init(): inotify_init error - %s\n", strerror(errno));
        return false;
    }

    _watches.resize(MAX_WATCHES_NUMBER);
    bool ret = readDir(_tree->getValue().getName().c_str(), _tree.get());
    if (ret)
        _tree->print();

    return ret;
}

bool InotifyImpl::readDir(const char* dir_path, Node<Item>* node)
{
    if (!node)
        return false;

    int wd = inotify_add_watch(_inotify_fd, dir_path, EVENT_MASK);
    if (wd < 0)
    {
        fprintf(stderr,
            "InotifyImpl::readDir(): adding watch failed - %s\n", strerror(errno));
        return false;
    }
    _watches[wd] = node;

    DIR* dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr,
            "InotifyImpl::readDir(): open dir failed - %s\n", strerror(errno));
        return false;
    }

    struct dirent* dir_content;
    while((dir_content = readdir(dir)) != NULL)
    {
        if (!strcmp(dir_content->d_name, ".") || !strcmp(dir_content->d_name, ".."))
            continue;

        std::stringstream f;
        f << dir_path << "/" << dir_content->d_name;
        struct stat st;
        if (lstat(f.str().c_str(), &st)) {
            fprintf(stderr,
                "InotifyImpl::readDir(): сannot lstat - %s\n", strerror(errno));
            closedir(dir);
            return false;
        }

        if (S_ISDIR(st.st_mode))
            readDir(f.str().c_str(), node->addChild(f.str().c_str()));
        else
            node->addChild(f.str().c_str());
    }

    closedir(dir);
    return true;
}

bool InotifyImpl::run()
{
    char buffer[BUF_LEN];

    int length = 0;
    while( (length = read(_inotify_fd, buffer, BUF_LEN)) > 0 )
    {
        const char* buffer_it = buffer;
        while( (buffer_it - buffer) < length )
        {
            inotify_event* event = (inotify_event*)buffer_it;
            buffer_it += EVENT_SIZE + event->len;
            switch(event->mask & EVENT_MASK)
            {
                case IN_Q_OVERFLOW: printf("IN_Q_OVERFLOW: %s\n", event->name); break;
                case IN_IGNORED: printf("IN_IGNORED: %s\n", event->name); break;

                case IN_MOVED_TO: /*printf("IN_MOVED_TO: %s\n", event->name); break;*/
                case IN_CREATE: printf("IN_CREATE || IN_MOVED_TO: %s\n", event->name);
                {
                    auto* node = _watches[event->wd];
                    std::stringstream fn;
                    fn << node->getValue().getName() << '/' << event->name;

                    if (event->mask & IN_ISDIR)
                        readDir(fn.str().c_str(),
                                node->addChild(fn.str().c_str()));
                    else
                        node = node->addChild(fn.str().c_str());
                }
                break;

                case IN_MOVED_FROM: /*printf("IN_MOVED_FROM: %s\n", event->name); break;*/
                case IN_DELETE: printf("IN_DELETE || IN_MOVED_FROM: %s\n", event->name);
                {
                    auto* node = _watches[event->wd];
                    std::stringstream fn;
                    fn << node->getValue().getName() << '/' << event->name;

                    node->removeChild(fn.str().c_str());
                }
                break;

                case IN_DELETE_SELF: /*printf("IN_DELETE_SELF: wd: %i\n", event->wd); break;*/
                case IN_MOVE_SELF: printf("IN_DELETE_SELF || IN_MOVE_SELF: wd: %i\n", event->wd);
                {
                    auto* node = _watches[event->wd];

                    // order of events: self: IN_DELETE_SELF, parent: IN_DELETE
                    // therefore only nulled pointer in _watches array
                    // delete it from cases IN_MOVED_FROM: IN_DELETE:
                    _watches[event->wd] = 0;

                    if (node == _tree.get())
                    {
                        // root dir deleted, not need main loop...
                        printf("Root dir was deleted or moved. Exit...\n");
                        return true;
                    }
                }
                break;
                default: break;
            }

            _tree->print();
        }
    }

    return true;
}
