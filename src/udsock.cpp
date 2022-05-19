
#include <limits.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>



class Component
{
};
class System 
{
    virtual void Execute() = 0;
};

class UDS;
class Listener : public System {
    
    private:
        const int& _f;
    public:
        virtual void Execute()
        {

        }
        Listener(UDS&uds) : _f(uds.Get()){}


};

class UDS : public Component {
    protected:
    struct sockaddr_un local;
    int _f{socket(AF_UNIX, SOCK_STREAM,0)};
    public:
    const int& Get()
    {
        return _f;
    }
    bool IsValid()
    {
        return _f < 0 ? false : true;
    }
    UDS()
    {
        std::memset(&local, 0, sizeof(struct sockaddr_un));
        local.sun_family = AF_UNIX;
    }
};



void foo()
{

}
