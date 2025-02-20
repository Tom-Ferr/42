/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 11:39:35 by tde-cama          #+#    #+#             */
/*   Updated: 2022/01/05 13:07:23 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <response.hpp>

Response::Response(void){
    return ;
};

Response::Response(Req_File const & file, Req_Parser const & req){
    std::stringstream conv;
    if(file.getReqFile().rfind(".css") < std::string::npos)
        _type = "text/css";
    else if(file.getReqFile().rfind(".jpg") < std::string::npos)
        _type = "image/jpeg";
    else if(file.getReqFile().rfind(".gif") < std::string::npos)
        _type = "image/gif";
    else if(file.getReqFile().rfind(".png") < std::string::npos)
        _type = "image/png";
    else if(file.getReqFile().rfind(".mp3") < std::string::npos)
        _type = "audio/mpeg";
    else
        _type = "text/html";
    _res = req.getVersion() + " " + file.getStatus();
    _res += "\r\nContent-Type: " + _type;
    if (_res.rfind("text/") < std::string::npos)
        _res += ";charset=UTF-8";
    _res += "\r\nContent-Length: ";
    conv << file.getSize();
    conv >> _len;
    _res += _len;
    _res += "\r\n\r\n";
};

Response::~Response(void){
    return ;
};

Response::Response(Response const & src){
    *this = src;
};

Response & Response::operator=(Response const & rhs){
    if (this != &rhs){
        this->_res = rhs._res;
        this->_len = rhs._len;
        this->_type = rhs._type;
    }
    return *this;
};

std::string Response::getResponse() const{
    return _res;
};

size_t Response::getSize() const{
    return _res.length();
};
