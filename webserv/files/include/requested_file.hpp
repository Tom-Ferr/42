#ifndef REQUESTED_FILE_HPP
# define REQUESTED_FILE_HPP

# include <string>
# include <fstream>
# include <cstdio>
# include <vector>
# include <request_parser.hpp>
# include <cgi.hpp>
# include <config.hpp>
# include <sys/stat.h>
# include <dirent.h>


class Req_File
{
private:
    std::string _content;
    size_t _size;
    std::string _status;
    std::string _req_file;
    Req_File(void);
    void isGET(Config const & conf, Req_Parser const & parser);
    void isPOST(Config const & conf, Req_Parser const & parser);
    void isDELETE(Config const & conf, Req_Parser const & parser);
    void readFile(std::ifstream & ifs);
    int setErrorPage(std::string const & path);
    std::string suffix(std::string type) const;
    void loadErrorPage(std::string const & status, std::string const & msn, Config const & conf);
public:
    Req_File(Config const & conf, Req_Parser const & parser);
    ~Req_File();
    Req_File(Req_File const & src);
    Req_File & operator=(Req_File const & rhs);

    std::string getContent() const;
    std::string getReqFile() const;
    size_t getSize() const;
    std::string getStatus() const;
    void resize(size_t const & len);
};

#endif