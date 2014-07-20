//-----------------------------------------------------------------------------
#ifdef TEST
#include "Test.h"
#endif

#include "InotifyImpl.h"

//-----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
#ifdef TEST
	test();
    return 0;
#endif // TEST

    InotifyImpl inotify("./temp");
    if (inotify.init())
        inotify.run();

    return 0;
}

//-----------------------------------------------------------------------------
