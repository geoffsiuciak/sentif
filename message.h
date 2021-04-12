#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <ostream>
#include <vector>
#include <string>


namespace sentif {

class Message {
    
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

} // namespace sentif

#endif // MESSAGE_H
