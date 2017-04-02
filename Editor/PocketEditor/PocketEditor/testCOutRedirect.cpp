//
//  testCOutRedirect.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include <iostream>

class outbuf : public std::streambuf {
public:
    virtual std::streamsize xsputn(const char *msg, std::streamsize count){
        std::string s(msg,count);
        return count;
    }
};

int main_cOutRedirect() {
    // set std::cout to use my custom streambuf
    outbuf ob;
    std::streambuf *sb = std::cout.rdbuf(&ob);

    // do some work here
    std::cout << "Hello stranger" << std::endl;

    // make sure to restore the original so we don't get a crash on close!
    std::cout.rdbuf(sb);
    return 0;
    
}
