#ifndef MSG_CONTAINER_H
#define MSG_CONTAINER_H

#include <iostream>
#include <ostream>
#include <vector>
#include <string>


namespace http {

    class Message
    {
        friend std::ostream &operator<<(std::ostream &, const Message &);

    public:
        Message();
        Message(const std::string &);
        Message(const char *buffer);

        void add_msg_content(char *);
        void add_msg_content(const std::string&);
        void print_msg() const;
        void reset();

        const std::vector<char>& get_msg_content();

    private:
        std::vector<char> msg;
    };

}


#endif
