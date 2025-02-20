/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 11:39:35 by tde-cama          #+#    #+#             */
/*   Updated: 2022/01/06 19:04:47 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request_parser.hpp>

Req_Parser::Req_Parser(void){
    return ;
};

Req_Parser::Req_Parser(Receive const & re)
    : _req(re.bin()), _sock(re.getSock()), _body_len(0), _bad(false)  {

    std::stringstream s_req(_req);
    std::string token;
    std::getline(s_req, token, ' ');
    _method = token;
    std::getline(s_req, token, ' ');
    _file = token;
    std::getline(s_req, token, '\r');
    _version = token;
    
    std::string data;
    while (std::getline(s_req, token, '\r')){
        if(!token.compare("\n")){
            std::getline(s_req, token, '\n');
            break ;
        }
        if(token.find("Host") < std::string::npos){
            std::stringstream doi(token);
            std::getline(doi, data, ' ');
            std::getline(doi, data, ' ');
            _host = data;
        }
        else if(token.find("Content-Type") < std::string::npos){
            std::stringstream doi(token);
            std::getline(doi, data, ' ');
            std::getline(doi, data, ' ');
            _type = data;
            if (_type == "multipart/form-data;"){
                // _type.erase(_type.rfind(";"), 1);
                std::getline(doi, data, '=');
                std::getline(doi, data, '\r');
                _boundary = data;
            }
        }
        else if(token.find("Transfer-Encoding") < std::string::npos){
            std::stringstream doi(token);
            std::getline(doi, data, ' ');
            std::getline(doi, data, ' ');
            _trans_enc = data;
        }
        else if(token.find("Content-Length") < std::string::npos){
            std::stringstream doi(token);
            std::getline(doi, data, ' ');
            std::getline(doi, data, ' ');
            _s_blen = data;
            std::stringstream conv(data);
            conv >> _body_len;
        }
    }
    if(_method.empty() || _file.empty() || _version.empty() || _host.empty())
        _bad = true;
    
    if(!_method.compare("POST")){
        if(_s_blen.empty() && _trans_enc.empty())
            _bad = true;
        else if(!_trans_enc.compare("chunked"))
            unchunk();
        else
            readBody(_body_len);

        if(_type ==  "multipart/form-data;" && !_bad)
            parseBody();
    }


    size_t pos = _file.find("?");
    if(pos < std::string::npos){
        _query = _file;
        _query.erase(0, pos);
        _file.erase(pos);
    }
    pos = _file.find(".");
    size_t extra = _file.find("/", pos);
    if(extra < std::string::npos){
        _extra = _file;
        _extra.erase(0, extra);
        _file.erase(extra);
    }
};

Req_Parser::~Req_Parser(void){
    return ;
};

Req_Parser::Req_Parser(Req_Parser const & src){
    *this = src;
};

Req_Parser & Req_Parser::operator=(Req_Parser const & rhs){
    if (this != &rhs){
        this->_req = rhs._req;
        this->_method = rhs._method;
        this->_file = rhs._file;
        this->_extra = rhs._extra;
        this->_query = rhs._query;
        this->_version = rhs._version;
        this->_type = rhs._type;
        this->_body = rhs._body;
        this->_host = rhs._host;
        this->_trans_enc = rhs._trans_enc;
        this->_boundary = rhs._boundary;
        this->_up_fname = rhs._up_fname;
        this->_sock = rhs._sock;
        this->_body_len = rhs._body_len;
        this->_bad = rhs._bad;
    }
    return *this;
};

std::string Req_Parser::getFile() const{
    return _file;
};

std::string Req_Parser::getExtra() const{
    return _extra;
};

std::string Req_Parser::getQuery() const{
    return _query;
};

std::string Req_Parser::getVersion() const{
    return _version;
};

std::string Req_Parser::getType() const{
    return _type;
};

std::string Req_Parser::getMethod() const{
    return _method;
};

std::string Req_Parser::getBody() const{
    return _body;
};

std::string Req_Parser::getHost() const{
    return _host;
};

std::string Req_Parser::getUpFname() const{
    return _up_fname;
};

size_t Req_Parser::getBodyLen() const{
    return _body_len;
};

bool Req_Parser::isBad() const{
    return _bad;
};

void Req_Parser::readBody(size_t const & len){
    
    size_t not_read = len;      
    while(not_read > 0){
        char* buffer = new char[not_read];
        size_t bytes = recv(_sock, buffer, not_read, 0);
        _body.append(buffer, bytes);
        not_read -= bytes;
        delete [] buffer;
    }
};

void Req_Parser::readChunk(std::string & bin){
    char buffer[3] = {0};
    while (recv(_sock , buffer, 1, 0)){
        bin += buffer;
        if (bin.rfind("\r\n") < std::string::npos)
            break ;
    }
};

void Req_Parser::unchunk(){
    size_t chunk_size = true;
    while (chunk_size){
        std::string bin;
        readChunk(bin);
        bin.resize(bin.rfind("\r\n"));
        std::stringstream conv(bin);
        conv >> std::hex >> chunk_size;
        _body_len += chunk_size;
        if (chunk_size)
            readBody(chunk_size);
        readChunk(bin);
    }
};

void Req_Parser::parseBody(){
    _body.erase(_body.rfind(_boundary) - 4);
            std::stringstream body_stream(_body);
            std::string line;
            size_t head = 0;
            while(std::getline(body_stream, line, '\r')){
                if(line.find("Content-Type") < std::string::npos){
                    std::stringstream doi(line);
                    std::getline(doi, line, ' ');
                    std::getline(doi, line, ' ');
                    _type = line;
                }
                else if(line.find("Content-Disposition") < std::string::npos){
                    std::stringstream doi(line);
                    for(int i = 3; i; --i)
                        std::getline(doi, line, ' ');
                    std::getline(doi, line, '"');
                    std::getline(doi, line, '"');
                    _up_fname = line;
                }
                else if(line == "\n"){
                    head = body_stream.tellg();
                    break ;
                }
            }
            _body.erase(0, ++head);
            std::stringstream count(_body);
            count.seekg(0, std::ios_base::end);
            _body_len = count.tellg();
};
