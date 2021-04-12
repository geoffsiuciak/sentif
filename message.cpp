#include "message.h"


namespace http {


Message::Message()
{
    // anything?
}


Message::Message(const std::string& buffer)
{
    /* call only if the entire message is
       contained within buffer */
    add_msg_content(buffer);
}


Message::Message(const char* buffer)
{
    /* call only if the entire message is
       contained within buffer */
    add_msg_content(buffer);
}


void Message::add_msg_content(char* buffer)
{
    char* ptr = buffer;
    while (ptr) {
        this->msg.push_back(*ptr++);
    }
}


void Message::add_msg_content(const std::string& buffer)
{
    for (char c : buffer) {
        this->msg.push_back(c);
    }
}


void Message::print_msg() const
{
    for (char c : this->msg) {
        std::cout << c;
    }

    std::cout << '\n';
}


void Message::reset() 
{
    this->msg.clear();
}


const std::vector<char>& Message::get_msg_content()
{
    return this->msg;
}


} // namespace http 
